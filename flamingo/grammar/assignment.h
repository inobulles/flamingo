// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"

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

	// Make sure identifier is already in scope (or a previous one).

	flamingo_var_t* const var = flamingo_scope_find_var(flamingo, identifier, size);

	if (var == NULL) {
		return error(flamingo, "'%.*s' was never declared", (int) size, identifier);
	}

	// Parse RHS expression (don't forget to decrement the reference counter of the previous value!) and primitive type checking:
	// - A function or a class can never be reassigned.
	// - A variable can't be assigned a value of a different type except if it was none.
	// - TODO This is also where the const qualifier will be checked too (but this feature is to be defined).

	flamingo_val_kind_t const prev_type = var->val->kind;
	char const* const prev_type_str = val_type_str(var->val);

	if (prev_type == FLAMINGO_VAL_KIND_FN) {
		return error(flamingo, "cannot assign to %s '%.*s'", val_role_str(var->val), (int) size, identifier);
	}

	val_decref(var->val);
	var_set_val(var, NULL);

	if (parse_expr(flamingo, right_node, &var->val) < 0) {
		return -1;
	}

	if (var->val->kind != prev_type && (prev_type != FLAMINGO_VAL_KIND_NONE && var->val->kind != FLAMINGO_VAL_KIND_NONE)) {
		return error(flamingo, "cannot assign %s to '%.*s' (%s)", val_type_str(var->val), (int) size, identifier, prev_type_str);
	}

	return 0;
}
