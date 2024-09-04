// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"

#include <common.h>
#include <val.c>

static int parse_print(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 2);

	TSNode const msg_node = ts_node_child_by_field_name(node, "msg", 3);
	char const* const type = ts_node_type(msg_node);

	if (strcmp(ts_node_type(msg_node), "expression") != 0) {
		return error(flamingo, "expected expression for message, got %s", type);
	}

	flamingo_val_t* val = NULL;

	if (parse_expr(flamingo, msg_node, &val) < 0) {
		return -1;
	}

	// XXX Don't forget to decrement reference at the end!

	int rv = 0;

	switch (val->kind) {
	case FLAMINGO_VAL_KIND_NONE:
		printf("<none>\n");
		break;
	case FLAMINGO_VAL_KIND_BOOL:
		printf("%s\n", val->boolean.val ? "true" : "false");
		break;
	case FLAMINGO_VAL_KIND_INT:
		printf("%ld\n", val->integer.integer);
		break;
	case FLAMINGO_VAL_KIND_STR:
		printf("%.*s\n", (int) val->str.size, val->str.str);
		break;
	case FLAMINGO_VAL_KIND_FN:
		// TODO Find a way to print the val's key (i.e. function/class name) too?
		printf("<%s>\n", val->fn.is_class ? "class" : "fn");
		break;
	default:
		rv = error(flamingo, "can't print expression kind: %d", val->kind);
	}

	val_decref(val);
	return rv;
}
