// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "expr.h"

#include <common.h>
#include <val.h>

#include <inttypes.h>

static int parse_print(flamingo_t* flamingo, TSNode node) {
	assert(ts_node_child_count(node) == 2);

	TSNode const msg_node = ts_node_child_by_field_name(node, "msg", 3);
	char const* const type = ts_node_type(msg_node);

	if (strcmp(ts_node_type(msg_node), "expression") != 0) {
		return error(flamingo, "expected expression for message, got %s", type);
	}

	flamingo_val_t* val = NULL;

	if (parse_expr(flamingo, msg_node, &val, NULL) < 0) {
		return -1;
	}

	// XXX Don't forget to decrement reference at the end!

	int rv = 0;
	flamingo_val_t* class;

	switch (val->kind) {
	case FLAMINGO_VAL_KIND_NONE:
		printf("<none>\n");
		break;
	case FLAMINGO_VAL_KIND_BOOL:
		printf("%s\n", val->boolean.boolean ? "true" : "false");
		break;
	case FLAMINGO_VAL_KIND_INT:
		printf("%" PRId64 "\n", val->integer.integer);
		break;
	case FLAMINGO_VAL_KIND_STR:
		printf("%.*s\n", (int) val->str.size, val->str.str);
		break;
	case FLAMINGO_VAL_KIND_FN:
		printf("<%s %.*s>\n", val_type_str(val), (int) val->name_size, val->name);
		break;
	case FLAMINGO_VAL_KIND_INST:
		class = val->inst.class;
		assert(class != NULL);

		printf("<instance of %.*s>\n", (int) class->name_size, class->name);
		break;
	default:
		rv = error(flamingo, "can't print expression kind: %d", val->kind);
	}

	val_decref(val);
	return rv;
}
