// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>
#include <scope.c>
#include <val.c>

static int parse_class_declaration(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 4);

	// Get qualifier list.

	TSNode const qualifiers_node = ts_node_child_by_field_name(node, "qualifiers", 10);
	bool const has_qualifiers = !ts_node_is_null(qualifiers_node);

	if (has_qualifiers) {
		char const* const qualifiers_type = ts_node_type(qualifiers_node);

		if (strcmp(qualifiers_type, "qualifier_list") != 0) {
			return error(flamingo, "expected qualifier_list for qualifiers, got %s", qualifiers_type);
		}
	}

	// Get class name.

	TSNode const name_node = ts_node_child_by_field_name(node, "name", 4);
	char const* const name_type = ts_node_type(name_node);

	if (strcmp(name_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for function name, got %s", name_type);
	}

	size_t const start = ts_node_start_byte(name_node);
	size_t const end = ts_node_end_byte(name_node);

	char const* const name = flamingo->src + start;
	size_t const size = end - start;

	// Get class body.

	TSNode const body = ts_node_child_by_field_name(node, "body", 4);
	char const* const body_type = ts_node_type(body);

	if (strcmp(body_type, "statement") != 0) {
		return error(flamingo, "expected statement for body, got %s", body_type);
	}

	// Check if identifier is already in scope (or a previous one) and error if it is.
	// Right now, redeclaring classes is not allowed.
	// Although this will probably work a bit differently once class prototypes are added.

	flamingo_var_t* const prev_var = flamingo_scope_find_var(flamingo, name, size);

	if (prev_var != NULL) {
		return error(flamingo, "the %s '%.*s' has already been declared in this scope", val_kind_str(prev_var->val), (int) size, name);
	}

	// Add class to scope.

	flamingo_var_t* const var = scope_add_var(cur_scope(flamingo), name, size);

	var->val = val_alloc();
	var->val->kind = FLAMINGO_VAL_KIND_CLASS;

	// Assign body node.
	// See comment in function declaration.

	var->val->class.body = malloc(sizeof body);
	memcpy(var->val->class.body, &body, sizeof body);

	var->val->class.src = flamingo->src;
	var->val->class.src_size = flamingo->src_size;

	return 0;
}
