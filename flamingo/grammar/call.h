// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"
#include "statement.h"

#include <common.h>

static int parse_call(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "call") == 0);
	assert(ts_node_child_count(node) == 3);

	// Get callable expression.
	// TODO Evaluate this motherfucker.

	TSNode const callable_node = ts_node_child_by_field_name(node, "callable", 8);
	char const* const callable_type = ts_node_type(callable_node);

	if (strcmp(callable_type, "expression") != 0) {
		return error(flamingo, "expected identifier for function name, got %s", callable_type);
	}

	// Get arguments.
	// TODO Do something with these arguments.

	TSNode const args = ts_node_child_by_field_name(node, "args", 6);
	bool const has_args = !ts_node_is_null(args);

	if (has_args) {
		char const* const args_type = ts_node_type(args);

		if (strcmp(args_type, "arg_list") != 0) {
			return error(flamingo, "expected arg_list for parameters, got %s", args_type);
		}
	}

	// Evaluate callable expression.

	flamingo_val_t* callable = NULL;

	if (parse_expr(flamingo, callable_node, &callable) < 0) {
		return -1;
	}

	if (callable->kind != FLAMINGO_VAL_KIND_FN) {
		return error(flamingo, "callable has a value kind of %d, which is not callable", callable->kind);
	}

	// Actually call the callable.

	/* TODO
		The issue with this is that the body we're calling may further down the scope stack, and blocks just blindly push a new scope on top of the stack.
	   What we should really be doing is copying the scope stack, rolling it back to the scope where the function was declared, and then pushing a new scope on top of that.
	   Maybe there's a better way? Here's an illustration of the problem:

		fn fun2() {
			print(a) # I should not have access to 'a' in here.
		}

		fn fun1() {
			fn fun3() {
				print(a) # In fact, and this is a semi-unrelated issue, I should have access to 'a' here either.
			}

			a = "hello"
			fun2()
			fun3()
		}
	*/

	TSNode* const body = callable->fn.body;
	return parse_statement(flamingo, *body);
}
