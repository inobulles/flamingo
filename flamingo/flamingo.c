// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo
// Copyright (c) 2025 Drake Fletcher

/*
 * Flamingo interpreter core.
 *
 * This file contains the main entry points for the Flamingo interpreter, including instance creation, destruction, and script execution.
 * It also manages the integration with Tree-sitter for parsing.
 */

#if __linux__
# define _GNU_SOURCE
#endif

// XXX Tree-sitter defines this, but 'features.h' on Linux defines this as well when '_DEFAULT_SOURCE' is set.
//     I don't know if this is bug in Tree-sitter or not (should it check if defined or undef itself?), but for now just undef it ourselves.

#undef _POSIX_C_SOURCE

#include "runtime/lib.c"

#include "parser.c"

#include "common.h"
#include "env.h"
#include "grammar/statement.h"
#include "highlight.h"
#include "primitive_type_member.h"
#include "scope.h"
#include "val.h"

#include <unistd.h>

typedef struct {
	TSParser* parser;
	TSTree* tree;
	TSNode root;
} ts_state_t;

extern TSLanguage const* tree_sitter_flamingo(void);

// Compute the 1-based line and column of a byte offset into the source.

static void compute_line_col(char const* src, size_t off, size_t* line, size_t* col) {
	*line = 1;
	*col = 1;

	for (size_t i = 0; i < off; i++) {
		if (src[i] == '\n') {
			(*line)++;
			*col = 1;
		}

		else {
			(*col)++;
		}
	}
}

__attribute__((format(printf, 2, 3))) int flamingo_raise_error(flamingo_t* flamingo, char const* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	// TODO validate the size of the program name

	// format caller's error message

	char formatted[sizeof flamingo->err];
	size_t res = vsnprintf(formatted, sizeof formatted, fmt, args);
	assert(res < sizeof formatted);

	va_end(args);

	// Snapshot the current source location on the first outstanding error, so that it survives until the error is printed.

	if (!flamingo->errors_outstanding) {
		flamingo->err_has_loc = flamingo->cur_node_has_loc;
		flamingo->err_start = flamingo->cur_node_start;
		flamingo->err_end = flamingo->cur_node_end;
	}

	// Prefix the message with its source location, if we have one.

	char located[sizeof flamingo->err];

	if (flamingo->cur_node_has_loc) {
		size_t line, col;
		compute_line_col(flamingo->src, flamingo->cur_node_start, &line, &col);
		res = snprintf(located, sizeof located, "%s:%zu:%zu: %s", flamingo->progname, line, col, formatted);
	}

	else {
		res = snprintf(located, sizeof located, "%s: %s", flamingo->progname, formatted);
	}

	assert(res < sizeof located);

	// format the new error message and concatenate to the previous one if there were still errors outstanding
	// TODO truncate the number of errors that we show at once (just do this by seeing how much longer we have)

	if (flamingo->errors_outstanding) {
		size_t const prev_len = strlen(flamingo->err);
		snprintf(flamingo->err + prev_len, sizeof flamingo->err - prev_len, ", %s", located);
	}

	else {
		snprintf(flamingo->err, sizeof flamingo->err, "%s", located);
	}

	flamingo->errors_outstanding = true;

	return -1;
}

// Print the source line containing the error, with the offending range underlined.

static void render_snippet(flamingo_t* flamingo, FILE* f) {
	char const* const src = flamingo->src;
	size_t const src_size = flamingo->src_size;

	size_t start = flamingo->err_start > src_size ? src_size : flamingo->err_start;
	size_t end = flamingo->err_end > src_size ? src_size : flamingo->err_end;

	// Find the bounds of the line containing the start of the range.

	size_t line_start = start;

	while (line_start > 0 && src[line_start - 1] != '\n') {
		line_start--;
	}

	size_t line_end = start;

	while (line_end < src_size && src[line_end] != '\n') {
		line_end++;
	}

	// Clamp the underlined range to this single line.

	if (end > line_end) {
		end = line_end;
	}

	if (end < start) {
		end = start;
	}

	size_t line, col;
	compute_line_col(src, start, &line, &col);

	// Syntax-highlight the line, but only when writing to a terminal (so piped or captured output stays plain).

	ts_state_t* const ts_state = flamingo->ts_state;
	size_t const line_len = line_end - line_start;

	uint8_t* colors = NULL;

	if (ts_state != NULL && isatty(fileno(f))) {
		colors = malloc(line_len);
	}

	bool const colorize = colors != NULL;

	if (colorize) {
		highlight_range(ts_state->root, line_start, line_end, colors);
	}

	// Source line, with a line-number gutter.

	fprintf(f, " %zu | ", line);

	if (colorize) {
		uint8_t cur = 0;

		for (size_t i = line_start; i < line_end; i++) {
			uint8_t const color = colors[i - line_start];

			if (color != cur) {
				fprintf(f, "\x1b[%um", color); // A code of 0 resets to the default colour.
				cur = color;
			}

			fputc(src[i], f);
		}

		if (cur != 0) {
			fputs("\x1b[0m", f);
		}
	}

	else {
		fprintf(f, "%.*s", (int) line_len, src + line_start);
	}

	fputc('\n', f);
	free(colors);

	// Underline, aligned under the source line.
	// We reproduce leading tabs so that the carets line up when the source is indented with tabs.

	int const gutter = snprintf(NULL, 0, " %zu ", line);
	fprintf(f, "%*s| ", gutter, "");

	for (size_t i = line_start; i < start; i++) {
		fputc(src[i] == '\t' ? '\t' : ' ', f);
	}

	if (colorize) {
		fputs("\x1b[1;31m", f); // Bold red.
	}

	if (end == start) {
		fputc('^', f);
	}

	else {
		for (size_t i = start; i < end; i++) {
			fputc(i == start ? '^' : '~', f);
		}
	}

	if (colorize) {
		fputs("\x1b[0m", f);
	}

	fputc('\n', f);
}

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src, size_t src_size) {
	flamingo->consistent = false;

	// Set initial state up.

	flamingo->progname = progname;
	flamingo->errors_outstanding = false;

	flamingo->cur_node_has_loc = false;
	flamingo->cur_node_start = 0;
	flamingo->cur_node_end = 0;

	flamingo->err_has_loc = false;
	flamingo->err_start = 0;
	flamingo->err_end = 0;

	flamingo->ts_state = NULL;

	flamingo->src = src;
	flamingo->src_size = src_size;

	flamingo->inherited_env = false;
	flamingo->env = NULL;

	flamingo->import_count = 0;
	flamingo->imported_srcs = NULL;
	flamingo->imported_flamingos = NULL;

	flamingo->import_path_count = 0;
	flamingo->import_paths = NULL;

	flamingo->cur_fn_body = NULL;
	flamingo->cur_fn_rv = NULL;

	flamingo->in_loop = 0;

	// Set up Tree-sitter and parser.

	ts_state_t* const ts_state = calloc(1, sizeof *ts_state);

	if (ts_state == NULL) {
		return error(flamingo, "failed to allocate memory for Tree-sitter state");
	}

	TSParser* const parser = ts_parser_new();

	if (parser == NULL) {
		error(flamingo, "failed to create Tree-sitter parser");
		goto err_ts_parser_new;
	}

	ts_state->parser = parser;

	TSLanguage const* const lang = tree_sitter_flamingo();
	ts_parser_set_language(parser, lang);

	TSTree* const tree = ts_parser_parse_string(parser, NULL, src, src_size);

	if (tree == NULL) {
		error(flamingo, "failed to parse source");
		goto err_ts_parser_parse_string;
	}

	ts_state->tree = tree;
	ts_state->root = ts_tree_root_node(tree);

	// TODO make sure tree is coherent
	//      I don't know if Tree-sitter has a simple way to check AST-coherency itself but otherwise just go down the tree and look for any MISSING or UNEXPECTED nodes

	flamingo->ts_state = ts_state;

	// Set primitive type members.

	primitive_type_member_init(flamingo);

	if (primitive_type_member_std(flamingo) < 0) {
		goto err_primitive_type_member_std;
	}

	flamingo->consistent = true;
	return 0;

err_primitive_type_member_std:

	ts_tree_delete(tree);

err_ts_parser_parse_string:

	ts_parser_delete(parser);

err_ts_parser_new:

	free(ts_state);

	return -1;
}

void flamingo_destroy(flamingo_t* flamingo) {
	if (!flamingo->consistent) {
		return;
	}

	// Free all the Tree-sitter-related stuff.

	ts_state_t* const ts_state = flamingo->ts_state;

	ts_tree_delete(ts_state->tree);
	ts_parser_delete(ts_state->parser);

	free(ts_state);

	// If we didn't inherit our scope stack, free it and all the scopes on it.

	if (!flamingo->inherited_env && flamingo->env != NULL) {
		for (size_t i = 0; i < flamingo->env->scope_stack_size; i++) {
			scope_empty(flamingo->env->scope_stack[i]);
		}

		env_free(flamingo->env);
	}

	// If we imported anything, free all the created flamingo instances and their sources.

	for (size_t i = 0; i < flamingo->import_count; i++) {
		flamingo_t* const imported_flamingo = &flamingo->imported_flamingos[i];
		char* const src = flamingo->imported_srcs[i];

		flamingo_destroy(imported_flamingo);
		free(src);
	}

	if (flamingo->imported_flamingos != NULL) {
		free(flamingo->imported_flamingos);
	}

	if (flamingo->imported_srcs != NULL) {
		free(flamingo->imported_srcs);
	}

	// Free the import paths.

	for (size_t i = 0; i < flamingo->import_path_count; i++) {
		free(flamingo->import_paths[i]);
	}

	if (flamingo->import_paths != NULL) {
		free(flamingo->import_paths);
	}

	// Finally, free the primitive type members.

	primitive_type_member_free(flamingo);

	flamingo->consistent = false;
}

char* flamingo_err(flamingo_t* flamingo) {
	if (!flamingo->errors_outstanding) {
		return "no errors";
	}

	flamingo->errors_outstanding = false;
	return flamingo->err;
}

void flamingo_print_error(flamingo_t* flamingo, FILE* f) {
	if (!flamingo->errors_outstanding) {
		fprintf(f, "no errors\n");
		return;
	}

	fprintf(f, "%s\n", flamingo->err);

	if (flamingo->err_has_loc && flamingo->src != NULL) {
		render_snippet(flamingo, f);
	}

	flamingo->errors_outstanding = false;
}

void flamingo_register_external_fn_cb(flamingo_t* flamingo, flamingo_external_fn_cb_t cb, void* data) { // TODO Better name for this.
	flamingo->external_fn_cb = cb;
	flamingo->external_fn_cb_data = data;
}

void flamingo_register_class_decl_cb(flamingo_t* flamingo, flamingo_class_decl_cb_t cb, void* data) {
	flamingo->class_decl_cb = cb;
	flamingo->class_decl_cb_data = data;
}

void flamingo_register_class_inst_cb(flamingo_t* flamingo, flamingo_class_inst_cb_t cb, void* data) {
	flamingo->class_inst_cb = cb;
	flamingo->class_inst_cb_data = data;
}

void flamingo_add_import_path(flamingo_t* flamingo, char* path) {
	char* const duped = strdup(path);
	assert(duped != NULL);

	flamingo->import_paths = realloc(flamingo->import_paths, (flamingo->import_path_count + 1) * sizeof *flamingo->import_paths);
	assert(flamingo->import_paths != NULL);

	flamingo->import_paths[flamingo->import_path_count++] = duped;
}

static int parse(flamingo_t* flamingo, TSNode node) {
	size_t const n = ts_node_child_count(node);

	for (size_t i = 0; i < n; i++) {
		TSNode const child = ts_node_child(node, i);

		if (parse_statement(flamingo, child) < 0) {
			return -1;
		}
	}

	return 0;
}

int flamingo_inherit_env(flamingo_t* flamingo, flamingo_env_t* env) {
	if (flamingo->env != NULL) {
		assert(flamingo->env->scope_stack_size == 0);
		return error(flamingo, "there is already an environment on this flamingo instance");
	}

	flamingo->inherited_env = true;
	flamingo->env = env;

	return 0;
}

int flamingo_run(flamingo_t* flamingo) {
	ts_state_t* const ts_state = flamingo->ts_state;
	assert(strcmp(ts_node_type(ts_state->root), "source_file") == 0);

	if (!flamingo->inherited_env) {
		if (flamingo->env != NULL) {
			env_free(flamingo->env);
		}

		flamingo->env = env_alloc();
		env_push_scope(flamingo->env);
	}

	return parse(flamingo, ts_state->root);
}

flamingo_var_t* flamingo_find_var(flamingo_t* flamingo, char const* key, size_t key_size) {
	return env_find_var(flamingo->env, key, key_size);
}

flamingo_val_t* flamingo_val_incref(flamingo_val_t* val) {
	return val_incref(val);
}

flamingo_val_t* flamingo_val_decref(flamingo_val_t* val) {
	return val_decref(val);
}

flamingo_val_t* flamingo_val_make_none(void) {
	return val_alloc();
}

flamingo_val_t* flamingo_val_make_int(int64_t integer) {
	flamingo_val_t* const val = val_alloc();

	val->kind = FLAMINGO_VAL_KIND_INT;
	val->integer.integer = integer;

	return val;
}

flamingo_val_t* flamingo_val_make_str(size_t size, char* str) {
	flamingo_val_t* const val = val_alloc();

	val->kind = FLAMINGO_VAL_KIND_STR;

	val->str.size = size;
	val->str.str = malloc(size);
	assert(val->str.str != NULL);
	memcpy(val->str.str, str, size);

	return val;
}

flamingo_val_t* flamingo_val_make_cstr(char* str) {
	return flamingo_val_make_str(strlen(str), str);
}

flamingo_val_t* flamingo_val_make_bool(bool boolean) {
	flamingo_val_t* const val = val_alloc();

	val->kind = FLAMINGO_VAL_KIND_BOOL;
	val->boolean.boolean = boolean;

	return val;
}

flamingo_val_t* flamingo_inst_get(flamingo_val_t* inst, char const* name) {
	flamingo_scope_t* const scope = inst->inst.scope;

	for (size_t i = 0; i < scope->vars_size; i++) {
		flamingo_var_t* const var = &scope->vars[i];

		if (flamingo_cstrcmp(var->key, name, var->key_size) == 0) {
			return var->val;
		}
	}

	return NULL;
}

int flamingo_inst_set(flamingo_val_t* inst, char const* name, flamingo_val_t* val) {
	flamingo_scope_t* const scope = inst->inst.scope;

	for (size_t i = 0; i < scope->vars_size; i++) {
		flamingo_var_t* const var = &scope->vars[i];

		if (flamingo_cstrcmp(var->key, name, var->key_size) == 0) {
			flamingo_val_decref(var->val);
			var->val = val;
			return 0;
		}
	}

	return -1;
}
