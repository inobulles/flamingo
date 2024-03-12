// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#include "runtime/lib.c"
#include "parser.c"

#include "flamingo.h"
#include "scope.c"
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

static int parse_literal(flamingo_t* flamingo, TSNode node, flamingo_expr_t* expr) {
	assert(strcmp(ts_node_type(node), "literal") == 0);
	assert(ts_node_child_count(node) == 1);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "string") == 0) {
		expr->kind = FLAMINGO_EXPR_KIND_STR;

		size_t const start = ts_node_start_byte(child);
		size_t const end = ts_node_end_byte(child);

		// XXX remove one from each side as we don't want the quotes

		expr->str.str = flamingo->src + start + 1;
		expr->str.size = end - start - 2;

		return 0;
	}

	if (strcmp(type, "number") == 0) {
		return error(flamingo, "number literals are not yet supported");
	}

	return error(flamingo, "unknown literal type: %s", type);
}

static int parse_expr(flamingo_t* flamingo, TSNode node, flamingo_expr_t* expr) {
	assert(strcmp(ts_node_type(node), "expression") == 0);
	assert(ts_node_child_count(node) == 1);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "literal") == 0) {
		return parse_literal(flamingo, child, expr);
	}

	return error(flamingo, "unknown expression type: %s", type);
}

static int parse_print(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 2);

	TSNode const msg = ts_node_child_by_field_name(node, "msg", 3);
	char const* const type = ts_node_type(msg);

	if (strcmp(ts_node_type(msg), "expression") != 0) {
		return error(flamingo, "expected expression for message, got %s", type);
	}

	flamingo_expr_t expr;

	if (parse_expr(flamingo, msg, &expr) < 0) {
		return -1;
	}

	if (expr.kind == FLAMINGO_EXPR_KIND_STR) {
		printf("%.*s\n", (int) expr.str.size, expr.str.str);
		return 0;
	}

	return error(flamingo, "can't print expression kind: %d", expr.kind);
}

static int parse_assignment(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 3);

	TSNode const right = ts_node_child_by_field_name(node, "right", 5);
	char const* const right_type = ts_node_type(right);

	if (strcmp(right_type, "expression") != 0) {
		return error(flamingo, "expected expression for name, got %s", right_type);
	}

	// get identifier name

	TSNode const left = ts_node_child_by_field_name(node, "left", 4);
	char const* const left_type = ts_node_type(left);

	if (strcmp(left_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for name, got %s", left_type);
	}

	size_t const start = ts_node_start_byte(left);
	size_t const end = ts_node_end_byte(left);

	char const* const identifier = flamingo->src + start;
	size_t const size = end - start;

	// check if identifier is already in scope (or a previous one) and declare it

	flamingo_var_t* var = flamingo_scope_find_var(flamingo, identifier, size);

	if (var == NULL) {
		var = scope_add_var(&flamingo->scope_stack[flamingo->scope_stack_size - 1], identifier, size);
	}

	// evaluate expression

	if (parse_expr(flamingo, right, &var->expr) < 0) {
		return -1;
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
