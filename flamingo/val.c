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
	val->kind = FLAMINGO_VAL_KIND_NONE;
	val->ref_count = 1;

	return val;
}

static flamingo_val_t* val_alloc(void) {
	flamingo_val_t* const val = calloc(1, sizeof *val);
	return val_init(val);
}

static void val_free(flamingo_val_t* val) {
	if (val->kind == FLAMINGO_VAL_KIND_STR) {
		free(val->str.str);
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
