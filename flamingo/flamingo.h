// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct flamingo_t flamingo_t;

typedef int (*flamingo_cb_call_t) (flamingo_t* flamingo, char* name, void* data);

typedef enum {
	FLAMINGO_EXPR_KIND_STR,
} flamingo_expr_kind_t;

typedef struct {
	flamingo_expr_kind_t kind;

	union {
		struct {
			char* str;
			size_t size;
		} str;
	};
} flamingo_expr_t;

typedef struct {
	char* key;
	size_t key_size;
	flamingo_expr_t expr;
} flamingo_var_t;

typedef struct {
	size_t vars_size;
	flamingo_var_t* vars;
} flamingo_scope_t;

struct flamingo_t {
	char const* progname;

	char* src;
	size_t src_size;

	char err[256];
	bool errors_outstanding;

	flamingo_cb_call_t cb_call;

	// runtime stuff

	size_t scope_stack_size;
	flamingo_scope_t* scope_stack;

	// tree-sitter stuff

	void* ts_state;
};

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src, size_t src_size);
void flamingo_destroy(flamingo_t* flamingo);

char* flamingo_err(flamingo_t* flamingo);
void flamingo_register_cb_call(flamingo_t* flamingo, flamingo_cb_call_t cb, void* data);
int flamingo_run(flamingo_t* flamingo);

flamingo_var_t* flamingo_scope_find_var(flamingo_t* flamingo, char const* key, size_t key_size);
