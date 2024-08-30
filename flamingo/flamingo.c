// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#include "parser.c"
#include "runtime/lib.c"

#include <common.h>
#include <grammar/statement.h>
#include <scope.c>

typedef struct {
	TSParser* parser;
	TSTree* tree;
	TSNode root;
} ts_state_t;

extern TSLanguage const* tree_sitter_flamingo(void);

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src, size_t src_size) {
	flamingo->progname = progname;
	flamingo->errors_outstanding = false;

	flamingo->src = src;
	flamingo->src_size = src_size;

	flamingo->inherited_scope_stack = false;
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

	if (!flamingo->inherited_scope_stack && flamingo->scope_stack != NULL) {
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

static int parse(flamingo_t* flamingo, TSNode node) {
	size_t const n = ts_node_child_count(node);

	for (size_t i = 0; i < n; i++) {
		TSNode const child = ts_node_child(node, i);

		if (strcmp(ts_node_type(child), "comment") == 0) {
			continue;
		}

		if (parse_statement(flamingo, child) < 0) {
			return -1;
		}
	}

	return 0;
}

int flamingo_inherit_scope_stack(flamingo_t* flamingo, size_t stack_size, flamingo_scope_t* stack) {
	if (flamingo->scope_stack != NULL) {
		assert(flamingo->scope_stack_size == 0);
		return error(flamingo, "there already is a scope stack on this flamingo instance");
	}

	flamingo->inherited_scope_stack = true;
	flamingo->scope_stack_size = stack_size;
	flamingo->scope_stack = stack;

	return 0;
}

int flamingo_run(flamingo_t* flamingo) {
	ts_state_t* const ts_state = flamingo->ts_state;
	assert(strcmp(ts_node_type(ts_state->root), "source_file") == 0);

	if (!flamingo->inherited_scope_stack) {
		if (flamingo->scope_stack != NULL) {
			free(flamingo->scope_stack);
		}

		flamingo->scope_stack_size = 0;
		flamingo->scope_stack = NULL;

		scope_stack_push(flamingo);
	}

	return parse(flamingo, ts_state->root);
}
