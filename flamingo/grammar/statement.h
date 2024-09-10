// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "assert.h"
#include "assignment.h"
#include "block.h"
#include "function_declaration.h"
#include "import.h"
#include "print.h"
#include "return.h"
#include "var_decl.h"

#include <common.h>

static int parse_statement(flamingo_t* flamingo, TSNode node) {
	assert(strcmp(ts_node_type(node), "statement") == 0);
	assert(ts_node_child_count(node) == 1);

	if (
		flamingo->cur_fn_body != NULL &&                       // In function?
		flamingo->cur_fn_rv != NULL &&                         // Returning?
		memcmp(flamingo->cur_fn_body, &node, sizeof node) != 0 // Not the body node?
	) {
		return 0;
	}

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "comment") == 0) {
		return 0;
	}

	if (strcmp(type, "doc_comment") == 0) {
		return 0;
	}

	else if (strcmp(type, "block") == 0) {
		return parse_block(flamingo, child, NULL);
	}

	else if (strcmp(type, "print") == 0) {
		return parse_print(flamingo, child);
	}

	else if (strcmp(type, "return") == 0) {
		return parse_return(flamingo, child);
	}

	else if (strcmp(type, "assert") == 0) {
		return parse_assert(flamingo, child);
	}

	else if (strcmp(type, "var_decl") == 0) {
		return parse_var_decl(flamingo, child);
	}

	else if (strcmp(type, "assignment") == 0) {
		return parse_assignment(flamingo, child);
	}

	else if (strcmp(type, "function_declaration") == 0) {
		return parse_function_declaration(flamingo, child, FLAMINGO_FN_KIND_FUNCTION);
	}

	else if (strcmp(type, "class_declaration") == 0) {
		return parse_function_declaration(flamingo, child, FLAMINGO_FN_KIND_CLASS);
	}

	else if (strcmp(type, "proto") == 0) {
		return parse_function_declaration(flamingo, child, FLAMINGO_FN_KIND_PROTO);
	}

	else if (strcmp(type, "expression") == 0) {
		return parse_expr(flamingo, child, NULL, NULL);
	}

	else if (strcmp(type, "import") == 0) {
		return parse_import(flamingo, child);
	}

	return error(flamingo, "unknown statment type: %s", type);
}
