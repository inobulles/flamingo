// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "assignment.h"
#include "block.h"
#include "function_declaration.h"
#include "import.h"
#include "print.h"

#include <common.h>

static int parse_statement(flamingo_t* flamingo, TSNode node) {
	assert(strcmp(ts_node_type(node), "statement") == 0);
	assert(ts_node_child_count(node) == 1);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "block") == 0) {
		return parse_block(flamingo, child);
	}

	else if (strcmp(type, "print") == 0) {
		return parse_print(flamingo, child);
	}

	else if (strcmp(type, "assignment") == 0) {
		return parse_assignment(flamingo, child);
	}

	else if (strcmp(type, "function_declaration") == 0) {
		return parse_function_declaration(flamingo, child);
	}

	else if (strcmp(type, "expression") == 0) {
		return parse_expr(flamingo, child, NULL);
	}

	else if (strcmp(type, "import") == 0) {
		return parse_import(flamingo, child);
	}

	return error(flamingo, "unknown statment type: %s", type);
}
