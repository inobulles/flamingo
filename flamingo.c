// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#include "flamingo.h"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

__attribute__((format(printf, 2, 3)))
static int error(flamingo_t* flamingo, char const* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	// TODO validate the size of the program name

	flamingo_lexer_t* const lexer = &flamingo->lexer;

	// format callers's error message

	char formatted[sizeof flamingo->err];
	size_t res = vsnprintf(formatted, sizeof formatted, fmt, args);
	assert(res < sizeof formatted);

	// format the new error message and concatenate to the previous one if there were still errors outstanding
	// TODO truncate the number of errors that we show at once (just do this by seeing how much longer we have)

	if (flamingo->errors_outstanding) {
		size_t const prev_len = strlen(flamingo->err);
		res = snprintf(flamingo->err + prev_len, sizeof flamingo->err - prev_len, ", %s:%zu:%zu: %s", flamingo->progname, lexer->line, lexer->col, formatted);
	}

	else {
		res = snprintf(flamingo->err, sizeof flamingo->err, "%s:%zu:%zu: %s", flamingo->progname, lexer->line, lexer->col, formatted);
	}

	assert(res < sizeof flamingo->err);

	va_end(args);
	flamingo->errors_outstanding = true;

	return -1;
}

static int compile_regexes(flamingo_t* flamingo) {
	flamingo_lexer_t* const lexer = &flamingo->lexer;

	if (lexer->regexes_compiled) {
		return 0;
	}

	lexer->regexes_compiled = true;

	assert(regcomp(&lexer->re_identifier, "^[_A-z][_A-z0-9]*$", REG_EXTENDED) == 0);
	assert(regcomp(&lexer->re_dec, "^[_0-9]+$", REG_EXTENDED) == 0);

	return 0;
}

static void understand(flamingo_t* flamingo, flamingo_token_t* tok) {
	flamingo_lexer_t* const lexer = &flamingo->lexer;
	char* const str = tok->str;

	// TODO accept more than just ASCII

	// exact matches

#define MATCH(_str, _kind, val_member, _val) \
	if (strcmp(str, _str) == 0) { \
		tok->kind = (_kind); \
		tok->val.val_member = (_val); \
		return; \
	}

	// literals

	MATCH("true", FLAMINGO_TOKEN_KIND_LITERAL_BOOL, bool_, true);
	MATCH("false", FLAMINGO_TOKEN_KIND_LITERAL_BOOL, bool_, false);

	// statements

	MATCH("import", FLAMINGO_TOKEN_KIND_IMPORT, none, NULL);

#undef MATCH

	// regex matches

#define MATCH(_re, _kind) \
	if (regexec(&lexer->re_##_re, str, 0, NULL, 0) != REG_NOMATCH) { \
		tok->kind = (_kind); \
		return; \
	}

	MATCH(identifier, FLAMINGO_TOKEN_KIND_IDENTIFIER);
	MATCH(dec, FLAMINGO_TOKEN_KIND_LITERAL_DEC);

#undef MATCH

	// TODO truncate the size of the token if too long, otherwise error message will be too long

	tok->kind = FLAMINGO_TOKEN_KIND_NOT_UNDERSTOOD;
	lexer->tokens_not_understood = true;
}

static int lex(flamingo_t* flamingo, char* src) {
	flamingo_lexer_t* const lexer = &flamingo->lexer;

	if (lexer->already_ran) {
		return error(flamingo, "lexer already ran");
	}

	lexer->already_ran = true;

	lexer->tokens = NULL;
	lexer->token_count = 0;

	// set up state

	lexer->line = 1;
	lexer->col = 1;

	lexer->tokens_not_understood = false;

	// compile regexes

	if (compile_regexes(flamingo) < 0) {
		return -1;
	}

	// tokenize source

	size_t const src_len = strlen(src);

	bool in_tok = false;
	bool in_comment = false;

	char* tok;

	for (; *src > 0; src++, lexer->col++) {
		// don't wanna deal with any CRLF bullshit

		if (*src == '\r') {
			return error(flamingo, "CR character not supported");
		}

		// check for whitespace, newline, or start of comment

		bool const is_space = *src == ' ' || *src == '\t' || *src == '\v';
		bool const is_newline = *src == '\n';
		bool const is_comment = *src == '#';

		if (is_space || is_newline || is_comment) {
			*src = '\0';

			if (in_tok) {
				understand(flamingo, &lexer->tokens[lexer->token_count - 1]);
			}

			in_tok = false;

			if (is_newline) {
				in_comment = false;
				lexer->line++;
				lexer->col = 0;
			}

			if (is_comment) {
				in_comment = true;
			}

			continue;
		}

		// ignore if in comment

		if (in_comment) {
			continue;
		}

		// check for token

		if (in_tok == true) {
			continue;
		}

		if (in_tok == false) {
			in_tok = true;
			tok = src;
		}

		// add token

		lexer->tokens = realloc(lexer->tokens, (lexer->token_count + 1) * sizeof *lexer->tokens);
		flamingo_token_t* const token = &lexer->tokens[lexer->token_count++];
		token->str = tok;

		token->line = lexer->line;
		token->col = lexer->col;
	}

	// if there were tokens not understood, return an error
	// TODO can this be done straight in the "understand" function and reduce all this complexity? I did it like this initially because tokens weren't already null-terminated being passed to "understand", but not they are, so I don't know if this is still relevant

	if (lexer->tokens_not_understood) {
		for (size_t i = 0; i < lexer->token_count; i++) {
			flamingo_token_t* const tok = &lexer->tokens[i];

			if (tok->kind != FLAMINGO_TOKEN_KIND_NOT_UNDERSTOOD) {
				continue;
			}

			error(flamingo, "unrecognized token: %s", tok->str);
		}

		return -1;
	}

	// go through literal tokens and parse their values

	for (size_t i = 0; i < lexer->token_count; i++) {
		flamingo_token_t* const tok = &lexer->tokens[i];

		if (tok->kind == FLAMINGO_TOKEN_KIND_LITERAL_DEC) {
			errno = 0;
			tok->val.int_ = strtoll(tok->str, NULL, 10);

			assert(errno != EINVAL);
			
			if (errno == ERANGE) {
				error(flamingo, "integer literal out of range: %s", tok->str);
				return -1;
			}
		}
	}

	return 0;
}

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src) {
	flamingo->progname = progname;

	if (lex(flamingo, src) < 0) {
		return -1;
	}
	 
	fprintf(stderr, "%s: not implemented (grammar parsing)\n", __func__);

	return 0;
}

void flamingo_destroy(flamingo_t* flamingo) {
	// free lexer stuff

	flamingo_lexer_t* const lexer = &flamingo->lexer;
	free(lexer->tokens);

	if (lexer->regexes_compiled) {
		regfree(&lexer->re_identifier);
	}
}

char* flamingo_err(flamingo_t* flamingo) {
	if (!flamingo->errors_outstanding) {
		return "no errors";
	}

	flamingo->errors_outstanding = false;
	return flamingo->err;
}

void flamingo_register_cb_call(flamingo_t* flamingo, flamingo_cb_call_t cb, void* data) {
	fprintf(stderr, "%s: not implemented\n", __func__);
}

int flamingo_run(flamingo_t *flamingo) {
	// print out tokens

	for (size_t i = 0; i < flamingo->lexer.token_count; i++) {
		flamingo_token_t* const tok = &flamingo->lexer.tokens[i];
		printf("%s:%zu:%zu: %s\n", __func__, tok->line, tok->col, tok->str);
	}

	fprintf(stderr, "%s: not implemented (actually run shit)\n", __func__);

	return 0;
}
