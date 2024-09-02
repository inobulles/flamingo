// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"

#include <common.h>
#include <val.c>

static int parse_return(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 2);

	TSNode const rv_node = ts_node_child_by_field_name(node, "rv", 2);
	char const* const type = ts_node_type(rv_node);

	if (strcmp(ts_node_type(rv_node), "expression") != 0) {
		return error(flamingo, "expected expression for return value, got %s", type);
	}

	// Parse the return value expression.
	// State of the current function return value should always be clean before this.

	assert(flamingo->cur_fn_rv == NULL);

	if (parse_expr(flamingo, rv_node, &flamingo->cur_fn_rv) < 0) {
		return -1;
	}

	return 0;
}
