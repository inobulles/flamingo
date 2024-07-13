// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#include "runtime/lib.c"
#include "parser.c"

#include "flamingo.h"
#include "scope.c"
#include "val.c"
#include "runtime/tree_sitter/api.h"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	TSParser* parser;
	TSTree* tree;
	TSNode root;
} ts_state_t;

extern TSLanguage const* tree_sitter_flamingo(void);

__attribute__((format(printf, 2, 3)))
static int error(flamingo_t* flamingo, char const* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	// TODO validate the size of the program name

	// format caller's error message

	char formatted[sizeof flamingo->err];
	size_t res = vsnprintf(formatted, sizeof formatted, fmt, args);
	assert(res < sizeof formatted);

	// format the new error message and concatenate to the previous one if there were still errors outstanding
	// TODO truncate the number of errors that we show at once (just do this by seeing how much longer we have)

	if (flamingo->errors_outstanding) {
		size_t const prev_len = strlen(flamingo->err);
		res = snprintf(flamingo->err + prev_len, sizeof flamingo->err - prev_len, ", %s:%d:%d: %s", flamingo->progname, 0, 0, formatted);
	}

	else {
		res = snprintf(flamingo->err, sizeof flamingo->err, "%s:%d:%d: %s", flamingo->progname, 0, 0, formatted);
	}

	assert(res < sizeof flamingo->err);

	va_end(args);
	flamingo->errors_outstanding = true;

	return -1;
}

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src, size_t src_size) {
	flamingo->progname = progname;
	flamingo->errors_outstanding = false;

	flamingo->src = src;
	flamingo->src_size = src_size;

	flamingo->scope_stack = NULL;

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

	return 0;

	ts_tree_delete(tree);

err_ts_parser_parse_string:

	ts_parser_delete(parser);

err_ts_parser_new:

	free(ts_state);

	return -1;
}

void flamingo_destroy(flamingo_t* flamingo) {
	ts_state_t* const ts_state = flamingo->ts_state;
	
	ts_tree_delete(ts_state->tree);
	ts_parser_delete(ts_state->parser);

	free(ts_state);

	if (flamingo->scope_stack != NULL) {
		for (size_t i = 0; i < flamingo->scope_stack_size; i++) {
			scope_free(&flamingo->scope_stack[i]);
		}

		free(flamingo->scope_stack);
	}
}

char* flamingo_err(flamingo_t* flamingo) {
	if (!flamingo->errors_outstanding) {
		return "no errors";
	}

	flamingo->errors_outstanding = false;
	return flamingo->err;
}

void flamingo_register_cb_call(flamingo_t* flamingo, flamingo_cb_call_t cb, void* data) {
	fprintf(stderr, "%s: not implemented\n", __func__);
}

static int parse(flamingo_t* flamingo, TSNode node);

static int parse_list(flamingo_t* flamingo, TSNode node) {
	size_t const n = ts_node_child_count(node);

	for (size_t i = 0; i < n; i++) {
		TSNode const child = ts_node_child(node, i);
		
		if (parse(flamingo, child) < 0) {
			return -1;
		}
	}

	return 0;
}

static int parse_identifier(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "identifier") == 0);
	assert(ts_node_child_count(node) == 0);

	size_t const start = ts_node_start_byte(node);
	size_t const end = ts_node_end_byte(node);

	char const* const identifier = flamingo->src + start;
	size_t const size = end - start;

	flamingo_var_t* const var = flamingo_scope_find_var(flamingo, identifier, size);

	if (var == NULL) {
		return error(flamingo, "could not find identifier: %.*s", (int) size, identifier);
	}

	*val = var->val;
	val_incref(*val);

	return 0;
}

static int parse_literal(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "literal") == 0);
	assert(ts_node_child_count(node) == 1);

	// if value already exists, free it
	// XXX not sure when this is the case, just doing this for now to be safe!

	if (*val != NULL) {
		val_free(*val);
		val_init(*val);
	}

	// otherwise, allocate it

	else {
		*val = val_alloc();
	}

	// in any case, val should not be NULL from this point forth

	assert(*val != NULL);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "string") == 0) {
		(*val)->kind = FLAMINGO_VAL_KIND_STR;

		size_t const start = ts_node_start_byte(child);
		size_t const end = ts_node_end_byte(child);

		// XXX remove one from each side as we don't want the quotes

		(*val)->str.size = end - start - 2;
		(*val)->str.str = malloc((*val)->str.size);

		assert((*val)->str.str != NULL);
		memcpy((*val)->str.str, flamingo->src + start + 1, (*val)->str.size);

		return 0;
	}

	if (strcmp(type, "number") == 0) {
		return error(flamingo, "number literals are not yet supported");
	}

	return error(flamingo, "unknown literal type: %s", type);
}

static int parse_expr(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "expression") == 0);
	assert(ts_node_child_count(node) == 1);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "literal") == 0) {
		return parse_literal(flamingo, child, val);
	}

	if (strcmp(type, "identifier") == 0) {
		return parse_identifier(flamingo, child, val);
	}

	return error(flamingo, "unknown expression type: %s", type);
}

static int parse_print(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 2);

	TSNode const msg_node = ts_node_child_by_field_name(node, "msg", 3);
	char const* const type = ts_node_type(msg_node);

	if (strcmp(ts_node_type(msg_node), "expression") != 0) {
		return error(flamingo, "expected expression for message, got %s", type);
	}

	flamingo_val_t* val = NULL;

	if (parse_expr(flamingo, msg_node, &val) < 0) {
		return -1;
	}

	// XXX Don't forget to decrement reference at the end!

	if (val->kind == FLAMINGO_VAL_KIND_STR) {
		printf("%.*s\n", (int) val->str.size, val->str.str);
		val_decref(val);

		return 0;
	}

	val_decref(val);
	return error(flamingo, "can't print expression kind: %d", val->kind);
}

static int parse_assignment(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 3);

	// Get RHS expression.

	TSNode const right_node = ts_node_child_by_field_name(node, "right", 5);
	char const* const right_type = ts_node_type(right_node);

	if (strcmp(right_type, "expression") != 0) {
		return error(flamingo, "expected expression for name, got %s", right_type);
	}

	// Get identifier name.

	TSNode const left_node = ts_node_child_by_field_name(node, "left", 4);
	char const* const left_type = ts_node_type(left_node);

	if (strcmp(left_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for name, got %s", left_type);
	}

	size_t const start = ts_node_start_byte(left_node);
	size_t const end = ts_node_end_byte(left_node);

	char const* const identifier = flamingo->src + start;
	size_t const size = end - start;

	// Check if identifier is already in scope (or a previous one) and declare it if not.

	flamingo_var_t* var = flamingo_scope_find_var(flamingo, identifier, size);

	if (var == NULL) {
		var = scope_add_var(cur_scope(flamingo), identifier, size);
	}

	// If variable is already in current or previous scope, since we're assigning a new value to it, we must decrement the reference counter of the previous value which was in the variable.

	else {
		val_decref(var->val);
		var->val = NULL;
	}

	// Evaluate expression.

	if (parse_expr(flamingo, right_node, &var->val) < 0) {
		return -1;
	}

	return 0;
}

static int parse_function_declaration(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 5);

	// Get qualifier list.

	TSNode const qualifiers_node = ts_node_child_by_field_name(node, "qualifiers", 10);
	bool const has_qualifiers = !ts_node_is_null(qualifiers_node);

	if (has_qualifiers) {
		char const* const qualifiers_type = ts_node_type(qualifiers_node);

		if (strcmp(qualifiers_type, "qualifier_list") != 0) {
			return error(flamingo, "expected qualifier_list for qualifiers, got %s", qualifiers_type);
		}
	}

	// Get function name.

	TSNode const name_node = ts_node_child_by_field_name(node, "name", 4);
	char const* const name_type = ts_node_type(name_node);

	if (strcmp(name_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for function name, got %s", name_type);
	}

	size_t const start = ts_node_start_byte(name_node);
	size_t const end = ts_node_end_byte(name_node);

	char const* const name = flamingo->src + start;
	size_t const size = end - start;

	// Get function parameters.

	TSNode const params = ts_node_child_by_field_name(node, "params", 6);
	bool const has_params = !ts_node_is_null(params);

	if (has_params) {
		char const* const params_type = ts_node_type(params);

		if (strcmp(params_type, "param_list") != 0) {
			return error(flamingo, "expected param_list for parameters, got %s", params_type);
		}
	}

	// Get function body.

	TSNode const body = ts_node_child_by_field_name(node, "body", 4);
	char const* const body_type = ts_node_type(body);

	if (strcmp(body_type, "statement") != 0) {
		return error(flamingo, "expected statement for body, got %s", body_type);
	}

	// Check if identifier is already in scope (or a previous one) and error if it is.
	// Right now, redeclaring functions is not allowed.
	// Although this will probably work a bit differently once function prototypes are added.

	flamingo_var_t* const prev_var = flamingo_scope_find_var(flamingo, name, size);

	if (prev_var != NULL) {
		char const* const thing = prev_var->val->kind == FLAMINGO_VAL_KIND_FN ? "function" : "variable";
		return error(flamingo, "the %s '%.*s' has already been declared in this scope", thing, (int) size, name);
	}

	return 0;
}

static int parse_statement(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 1);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "print") == 0) {
		return parse_print(flamingo, child);
	}

	if (strcmp(type, "assignment") == 0) {
		return parse_assignment(flamingo, child);
	}

	if (strcmp(type, "function_declaration") == 0) {
		return parse_function_declaration(flamingo, child);
	}

	return error(flamingo, "unknown statment type: %s", type);
}

static int parse(flamingo_t* flamingo, TSNode node) {
	char const* const type = ts_node_type(node);

	if (strcmp(type, "source_file") == 0) {
		return parse_list(flamingo, node);
	}

	if (strcmp(type, "statement") == 0) {
		return parse_statement(flamingo, node);
	}

	return error(flamingo, "unknown node type: %s", type);
}

int flamingo_run(flamingo_t* flamingo) {
	ts_state_t* const ts_state = flamingo->ts_state;
	assert(strcmp(ts_node_type(ts_state->root), "source_file") == 0);

	if (flamingo->scope_stack != NULL) {
		free(flamingo->scope_stack);
	}

	flamingo->scope_stack_size = 0;
	flamingo->scope_stack = NULL;

	scope_stack_push(flamingo);

	printf("%s\n", ts_node_string(ts_state->root));

	return parse(flamingo, ts_state->root);
}
