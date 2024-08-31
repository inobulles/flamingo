// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>
#include <scope.c>
#include <val.c>

static int parse_function_declaration(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 5 || ts_node_child_count(node) == 6);

	// Get qualifier list.

	TSNode const qualifiers_node = ts_node_child_by_field_name(node, "qualifiers", 10);
	bool const has_qualifiers = !ts_node_is_null(qualifiers_node);

	if (has_qualifiers) {
		char const* const qualifiers_type = ts_node_type(qualifiers_node);

		if (strcmp(qualifiers_type, "qualifier_list") != 0) {
			return error(flamingo, "expected qualifier_list for qualifiers, got %s", qualifiers_type);
		}
	}

	// Get function name.

	TSNode const name_node = ts_node_child_by_field_name(node, "name", 4);
	char const* const name_type = ts_node_type(name_node);

	if (strcmp(name_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for function name, got %s", name_type);
	}

	size_t const start = ts_node_start_byte(name_node);
	size_t const end = ts_node_end_byte(name_node);

	char const* const name = flamingo->src + start;
	size_t const size = end - start;

	// Get function parameters.

	TSNode const params = ts_node_child_by_field_name(node, "params", 6);
	bool const has_params = !ts_node_is_null(params);

	if (has_params) {
		char const* const params_type = ts_node_type(params);

		if (strcmp(params_type, "param_list") != 0) {
			return error(flamingo, "expected param_list for parameters, got %s", params_type);
		}
	}

	// Get function body.

	TSNode const body = ts_node_child_by_field_name(node, "body", 4);
	char const* const body_type = ts_node_type(body);

	if (strcmp(body_type, "statement") != 0) {
		return error(flamingo, "expected statement for body, got %s", body_type);
	}

	// Check parameter types.

	if (has_params) {
		size_t const n = ts_node_child_count(params);

		for (size_t i = 0; i < n; i++) {
			TSNode const child = ts_node_named_child(params, i);
			char const* const child_type = ts_node_type(child);

			if (strcmp(child_type, "param") != 0) {
				return error(flamingo, "expected param in parameter list, got %s", child_type);
			}
		}
	}

	// Check if identifier is already in scope (or a previous one) and error if it is.
	// Right now, redeclaring functions is not allowed.
	// Although this will probably work a bit differently once function prototypes are added.

	flamingo_var_t* const prev_var = flamingo_scope_find_var(flamingo, name, size);

	if (prev_var != NULL) {
		return error(flamingo, "the %s '%.*s' has already been declared in this scope", val_kind_str(prev_var->val), (int) size, name);
	}

	// Add function to scope.

	flamingo_var_t* const var = scope_add_var(cur_scope(flamingo), name, size);

	var->val = val_alloc();
	var->val->kind = FLAMINGO_VAL_KIND_FN;

	// Assign body node.
	// Since I want 'flamingo.h' to be usable without importing all of Tree-sitter, 'var->val->fn.body' can't just be a 'TSNode'.
	// Thus, since only this file knows about the size of 'TSNode', we must dynamically allocate this on the heap.

	var->val->fn.body = malloc(sizeof body);
	memcpy(var->val->fn.body, &body, sizeof body);

	var->val->fn.params = NULL;

	if (has_params) {
		var->val->fn.params = malloc(sizeof params);
		memcpy(var->val->fn.params, &params, sizeof params);
	}

	var->val->fn.src = flamingo->src;
	var->val->fn.src_size = flamingo->src_size;

	return 0;
}
