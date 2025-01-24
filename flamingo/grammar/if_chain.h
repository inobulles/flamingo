// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2025 Aymeric Wibo

#pragma once

#include "../common.h"

static int parse_if_chain(flamingo_t* flamingo, TSNode node) {
	// Get if condition.

	TSNode const if_condition_node = ts_node_child_by_field_name(node, "condition", 9);
	char const* const if_condition_type = ts_node_type(if_condition_node);

	if (strcmp(if_condition_type, "expression") != 0) {
		return error(flamingo, "expected expression for if condition, got %s", if_condition_type);
	}

	// Get if body.

	TSNode const if_body_node = ts_node_child_by_field_name(node, "body", 4);
	char const* const if_body_type = ts_node_type(if_body_node);

	if (strcmp(if_body_type, "block") != 0) {
		return error(flamingo, "expected block for if body, got %s", if_body_type);
	}

	// Get elif conditions.
	// The first named child we should encounter (other than the if) is the first elif condition.
	// Right after it, we should encounter it's respective body.

	for (size_t i = 0; i < ts_node_child_count(node); i++) {
		TSNode const child = ts_node_child(node, i);

		if (!ts_node_is_named(child)) {
			continue;
		}

		char const* const name = ts_node_field_name_for_child(node, i);

		// If we see any one of the other accepted nodes, skip it.

		if (
			strcmp(name, "condition") == 0 ||
			strcmp(name, "body") == 0 ||
			strcmp(name, "else_body") == 0
		) {
			continue;
		}

		if (strcmp(name, "elif_condition") != 0) {
			return error(flamingo, "expected elif_condition, got %s", name);
		}

		// We now know that child was an elif condition.

		TSNode const elif_condition_node = child;
		char const* const elif_condition_type = ts_node_type(elif_condition_node);

		if (strcmp(elif_condition_type, "expression") != 0) {
			return error(flamingo, "expected expression for elif condition, got %s", elif_condition_type);
		}

		// Go to the next named node, which should be the body of this elif statement.

		TSNode const elif_body_node = ts_node_next_sibling(elif_condition_node);
		char const* const elif_body_type = ts_node_type(elif_body_node);

		if (strcmp(elif_body_type, "block") != 0) {
			return error(flamingo, "expected block for elif body, got %s", elif_body_type);
		}

		printf("elif condition: %.*s\n", (int) ts_node_end_byte(elif_condition_node) - ts_node_start_byte(elif_condition_node), flamingo->src + ts_node_start_byte(elif_condition_node));

		// Jump forward so we don't hit the elif body.

		i++;
	}

	// Get else body.

	TSNode const else_body_node = ts_node_child_by_field_name(node, "else_body", 9);
	bool const has_else_body = !ts_node_is_null(else_body_node);

	if (has_else_body) {
		char const* const else_body_type = ts_node_type(else_body_node);

		if (strcmp(else_body_type, "block") != 0) {
			return error(flamingo, "expected block for else body, got %s", else_body_type);
		}
	}

	return 0;
}
