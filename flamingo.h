// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <regex.h>

#define FLAMINGO_GEN_ENUM(NAME) NAME,
#define FLAMINGO_GEN_STRS(STR) #STR,

#define FLAMINGO_ENUM(T) \
	typedef enum { T##_ENUM(FLAMINGO_GEN_ENUM) } T; \
	__attribute__((unused)) static char const* const T##_strs[] = { T##_ENUM(FLAMINGO_GEN_STRS) };

#define flamingo_token_kind_t_ENUM(_) \
	_(FLAMINGO_TOKEN_KIND_NOT_TERMINATED) \
	_(FLAMINGO_TOKEN_KIND_NOT_UNDERSTOOD) \
	_(FLAMINGO_TOKEN_KIND_IDENTIFIER) \
	_(FLAMINGO_TOKEN_KIND_DELIMITER) \
	_(FLAMINGO_TOKEN_KIND_OPERATOR) \
	/* delimiters */ \
	_(FLAMINGO_TOKEN_KIND_LPAREN) \
	_(FLAMINGO_TOKEN_KIND_RPAREN) \
	/* literals */ \
	_(FLAMINGO_TOKEN_KIND_LITERAL_DEC) \
	_(FLAMINGO_TOKEN_KIND_LITERAL_BOOL) \
	_(FLAMINGO_TOKEN_KIND_LITERAL_STR) \
	/* statements */ \
	_(FLAMINGO_TOKEN_KIND_IMPORT) \
	/* end */

FLAMINGO_ENUM(flamingo_token_kind_t)

typedef struct {
	flamingo_token_kind_t kind;
	char* str;
	size_t line, col;

	union {
		void* none;
		int64_t int_;
		bool bool_;
		char* str;
	} val;
} flamingo_token_t;

typedef struct {
	bool already_ran;

	flamingo_token_t* tokens;
	size_t token_count;

	// regexes

	bool regexes_compiled;

	regex_t re_identifier;
	regex_t re_dec;

	// state for lexer

	bool tokens_not_understood; // TODO still necessary?

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
