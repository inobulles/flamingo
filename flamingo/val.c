// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "flamingo.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static flamingo_val_t* val_incref(flamingo_val_t* val) {
	assert(val->ref_count > 0); // value has already been freed
	val->ref_count++;

	return val;
}

static flamingo_val_t* val_init(flamingo_val_t* val) {
	// By default, values are anonymous.

	val->name = NULL;
	val->name_size = 0;

	// By default, values are nones.

	val->kind = FLAMINGO_VAL_KIND_NONE;
	val->ref_count = 1;

	return val;
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
		switch (val->fn.kind) {
		case FLAMINGO_FN_KIND_PROTO:
			return "external function";
		case FLAMINGO_FN_KIND_FUNCTION:
			return "function";
		case FLAMINGO_FN_KIND_CLASS:
			return "class";
		default:
			assert(false);
		}
	case FLAMINGO_VAL_KIND_NONE:
		return "none";
	case FLAMINGO_VAL_KIND_INST:
		return "instance";
	default:
		return "unknown";
	}
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
		return val_type_str(val);
	default:
		return "unknown";
	}
}

static flamingo_val_t* val_alloc(void) {
	flamingo_val_t* const val = calloc(1, sizeof *val);
	assert(val != NULL);

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
	if (val == NULL) {
		return NULL;
	}

	val->ref_count--;

	if (val->ref_count > 0) {
		return val;
	}

	// if value is not referred to by anyone, free it

	val_free(val);
	return NULL;
}

flamingo_val_t* flamingo_val_make_none(void) {
	return val_alloc();
}

flamingo_val_t* flamingo_val_make_int(int64_t integer) {
	flamingo_val_t* const val = val_alloc();

	val->kind = FLAMINGO_VAL_KIND_INT;
	val->integer.integer = integer;

	return val;
}

flamingo_val_t* flamingo_val_make_str(size_t size, char* str) {
	flamingo_val_t* const val = val_alloc();

	val->kind = FLAMINGO_VAL_KIND_STR;

	val->str.size = size;
	val->str.str = malloc(size);
	assert(val->str.str != NULL);
	memcpy(val->str.str, str, size);

	return val;
}

flamingo_val_t* flamingo_val_make_cstr(char* str) {
	return flamingo_val_make_str(strlen(str), str);
}

flamingo_val_t* flamingo_val_make_bool(bool boolean) {
	flamingo_val_t* const val = val_alloc();

	val->kind = FLAMINGO_VAL_KIND_BOOL;
	val->boolean.boolean = boolean;

	return val;
}

flamingo_val_t* flamingo_val_find_arg(flamingo_arg_list_t* args, char const* name) {
	for (size_t i = 0; i < args->count; i++) {
		flamingo_val_t* const arg = args->args[i];

		if (flamingo_cstrcmp(arg->name, (char*) name, arg->name_size) == 0) {
			return arg;
		}
	}

	return NULL;
}
