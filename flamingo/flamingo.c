// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#include "flamingo.h"
#include "tree_sitter/parser.h"
#include "runtime/tree_sitter/api.h"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern TSLanguage const* tree_sitter_flamingo(void);

__attribute__((format(printf, 2, 3)))
static int error(flamingo_t* flamingo, char const* fmt, ...) {
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

int flamingo_create(flamingo_t* flamingo, char const* progname, char* src) {
	TSParser* const parser = ts_parser_new();

	if (parser == NULL) {
		return error(flamingo, "failed to create Tree-sitter parser");
	}

	TSLanguage const* const lang = tree_sitter_flamingo();
	ts_parser_set_language(parser, lang);

	TSTree* const tree = ts_parser_parse_string(parser, NULL, src, strlen(src));

	if (tree == NULL) {
		ts_parser_delete(parser);
		return error(flamingo, "failed to parse source");
	}

	TSNode const root = ts_tree_root_node(tree);
	printf("%s\n", ts_node_string(root));

	ts_parser_delete(parser);
	ts_tree_delete(tree);

	return 0;
}

void flamingo_destroy(flamingo_t* flamingo) {
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
	fprintf(stderr, "%s: not implemented (actually run shit)\n", __func__);
	return 0;
}
