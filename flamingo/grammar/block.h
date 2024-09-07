// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "statement.h"

#include <common.h>
#include <scope.c>

static int parse_block(flamingo_t* flamingo, TSNode node, flamingo_scope_t** inner_scope) {
	assert(strcmp(ts_node_type(node), "block") == 0);

	scope_stack_push(flamingo);
	size_t const n = ts_node_named_child_count(node);

	for (size_t i = 0; i < n; i++) {
		TSNode const child = ts_node_named_child(node, i);
		char const* const child_type = ts_node_type(child);

		if (strcmp(child_type, "statement") != 0) {
			return error(flamingo, "expected statement in block, got %s", child_type);
		}

		if (parse_statement(flamingo, child) < 0) {
			return -1;
		}
	}

	if (inner_scope == NULL) {
		scope_pop(flamingo);
	}

	else {
		*inner_scope = scope_gently_detach(flamingo);
	}

	return 0;
}
