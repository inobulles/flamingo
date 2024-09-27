// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>
#include <scope.c>
#include <val.c>
#include <env.c>

static int parse_function_declaration(flamingo_t* flamingo, TSNode node, flamingo_fn_kind_t kind) {
	size_t const child_count = ts_node_child_count(node);
	assert(child_count >= 4 || child_count <= 6);

	char const* thing = "unknown";

	switch (kind) {
	case FLAMINGO_FN_KIND_FUNCTION:
		thing = "function";
		break;
	case FLAMINGO_FN_KIND_CLASS:
		thing = "class";
		break;
	case FLAMINGO_FN_KIND_EXTERN:
		thing = "external prototype";
		break;
	default:
		assert(false);
	}

	// Get qualifier list.

	TSNode const qualifiers_node = ts_node_child_by_field_name(node, "qualifiers", 10);
	bool const has_qualifiers = !ts_node_is_null(qualifiers_node);

	if (has_qualifiers) {
		char const* const qualifiers_type = ts_node_type(qualifiers_node);

		if (strcmp(qualifiers_type, "qualifier_list") != 0) {
			return error(flamingo, "expected qualifier_list for qualifiers, got %s", qualifiers_type);
		}
	}

	// Get function/class name.

	TSNode const name_node = ts_node_child_by_field_name(node, "name", 4);
	char const* const name_type = ts_node_type(name_node);

	if (strcmp(name_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for %s name, got %s", thing, name_type);
	}

	size_t const start = ts_node_start_byte(name_node);
	size_t const end = ts_node_end_byte(name_node);

	char const* const name = flamingo->src + start;
	size_t const size = end - start;

	// Get function/class parameters.

	TSNode const params = ts_node_child_by_field_name(node, "params", 6);
	bool const has_params = !ts_node_is_null(params);

	if (has_params) {
		char const* const params_type = ts_node_type(params);

		if (strcmp(params_type, "param_list") != 0) {
			return error(flamingo, "expected param_list for parameters, got %s", params_type);
		}
	}

	// Get function/class body (only for non-prototypes).

	TSNode body;

	if (kind != FLAMINGO_FN_KIND_EXTERN) {
		body = ts_node_child_by_field_name(node, "body", 4);
		char const* const body_type = ts_node_type(body);

		if (strcmp(body_type, "block") != 0) {
			return error(flamingo, "expected block for body, got %s", body_type);
		}
	}

	// Check parameter types.

	if (has_params) {
		size_t const n = ts_node_named_child_count(params);

		for (size_t i = 0; i < n; i++) {
			TSNode const child = ts_node_named_child(params, i);
			char const* const child_type = ts_node_type(child);

			if (strcmp(child_type, "param") != 0) {
				return error(flamingo, "expected param in parameter list, got %s", child_type);
			}
		}
	}

	// Check if identifier is already in current scope (shallow search) and error if it is.
	// Redeclaring functions/classes is not allowed (I have decided against prototypes).
	// If its in a previous one, that's alright, we'll just shadow it.

	flamingo_scope_t* const cur_scope = env_cur_scope(flamingo->env);
	flamingo_var_t* const prev_var = scope_shallow_find_var(cur_scope, name, size);

	if (prev_var != NULL) {
		return error(flamingo, "the %s '%.*s' has already been declared in this scope", val_role_str(prev_var->val), (int) size, name);
	}

	// Add function/class to scope.

	flamingo_var_t* const var = scope_add_var(cur_scope, name, size);

	var_set_val(var, val_alloc());
	var->val->kind = FLAMINGO_VAL_KIND_FN;
	var->val->fn.kind = kind;

	var->val->fn.params = NULL;

	if (has_params) {
		var->val->fn.params = malloc(sizeof params);
		memcpy(var->val->fn.params, &params, sizeof params);
	}

	// Assign body node.
	// Prototypes by definition don't have bodies.

	if (kind != FLAMINGO_FN_KIND_EXTERN) {
		var->val->fn.body = malloc(sizeof body);
		memcpy(var->val->fn.body, &body, sizeof body);
	}

	// Since I want 'flamingo.h' to be usable without importing all of Tree-sitter, 'var->val->fn.body' can't just be a 'TSNode'.
	// Thus, since only this file knows about the size of 'TSNode', we must dynamically allocate this on the heap.

	var->val->fn.src = flamingo->src;
	var->val->fn.src_size = flamingo->src_size;

	return 0;
}
