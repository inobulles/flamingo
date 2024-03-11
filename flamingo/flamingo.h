// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct flamingo_t flamingo_t;

typedef int (*flamingo_cb_call_t) (flamingo_t* flamingo, char* name, void* data);

struct flamingo_t {
	char const* progname;

	char* src;
	size_t src_size;

	char err[256];
	bool errors_outstanding;

	flamingo_cb_call_t cb_call;

	// tree-sitter stuff

	void* ts_state;
};

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src, size_t src_size);
void flamingo_destroy(flamingo_t* flamingo);

char* flamingo_err(flamingo_t* flamingo);
void flamingo_register_cb_call(flamingo_t* flamingo, flamingo_cb_call_t cb, void* data);
int flamingo_run(flamingo_t* flamingo);
