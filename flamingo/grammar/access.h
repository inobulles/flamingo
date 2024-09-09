// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"

#include <common.h>
#include <scope.c>

static int access_find_var(flamingo_t* flamingo, TSNode node, flamingo_var_t** var, flamingo_val_t** accessed_val) {
	assert(var != NULL);
	assert(accessed_val != NULL);
	assert(strcmp(ts_node_type(node), "access") == 0);
	assert(ts_node_named_child_count(node) == 2);

	// Get accessed expression.

	TSNode const accessed = ts_node_child_by_field_name(node, "accessed", 8);
	char const* const accessed_type = ts_node_type(accessed);

	if (strcmp(accessed_type, "expression") != 0) {
		return error(flamingo, "expected expression for accessed, got %s", accessed_type);
	}

	// Get accessor identifier.

	TSNode const accessor_node = ts_node_child_by_field_name(node, "accessor", 8);
	char const* const accessor_type = ts_node_type(accessor_node);

	if (strcmp(accessor_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for accessor, got %s", accessor_type);
	}

	size_t const start = ts_node_start_byte(accessor_node);
	size_t const end = ts_node_end_byte(accessor_node);

	char const* const accessor = flamingo->src + start;
	size_t const size = end - start;

	// Parse accessed expression.

	if (parse_expr(flamingo, accessed, accessed_val) != 0) {
		return -1;
	}

	// Check if accessed value is accessible.
	// XXX For now this is only instances, but in the future I'm going to want to be able to access static members on classes directly too.

	if ((*accessed_val)->kind != FLAMINGO_VAL_KIND_INST) {
		return error(flamingo, "accessed expression is not accessible (must be instance, is %s)", val_type_str(*accessed_val));
	}

	// Actually access.

	flamingo_scope_t* const scope = (*accessed_val)->inst.scope;
	*var = scope_shallow_find_var(scope, accessor, size);

	if (*var == NULL) {
		return error(flamingo, "member '%.*s' was never in declared", (int) size, accessor);
	}

	return 0;
}

static int parse_access(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	flamingo_var_t* var;
	flamingo_val_t* accessed_val;

	if (access_find_var(flamingo, node, &var, &accessed_val) < 0) {
		return -1;
	}

	// Set value.

	if (val == NULL) {
		return 0;
	}

	assert(*val == NULL);
	*val = var->val;
	val_incref(*val);

	val_decref(accessed_val);
	return 0;
}
