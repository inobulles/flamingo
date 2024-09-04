// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "call.h"
#include "identifier.h"
#include "literal.h"

#include <common.h>
#include <math.h>

// TODO Move to own file.

static int parse_binary_expr(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "binary_expression") == 0);
	assert(ts_node_named_child_count(node) == 3);

	// Get left operand.

	TSNode const left = ts_node_child_by_field_name(node, "left", 4);
	char const* const left_type = ts_node_type(left);

	if (strcmp(left_type, "expression") != 0) {
		return error(flamingo, "expected expression for left operand, got %s", left_type);
	}

	// Get right operand.

	TSNode const right = ts_node_child_by_field_name(node, "right", 5);
	char const* const right_type = ts_node_type(right);

	if (strcmp(right_type, "expression") != 0) {
		return error(flamingo, "expected expression for right operand, got %s", right_type);
	}

	// Get operator.
	// XXX Calling this all 'op_*' because clang-format thinks 'operator' is the C++ keyword and so is annoying with it.

	TSNode const op_node = ts_node_child_by_field_name(node, "operator", 8);
	char const* const op_type = ts_node_type(op_node);

	if (strstr("operator", op_type) != 0) { // XXX Yes, I'm being lazy.
		return error(flamingo, "expected operator, got %s", op_type);
	}

	size_t const start = ts_node_start_byte(op_node);
	size_t const end = ts_node_end_byte(op_node);

	char const* const op = flamingo->src + start;
	size_t const op_size = end - start;

	// Parse operands.

	flamingo_val_t* left_val = NULL;

	if (parse_expr(flamingo, left, &left_val) != 0) {
		return -1;
	}

	flamingo_val_t* right_val = NULL;

	if (parse_expr(flamingo, right, &right_val) != 0) {
		return -1;
	}

	// Check if the operands are compatible.

	if (left_val->kind != right_val->kind) {
		return error(flamingo, "operands have incompatible types: %s and %s", val_type_str(left_val), val_type_str(right_val));
	}

	flamingo_val_kind_t const kind = left_val->kind; // Same as 'right_val->kind' by this point.

	// Allocate our value.
	// We can stop here if we discard the result, as we've already evaluated our operand expressions (which we need to do as they might modify state).
	// TODO It would be nice to find a way to check the operands are compatible with the operator before this point so we get errors. Though I don't think this is the only place where we're not doing this correctly so this might be a difficult change to make.

	if (val == NULL) {
		return 0;
	}

	assert(*val == NULL);
	*val = val_alloc();

	// Do the math.

	int rv = 0;

	if (kind == FLAMINGO_VAL_KIND_INT) {
		// Integer arithmetic.

		if (strncmp(op, "+", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_INT;
			(*val)->integer.integer = left_val->integer.integer + right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "-", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_INT;
			(*val)->integer.integer = left_val->integer.integer - right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "*", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_INT;
			(*val)->integer.integer = left_val->integer.integer * right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "/", op_size) == 0) {
			if (right_val->integer.integer == 0) {
				return error(flamingo, "division by zero");
			}

			(*val)->kind = FLAMINGO_VAL_KIND_INT;
			(*val)->integer.integer = left_val->integer.integer / right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "%", op_size) == 0) {
			if (right_val->integer.integer == 0) {
				return error(flamingo, "modulo by zero");
			}

			(*val)->kind = FLAMINGO_VAL_KIND_INT;
			(*val)->integer.integer = left_val->integer.integer % right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "**", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_INT;
			(*val)->integer.integer = pow(left_val->integer.integer, right_val->integer.integer);
			goto done;
		}

		// Comparisons.

		if (strncmp(op, "==", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->integer.integer == right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "!=", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->integer.integer != right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "<", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->integer.integer < right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, "<=", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->integer.integer <= right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, ">", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->integer.integer > right_val->integer.integer;
			goto done;
		}

		if (strncmp(op, ">=", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->integer.integer >= right_val->integer.integer;
			goto done;
		}
	}

	if (kind == FLAMINGO_VAL_KIND_BOOL) {
		// Logical operators.

		if (strncmp(op, "&&", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->boolean.val && right_val->boolean.val;
			goto done;
		}

		if (strncmp(op, "||", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->boolean.val || right_val->boolean.val;
			goto done;
		}

		if (strncmp(op, "^^", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = (!!left_val->boolean.val) ^ (!!right_val->boolean.val);
			goto done;
		}

		// Comparisons.

		if (strncmp(op, "==", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->boolean.val == right_val->boolean.val;
			goto done;
		}

		if (strncmp(op, "!=", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->boolean.val != right_val->boolean.val;
			goto done;
		}
	}

	if (kind == FLAMINGO_VAL_KIND_STR) {
		// String concatenation.
		// TODO Multiplication (but then I need to rethink the whole operands having to have the same type thing).

		if (strncmp(op, "+", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_STR;

			(*val)->str.size = left_val->str.size + right_val->str.size;
			(*val)->str.str = malloc((*val)->str.size);
			assert((*val)->str.str != NULL);

			memcpy((*val)->str.str, left_val->str.str, left_val->str.size);
			memcpy((*val)->str.str + left_val->str.size, right_val->str.str, right_val->str.size);

			goto done;
		}

		// Comparisons.

		if (strncmp(op, "==", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->str.size == right_val->str.size && memcmp(left_val->str.str, right_val->str.str, left_val->str.size) == 0;
			goto done;
		}

		if (strncmp(op, "!=", op_size) == 0) {
			(*val)->kind = FLAMINGO_VAL_KIND_BOOL;
			(*val)->boolean.val = left_val->str.size != right_val->str.size || memcmp(left_val->str.str, right_val->str.str, left_val->str.size) != 0;
			goto done;
		}
	}

	rv = error(flamingo, "unknown operator '%.*s' for type %s", (int) op_size, op, val_type_str(left_val));

done:

	val_decref(left_val);
	val_decref(right_val);

	return rv;
}

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

	// These expressions could have side-effects, so we need to parse them anyway.

	if (strcmp(type, "call") == 0) {
		return parse_call(flamingo, child, val);
	}

	if (strcmp(type, "parenthesized_expression") == 0) {
		TSNode const grandchild = ts_node_child_by_field_name(child, "expression", 10);
		return parse_expr(flamingo, grandchild, val);
	}

	if (strcmp(type, "binary_expression") == 0) {
		return parse_binary_expr(flamingo, child, val);
	}

	return error(flamingo, "unknown expression type: %s", type);
}
