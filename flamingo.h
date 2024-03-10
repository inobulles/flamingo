// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <regex.h>

typedef enum {
	FLAMINGO_TOKEN_KIND_NOT_UNDERSTOOD,

	FLAMINGO_TOKEN_KIND_IDENTIFIER,
	FLAMINGO_TOKEN_KIND_LITERAL,
	FLAMINGO_TOKEN_KIND_DELIMITER,
	FLAMINGO_TOKEN_KIND_OPERATOR,

	// statements

	FLAMINGO_TOKEN_KIND_IMPORT,
} flamingo_token_kind_t;

typedef struct {
	flamingo_token_kind_t kind;
	char* str;
	size_t line, col;
} flamingo_token_t;

typedef struct {
	bool already_ran;

	flamingo_token_t* tokens;
	size_t token_count;

	// regexes

	bool regexes_compiled;

	regex_t re_identifier;

	// state for lexer

	bool tokens_not_understood;

	size_t line;
	size_t col;
} flamingo_lexer_t;

typedef struct flamingo_t flamingo_t;

typedef int (*flamingo_cb_call_t) (flamingo_t* flamingo, char* name, void* data);

struct flamingo_t {
	char const* progname;

	flamingo_lexer_t lexer;

	char err[256];
	bool errors_outstanding;

	flamingo_cb_call_t cb_call;
};

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src);
void flamingo_destroy(flamingo_t* flamingo);

char* flamingo_err(flamingo_t* flamingo);
void flamingo_register_cb_call(flamingo_t* flamingo, flamingo_cb_call_t cb, void* data);
int flamingo_run(flamingo_t* flamingo);
