// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include "flamingo.h"
#include "runtime/tree_sitter/api.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int parse_expr(flamingo_t* flamingo, TSNode node, flamingo_val_t** val, flamingo_val_t** accessed_val_ref);
static inline int parse_binary_expr(flamingo_t* flamingo, TSNode node, flamingo_val_t** val);
static inline int access_find_var(flamingo_t* flamingo, TSNode node, flamingo_var_t** var, flamingo_val_t** accessed_val);
static inline int parse_access(flamingo_t* flamingo, TSNode node, flamingo_val_t** val, flamingo_val_t** accessed_val);
static inline int parse_statement(flamingo_t* flamingo, TSNode node);
static inline int parse_block(flamingo_t* flamingo, TSNode node, flamingo_scope_t** inner_scope);
static inline int parse_print(flamingo_t* flamingo, TSNode node);
static inline int parse_return(flamingo_t* flamingo, TSNode node);
static inline int parse_assert(flamingo_t* flamingo, TSNode node);
static inline int parse_literal(flamingo_t* flamingo, TSNode node, flamingo_val_t** val);
static inline int parse_identifier(flamingo_t* flamingo, TSNode node, flamingo_val_t** val);
static inline int parse_call(flamingo_t* flamingo, TSNode node, flamingo_val_t** val);
static inline int parse_var_decl(flamingo_t* flamingo, TSNode node);
static inline int parse_assignment(flamingo_t* flamingo, TSNode node);
static inline int parse_import(flamingo_t* flamingo, TSNode node);
static inline int parse_function_declaration(flamingo_t* flamingo, TSNode node, flamingo_fn_kind_t kind);

__attribute__((format(printf, 2, 3))) static inline int error(flamingo_t* flamingo, char const* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	// TODO validate the size of the program name

	// format caller's error message

	char formatted[sizeof flamingo->err];
	size_t res = vsnprintf(formatted, sizeof formatted, fmt, args);
	assert(res < sizeof formatted);

	// format the new error message and concatenate to the previous one if there were still errors outstanding
	// TODO truncate the number of errors that we show at once (just do this by seeing how much longer we have)

	if (flamingo->errors_outstanding) {
		size_t const prev_len = strlen(flamingo->err);
		res = snprintf(flamingo->err + prev_len, sizeof flamingo->err - prev_len, ", %s:%d:%d: %s", flamingo->progname, 0, 0, formatted);
	}

	else {
		res = snprintf(flamingo->err, sizeof flamingo->err, "%s:%d:%d: %s", flamingo->progname, 0, 0, formatted);
	}

	assert(res < sizeof flamingo->err);

	va_end(args);
	flamingo->errors_outstanding = true;

	return -1;
}
