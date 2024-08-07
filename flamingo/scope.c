// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "flamingo.h"
#include "val.c"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static flamingo_scope_t* cur_scope(flamingo_t* flamingo) {
	return &flamingo->scope_stack[flamingo->scope_stack_size - 1];
}

static void scope_free(flamingo_scope_t* scope) {
	for (size_t i = 0; i < scope->vars_size; i++) {
		flamingo_var_t* const var = &scope->vars[i];

		val_decref(var->val);
		free(var->key);
	}

	free(scope->vars);
}

static flamingo_scope_t* scope_stack_push(flamingo_t* flamingo) {
	flamingo->scope_stack = (/* XXX no clue why I need to explicitly cast here */ flamingo_scope_t*) realloc(flamingo->scope_stack, ++flamingo->scope_stack_size * sizeof *flamingo->scope_stack);
	assert(flamingo->scope_stack != NULL);

	flamingo_scope_t* const scope = &flamingo->scope_stack[flamingo->scope_stack_size - 1];

	scope->vars_size = 0;
	scope->vars = NULL;

	return scope;
}

static flamingo_var_t* scope_add_var(flamingo_scope_t* scope, char const* key, size_t key_size) {
	scope->vars = (flamingo_var_t*) realloc(scope->vars, ++scope->vars_size * sizeof *scope->vars);
	assert(scope->vars != NULL);

	flamingo_var_t* const var = &scope->vars[scope->vars_size - 1];

	var->key = strdup(key);
	assert(var->key != NULL);
	var->key_size = key_size;

	var->val = NULL;

	return var;
}

static void scope_pop(flamingo_t* flamingo) {
	// TODO Free containing variables.
	scope_free(&flamingo->scope_stack[--flamingo->scope_stack_size]);
}

flamingo_var_t* flamingo_scope_find_var(flamingo_t* flamingo, char const* key, size_t key_size) {
	// go backwards down the stack to allow for shadowing

	for (ssize_t i = flamingo->scope_stack_size - 1; i >= 0; i--) {
		flamingo_scope_t* const scope = &flamingo->scope_stack[i];

		for (size_t j = 0; j < scope->vars_size; j++) {
			flamingo_var_t* const var = &scope->vars[j];

			if (var->key_size == key_size && memcmp(var->key, key, key_size) == 0) {
				return var;
			}
		}
	}

	return NULL;
}
