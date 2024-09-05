// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "flamingo.h"

#include <assert.h>
#include <stdlib.h>

static flamingo_val_t* val_incref(flamingo_val_t* val) {
	assert(val->ref_count > 0); // value has already been freed
	val->ref_count++;

	return val;
}

static flamingo_val_t* val_init(flamingo_val_t* val) {
	val->name = NULL;
	val->name_size = 0;

	val->kind = FLAMINGO_VAL_KIND_NONE;
	val->ref_count = 1;

	return val;
}

static char const* val_role_str(flamingo_val_t* val) {
	switch (val->kind) {
	case FLAMINGO_VAL_KIND_STR:
	case FLAMINGO_VAL_KIND_INT:
	case FLAMINGO_VAL_KIND_BOOL:
	case FLAMINGO_VAL_KIND_NONE:
	case FLAMINGO_VAL_KIND_INST:
		return "variable";
	case FLAMINGO_VAL_KIND_FN:
		return val->fn.is_class ? "class" : "function";
	default:
		return "unknown";
	}
}

static char const* val_type_str(flamingo_val_t* val) {
	switch (val->kind) {
	case FLAMINGO_VAL_KIND_BOOL:
		return "boolean";
	case FLAMINGO_VAL_KIND_INT:
		return "integer";
	case FLAMINGO_VAL_KIND_STR:
		return "string";
	case FLAMINGO_VAL_KIND_FN:
		return val->fn.is_class ? "class" : "function";
	case FLAMINGO_VAL_KIND_NONE:
		return "none";
	case FLAMINGO_VAL_KIND_INST:
		return "instance";
	default:
		return "unknown";
	}
}

static flamingo_val_t* val_alloc(void) {
	flamingo_val_t* const val = calloc(1, sizeof *val);
	return val_init(val);
}

static void scope_free(flamingo_scope_t* scope);

static void val_free(flamingo_val_t* val) {
	if (val->kind == FLAMINGO_VAL_KIND_STR) {
		free(val->str.str);
	}

	if (val->kind == FLAMINGO_VAL_KIND_FN) {
		free(val->fn.body);
	}

	if (val->kind == FLAMINGO_VAL_KIND_INST) {
		scope_free(val->inst.scope);

		if (val->inst.free_data != NULL) {
			val->inst.free_data(val, val->inst.data);
		}
	}

	// TODO when should the memory pointed to by val itself be freed?
}

static flamingo_val_t* val_decref(flamingo_val_t* val) {
	val->ref_count--;

	if (val->ref_count > 0) {
		return val;
	}

	// if value is not referred to by anyone, free it

	val_free(val);
	return NULL;
}
