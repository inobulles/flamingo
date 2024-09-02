// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"
#include "statement.h"
#include <scope.c>

#include <common.h>

static int setup_args(flamingo_t* flamingo, TSNode args, TSNode* params) {
	size_t const n = ts_node_named_child_count(args);
	size_t const param_count = params == NULL ? 0 : ts_node_named_child_count(*params);

	if (n != param_count) {
		return error(flamingo, "callable expected %zu arguments, got %zu instead", param_count, n);
	}

	for (size_t i = 0; i < n; i++) {
		// Get argument.

		TSNode const arg = ts_node_named_child(args, i);
		char const* const arg_type = ts_node_type(arg);

		if (strcmp(arg_type, "expression") != 0) {
			return error(flamingo, "expected expression in argument list, got %s", arg_type);
		}

		// Get parameter in same position.
		// assert: Type should already have been checked when declaring the function.

		TSNode const param = ts_node_named_child(*params, i);
		char const* const param_type = ts_node_type(param);
		assert(strcmp(param_type, "param") == 0);

		// Get parameter identifier.

		TSNode const identifier = ts_node_child_by_field_name(param, "ident", 5);
		assert(strcmp(ts_node_type(identifier), "identifier") == 0);

		size_t const start = ts_node_start_byte(identifier);
		size_t const end = ts_node_end_byte(identifier);

		char const* const name = flamingo->src + start;
		size_t const size = end - start;

		// Get parameter type if it has one.

		TSNode const type = ts_node_child_by_field_name(param, "type", 4);
		bool has_type = !ts_node_is_null(args);

		if (has_type) {
			assert(strcmp(ts_node_type(type), "type") == 0);
		}

		(void) type;

		// Create parameter variable.

		flamingo_var_t* const var = scope_add_var(cur_scope(flamingo), name, size);

		// Parse argument expression into that parameter variable.

		if (parse_expr(flamingo, arg, &var->val) < 0) {
			return -1;
		}
	}

	return 0;
}

static int parse_call(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "call") == 0);
	assert(ts_node_child_count(node) == 3 || ts_node_child_count(node) == 4);

	// Get callable expression.
	// TODO Evaluate this motherfucker.

	TSNode const callable_node = ts_node_child_by_field_name(node, "callable", 8);
	char const* const callable_type = ts_node_type(callable_node);

	if (strcmp(callable_type, "expression") != 0) {
		return error(flamingo, "expected identifier for function name, got %s", callable_type);
	}

	// Get arguments.

	TSNode const args = ts_node_child_by_field_name(node, "args", 4);
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

	// Switch context's source if the function was created in another.

	char* const prev_src = flamingo->src;
	size_t const prev_src_size = flamingo->src_size;

	flamingo->src = callable->fn.src;
	flamingo->src_size = callable->fn.src_size;

	// Create a new scope for the function for the argument assignments.

	scope_stack_push(flamingo);

	// Evaluate argument expressions.
	// Add our arguments as variables, with the function parameters as names.

	TSNode* const params = callable->fn.params;

	if (has_args) {
		if (setup_args(flamingo, args, params) < 0) {
			return -1;
		}
	}

	// Actually parse the function's body.

	TSNode* const body = callable->fn.body;
	int const rv = parse_statement(flamingo, *body);

	// Unwind the scope stack and switch back to previous source context.

	scope_pop(flamingo);

	flamingo->src = prev_src;
	flamingo->src_size = prev_src_size;

	// Set the value of this expression to a none value.
	// XXX Should obviously not do this if return statement in function.
	// Don't need to do anything if we're not going to assign it to a value.

	if (val != NULL) {
		assert(*val == NULL);
		*val = val_alloc();
	}

	return rv;
}
