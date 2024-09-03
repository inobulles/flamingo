// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>
#include <scope.c>
#include <val.c>

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
	// If it's a function, error.

	flamingo_var_t* var = flamingo_scope_find_var(flamingo, identifier, size);

	if (var == NULL) {
		var = scope_add_var(cur_scope(flamingo), identifier, size);
	}

	else if (var->val->kind == FLAMINGO_VAL_KIND_FN || var->val->kind == FLAMINGO_VAL_KIND_CLASS) {
		return error(flamingo, "cannot assign to %s '%.*s'", val_role_str(var->val), (int) size, identifier);
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
