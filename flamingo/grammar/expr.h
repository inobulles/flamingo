// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "call.h"
#include "identifier.h"
#include "literal.h"

#include <common.h>

static int parse_expr(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "expression") == 0);
	assert(ts_node_child_count(node) == 1);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	// 'val == NULL' means that we don't care about the result of the expression and can discard it.
	// These types of expressions are dead-ends if we're discarding the value and they can't have side-effect either, so just don't parse them.

	if (val != NULL && strcmp(type, "literal") == 0) {
		return parse_literal(flamingo, child, val);
	}

	if (val != NULL && strcmp(type, "identifier") == 0) {
		return parse_identifier(flamingo, child, val);
	}

	// These expressions do have side-effects, so we need to parse them anyway.

	if (strcmp(type, "call") == 0) {
		return parse_call(flamingo, child, val);
	}

	return error(flamingo, "unknown expression type: %s", type);
}
