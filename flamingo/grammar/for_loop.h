// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2025 Aymeric Wibo

#pragma once

#include "../common.h"
#include "../grammar/expr.h"
#include "../val.h"

static int parse_for_loop(flamingo_t* flamingo, TSNode node) {
	// Get current variable name.

	TSNode const cur_var_name_node = ts_node_child_by_field_name(node, "cur_var_name", 12);
	char const* const cur_var_name_type = ts_node_type(cur_var_name_node);

	if (strcmp(cur_var_name_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for current variable name, got %s", cur_var_name_type);
	}

	// Get iterator.

	TSNode const iterator_node = ts_node_child_by_field_name(node, "iterator", 8);
	char const* const iterator_type = ts_node_type(iterator_node);

	if (strcmp(iterator_type, "expression") != 0) {
		return error(flamingo, "expected expression for iterator, got %s", iterator_type);
	}

	// Get for body.

	TSNode const body_node = ts_node_child_by_field_name(node, "body", 4);
	char const* const body_type = ts_node_type(body_node);

	if (strcmp(body_type, "block") != 0) {
		return error(flamingo, "expected block for body, got %s", body_type);
	}

	// Evaluate iterator.
	// TODO We might want a mechanism for properly iterating over an iterable, because here we have to evaluate the entire value, which is very inefficient for a range function e.g. (see range vs xrange in Python 2)/

	// TODO Evaluate iterator and run for loop.
	//      I'm thinking that the flamingo object should probably store the closest loop in each stack frame for continues/breaks.
	//      Let's shy away from the temptation of using this infrastructure to implement gotos, at least for now when I'm not yet entirely sure if I want this language to be Turing-complete or not.

	return 0;
}
