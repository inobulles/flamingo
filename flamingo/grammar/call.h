// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "block.h"
#include "expr.h"
#include <scope.h>

#include <common.h>

// XXX Right now, there's no way of defining primitive type member parameters.
// So I'm just ignoring them for now.
// We need to find a long-term solution at some point.

static int setup_args_no_param(flamingo_t* flamingo, TSNode args) {
	size_t const n = ts_node_named_child_count(args);

	for (size_t i = 0; i < n; i++) {
		// Get argument.

		TSNode const arg = ts_node_named_child(args, i);
		char const* const arg_type = ts_node_type(arg);

		if (strcmp(arg_type, "expression") != 0) {
			return error(flamingo, "expected expression in argument list, got %s", arg_type);
		}

		// Create parameter variable.

		flamingo_scope_t* const scope = env_cur_scope(flamingo->env);
		flamingo_var_t* const var = scope_add_var(scope, "nothing to see here!", 0);

		// Parse argument expression into that parameter variable.

		if (parse_expr(flamingo, arg, &var->val, NULL) < 0) {
			return -1;
		}
	}

	return 0;
}

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
		// assert: Type should already have been checked when declaring the function/class.

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
		bool const has_type = !ts_node_is_null(type);

		if (has_type) {
			assert(strcmp(ts_node_type(type), "type") == 0);
		}

		(void) type;

		// Create parameter variable.

		flamingo_scope_t* const scope = env_cur_scope(flamingo->env);
		flamingo_var_t* const var = scope_add_var(scope, name, size);

		// Parse argument expression into that parameter variable.

		if (parse_expr(flamingo, arg, &var->val, NULL) < 0) {
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
		return error(flamingo, "expected identifier for callable name, got %s", callable_type);
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
	flamingo_val_t* accessed_val = NULL;

	if (parse_expr(flamingo, callable_node, &callable, &accessed_val) < 0) {
		return -1;
	}

	if (callable->kind != FLAMINGO_VAL_KIND_FN) {
		return error(flamingo, "callable expression is of type %s, which is not callable", val_type_str(callable));
	}

	bool const is_class = callable->fn.kind == FLAMINGO_FN_KIND_CLASS;
	bool const on_inst = accessed_val != NULL && accessed_val->kind == FLAMINGO_VAL_KIND_INST;
	bool const is_extern = callable->fn.kind == FLAMINGO_FN_KIND_EXTERN;
	bool const is_ptm = callable->fn.kind == FLAMINGO_FN_KIND_PTM;

	// Actually call the callable.
	// Switch context's source if the callable was created in another.

	char* const prev_src = flamingo->src;
	size_t const prev_src_size = flamingo->src_size;

	if (callable->fn.src != NULL) {
		flamingo->src = callable->fn.src;
		flamingo->src_size = callable->fn.src_size;
	}

	// Switch context's current callable body if we were called from another.

	TSNode* const prev_fn_body = flamingo->cur_fn_body;
	flamingo->cur_fn_body = callable->fn.body;

	// Switch context's current environment to the one closed over by the function.

	flamingo_env_t* const prev_env = flamingo->env;
	flamingo->env = callable->fn.env == NULL ? prev_env : callable->fn.env;

	// If calling on an instance, add that instance's scope to the scope stack.
	// We do this before the arguments scope because we want parameters to shadow stuff in the instance's scope.

	if (on_inst) {
		env_gently_attach_scope(flamingo->env, accessed_val->inst.scope);
	}

	// Create a new scope for the function for the argument assignments.
	// It's important to set 'scope->class_scope' to false for functions as new scopes will copy the 'class_scope' property from their parents otherwise.

	flamingo_scope_t* scope = env_push_scope(flamingo->env);
	scope->class_scope = is_class;

	// Evaluate argument expressions.
	// Add our arguments as variables, with the function parameters as names.

	TSNode* const params = callable->fn.params;

	if (has_args) {
		if (is_ptm) {
			if (setup_args_no_param(flamingo, args) < 0) {
				return -1;
			}
		}

		else if (setup_args(flamingo, args, params) < 0) {
			return -1;
		}
	}

	// If external function or primitive type member: call the function's callback.
	// If function or class: actually parse the function's body.

	TSNode* const body = callable->fn.body;
	bool const is_expr = strcmp(ts_node_type(*body), "expression") == 0;

	flamingo_scope_t* inner_scope;

	if (is_extern || is_ptm) {
		if (is_extern && flamingo->external_fn_cb == NULL) {
			return error(flamingo, "cannot call external function without a external function callback being set");
		}

		// Create arg list.

		flamingo_scope_t* const arg_scope = env_cur_scope(flamingo->env);

		size_t const arg_count = arg_scope->vars_size;
		flamingo_val_t** const args = malloc(arg_count * sizeof *args);
		assert(args != NULL);

		for (size_t i = 0; i < arg_count; i++) {
			args[i] = arg_scope->vars[i].val;
		}

		flamingo_arg_list_t arg_list = {
			.count = arg_count,
			.args = args,
		};

		// Actually call the external function callback or primitive type member.

		assert(flamingo->cur_fn_rv == NULL);

		if (is_extern && flamingo->external_fn_cb(flamingo, callable->name_size, callable->name, flamingo->external_fn_cb_data, &arg_list, &flamingo->cur_fn_rv) < 0) {
			return -1;
		}

		else if (is_ptm && callable->fn.ptm_cb(flamingo, accessed_val, &arg_list, &flamingo->cur_fn_rv)) {
			return -1;
		}

		free(args);
	}

	else if (is_expr) {
		assert(callable->fn.kind == FLAMINGO_FN_KIND_FUNCTION); // The only kind of callable that can have an expression body.

		if (parse_expr(flamingo, *body, val, NULL) < 0) {
			return -1;
		}
	}

	else if (parse_block(flamingo, *body, is_class ? &inner_scope : NULL) < 0) {
		return -1;
	}

	// Unwind the scope stack and switch back to previous source, current function body context, and environment..

	env_pop_scope(flamingo->env);

	if (on_inst) {
		env_gently_detach_scope(flamingo->env);
	}

	flamingo->src = prev_src;
	flamingo->src_size = prev_src_size;

	flamingo->cur_fn_body = prev_fn_body;
	flamingo->env = prev_env;

	// If the function body was an expression, we're done (no return value, call value was already set).

	if (is_expr) {
		goto done;
	}

	// If class, create an instance.

	if (callable->fn.kind == FLAMINGO_FN_KIND_CLASS) {
		if (val == NULL) {
			goto done;
		}

		assert(*val == NULL);
		*val = val_alloc();
		(*val)->kind = FLAMINGO_VAL_KIND_INST;

		(*val)->inst.class = callable;
		(*val)->inst.scope = inner_scope;
		(*val)->inst.data = NULL;
		(*val)->inst.free_data = NULL;

		goto done;
	}

	// Set the value of this expression to the return value.
	// Just discard it if we're not going to set it to anything.

	if (val == NULL) {
		if (flamingo->cur_fn_rv != NULL) {
			val_decref(flamingo->cur_fn_rv);
		}
	}

	else {
		// If return value was never set, just create a none value.

		if (flamingo->cur_fn_rv == NULL) {
			flamingo->cur_fn_rv = val_alloc();
		}

		assert(*val == NULL);
		*val = flamingo->cur_fn_rv;
	}

done:

	flamingo->cur_fn_rv = NULL;
	return 0;
}
