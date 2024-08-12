// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "common.h"
#include "val.c"

static int parse_literal(flamingo_t* flamingo, TSNode node, flamingo_val_t** val) {
	assert(strcmp(ts_node_type(node), "literal") == 0);
	assert(ts_node_child_count(node) == 1);

	// if value already exists, free it
	// XXX not sure when this is the case, just doing this for now to be safe!

	if (*val != NULL) {
		val_free(*val);
		val_init(*val);
	}

	// otherwise, allocate it

	else {
		*val = val_alloc();
	}

	// in any case, val should not be NULL from this point forth

	assert(*val != NULL);

	TSNode const child = ts_node_child(node, 0);
	char const* const type = ts_node_type(child);

	if (strcmp(type, "string") == 0) {
		(*val)->kind = FLAMINGO_VAL_KIND_STR;

		size_t const start = ts_node_start_byte(child);
		size_t const end = ts_node_end_byte(child);

		// XXX remove one from each side as we don't want the quotes

		(*val)->str.size = end - start - 2;
		(*val)->str.str = malloc((*val)->str.size);

		assert((*val)->str.str != NULL);
		memcpy((*val)->str.str, flamingo->src + start + 1, (*val)->str.size);

		return 0;
	}

	if (strcmp(type, "number") == 0) {
		return error(flamingo, "number literals are not yet supported");
	}

	return error(flamingo, "unknown literal type: %s", type);
}
