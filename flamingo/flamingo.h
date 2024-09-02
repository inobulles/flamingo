// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct flamingo_t flamingo_t;

typedef int (*flamingo_cb_call_t)(flamingo_t* flamingo, char* name, void* data);

typedef enum {
	FLAMINGO_VAL_KIND_NONE,
	FLAMINGO_VAL_KIND_BOOL,
	FLAMINGO_VAL_KIND_STR,
	FLAMINGO_VAL_KIND_FN,
	FLAMINGO_VAL_KIND_CLASS,
} flamingo_val_kind_t;

typedef void* flamingo_ts_node_t;

typedef struct {
	flamingo_val_kind_t kind;
	size_t ref_count;

	union {
		struct {
			bool val;
		} boolean;

		struct {
			char* str;
			size_t size;
		} str;

		struct {
			uint64_t integer;
		} integer;

		struct {
			flamingo_ts_node_t body;
			flamingo_ts_node_t params;

			// Functions can be defined in other files entirely.
			// While the Tree-sitter state is held within the nodes themselves, the source they point to is not, which is why we need to keep track of it here.

			char* src;
			size_t src_size;
		} fn;

		struct {
			flamingo_ts_node_t body;

			// Ditto as for functions.

			char* src;
			size_t src_size;
		} class;
	};
} flamingo_val_t;

typedef struct {
	char* key;
	size_t key_size;
	flamingo_val_t* val;
} flamingo_var_t;

typedef struct {
	size_t vars_size;
	flamingo_var_t* vars;
} flamingo_scope_t;

struct flamingo_t {
	char const* progname;
	bool consistent; // Set if we managed to create the instance, so we know whether or not it still needs freeing.

	char* src;
	size_t src_size;

	char err[256];
	bool errors_outstanding;

	flamingo_cb_call_t cb_call;

	// Runtime stuff.

	bool inherited_scope_stack;
	size_t scope_stack_size;
	flamingo_scope_t* scope_stack;

	// Tree-sitter stuff.

	void* ts_state;

	// Import-related stuff, i.e. stuff we have to free ourselves.

	size_t import_count;
	flamingo_t* imported_flamingos;
	char** imported_srcs;

	// Current function stuff.

	flamingo_ts_node_t cur_fn_body;
	flamingo_val_t* cur_fn_rv;
};

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src, size_t src_size);
void flamingo_destroy(flamingo_t* flamingo);

char* flamingo_err(flamingo_t* flamingo);
void flamingo_register_cb_call(flamingo_t* flamingo, flamingo_cb_call_t cb, void* data);
int flamingo_inherit_scope_stack(flamingo_t* flamingo, size_t stack_size, flamingo_scope_t* stack);
int flamingo_run(flamingo_t* flamingo);

flamingo_var_t* flamingo_scope_find_var(flamingo_t* flamingo, char const* key, size_t key_size);
