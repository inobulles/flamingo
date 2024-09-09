// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "flamingo.h"
#include <val.c>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static flamingo_scope_t* parent_scope(flamingo_t* flamingo) {
	if (flamingo->scope_stack_size < 2) {
		return NULL;
	}

	return flamingo->scope_stack[flamingo->scope_stack_size - 2];
}

static flamingo_scope_t* cur_scope(flamingo_t* flamingo) {
	assert(flamingo->scope_stack_size >= 1);
	return flamingo->scope_stack[flamingo->scope_stack_size - 1];
}

static flamingo_scope_t* scope_alloc(void) {
	flamingo_scope_t* const scope = malloc(sizeof *scope);
	assert(scope != NULL);

	scope->vars_size = 0;
	scope->vars = NULL;

	return scope;
}

static void scope_free(flamingo_scope_t* scope) {
	for (size_t i = 0; i < scope->vars_size; i++) {
		flamingo_var_t* const var = &scope->vars[i];

		val_decref(var->val);
		free(var->key);
	}

	free(scope->vars);
	free(scope);
}

static void scope_gently_attach(flamingo_t* flamingo, flamingo_scope_t* scope) {
	flamingo->scope_stack = realloc(flamingo->scope_stack, ++flamingo->scope_stack_size * sizeof *flamingo->scope_stack);
	assert(flamingo->scope_stack != NULL);

	flamingo->scope_stack[flamingo->scope_stack_size - 1] = scope;
}

static flamingo_scope_t* scope_stack_push(flamingo_t* flamingo) {
	flamingo_scope_t* const scope = scope_alloc();
	scope_gently_attach(flamingo, scope);

	flamingo_scope_t* const parent = parent_scope(flamingo);
	scope->class_scope = parent != NULL ? parent->class_scope : false;

	return scope;
}

static flamingo_var_t* scope_add_var(flamingo_scope_t* scope, char const* key, size_t key_size) {
	scope->vars = (flamingo_var_t*) realloc(scope->vars, ++scope->vars_size * sizeof *scope->vars);
	assert(scope->vars != NULL);

	flamingo_var_t* const var = &scope->vars[scope->vars_size - 1];

	var->key = malloc(key_size);
	assert(var->key != NULL);

	var->key_size = key_size;
	memcpy(var->key, key, key_size);

	var->val = NULL;

	return var;
}

static void var_set_val(flamingo_var_t* var, flamingo_val_t* val) {
	if (var->val != NULL && val != NULL) {
		val->name = NULL;
		val->name_size = 0;
	}

	var->val = val;

	if (val != NULL) {
		val->name = var->key;
		val->name_size = var->key_size;
	}
}

static flamingo_scope_t* scope_gently_detach(flamingo_t* flamingo) {
	return flamingo->scope_stack[--flamingo->scope_stack_size];
}

static void scope_pop(flamingo_t* flamingo) {
	// TODO Free containing variables.
	scope_free(scope_gently_detach(flamingo));
}

flamingo_var_t* scope_shallow_find_var(flamingo_scope_t* scope, char const* key, size_t key_size) {
	for (size_t i = 0; i < scope->vars_size; i++) {
		flamingo_var_t* const var = &scope->vars[i];

		if (var->key_size == key_size && memcmp(var->key, key, key_size) == 0) {
			return var;
		}
	}

	return NULL;
}

flamingo_var_t* flamingo_scope_find_var(flamingo_t* flamingo, char const* key, size_t key_size) {
	// go backwards down the stack to allow for shadowing

	for (ssize_t i = flamingo->scope_stack_size - 1; i >= 0; i--) {
		flamingo_scope_t* const scope = flamingo->scope_stack[i];
		flamingo_var_t* const var = scope_shallow_find_var(scope, key, key_size);

		if (var != NULL) {
			return var;
		}
	}

	return NULL;
}
