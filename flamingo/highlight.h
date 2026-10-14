// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2026 Aymeric Wibo

/*
 * Syntax highlighting for error snippets.
 *
 * This runs tree-sitter-flamingo's 'highlights.scm' query (embedded as 'flamingo_highlights_query') over a range of the source and produces a per-byte array of ANSI SGR colour codes, which 'render_snippet' uses to colourise the offending line.
 */

#pragma once

#include "common.h"
#include "highlights.h"

extern TSLanguage const* tree_sitter_flamingo(void);

// Does the capture name's leading component equal 'prefix'?
// Highlight names are dotted (e.g. "function.method"), and we colour by the leading component.

static inline bool highlight_cap_is(char const* name, size_t len, char const* prefix) {
	size_t const prefix_len = strlen(prefix);

	if (len < prefix_len || memcmp(name, prefix, prefix_len) != 0) {
		return false;
	}

	return len == prefix_len || name[prefix_len] == '.';
}

// Map a highlight capture name to an ANSI SGR foreground colour code, or 0 for no colour.

static inline uint8_t highlight_color(char const* name, size_t len) {
	if (highlight_cap_is(name, len, "keyword")) {
		return 35; // magenta
	}

	if (highlight_cap_is(name, len, "string")) {
		return 32; // green
	}

	if (highlight_cap_is(name, len, "number") || highlight_cap_is(name, len, "constant")) {
		return 33; // yellow
	}

	if (highlight_cap_is(name, len, "function")) {
		return 34; // blue
	}

	if (highlight_cap_is(name, len, "type")) {
		return 36; // cyan
	}

	if (highlight_cap_is(name, len, "comment")) {
		return 90; // bright black (grey)
	}

	return 0; // Variables, operators, and anything unrecognised: no colour.
}

// Compile the embedded highlight query.
// Tree-sitter rejects the whole query if any single pattern mentions a node type, field, or capture the grammar doesn't have. Since the parser and the query can drift apart across grammar versions, we blank out the offending line and retry: an outdated pattern then disables only its own highlight instead of all of them. On a genuine syntax error (a bug in our own query), we give up and highlighting is simply skipped.

static inline TSQuery* highlight_compile_query(void) {
	uint32_t const len = (uint32_t) strlen(flamingo_highlights_query);
	char* const source = malloc(len + 1);

	if (source == NULL) {
		return NULL;
	}

	memcpy(source, flamingo_highlights_query, len + 1);
	TSQuery* query = NULL;

	for (;;) {
		uint32_t error_offset;
		TSQueryError error_type;

		query = ts_query_new(tree_sitter_flamingo(), source, len, &error_offset, &error_type);

		if (query != NULL || error_type == TSQueryErrorSyntax || error_offset >= len) {
			break;
		}

		// Blank out the line containing the offending token, keeping all byte offsets stable.

		size_t line_start = error_offset;

		while (line_start > 0 && source[line_start - 1] != '\n') {
			line_start--;
		}

		size_t line_end = error_offset;

		while (line_end < len && source[line_end] != '\n') {
			line_end++;
		}

		// If there's nothing left to blank on this line, we'd loop forever, so give up.

		bool blanked = false;

		for (size_t i = line_start; i < line_end; i++) {
			if (source[i] != ' ') {
				source[i] = ' ';
				blanked = true;
			}
		}

		if (!blanked) {
			break;
		}
	}

	free(source);
	return query;
}

// Fill 'colors' (one entry per byte of the source range [start, end)) with ANSI SGR colour codes (0 = no colour).
// Highlighting is best-effort: on any failure, the range is simply left uncoloured.

static inline void highlight_range(TSNode root, size_t start, size_t end, uint8_t* colors) {
	size_t const len = end - start;
	memset(colors, 0, len);

	TSQuery* const query = highlight_compile_query();

	if (query == NULL) {
		return;
	}

	// Track which query pattern coloured each byte.
	// The query is ordered so that the last matching pattern wins (see 'highlights.scm'), so we only ever overwrite with a pattern of equal or higher index. This makes the result independent of the order in which captures are returned.

	int* const winner = malloc(len * sizeof *winner);

	if (winner == NULL) {
		ts_query_delete(query);
		return;
	}

	for (size_t i = 0; i < len; i++) {
		winner[i] = -1;
	}

	TSQueryCursor* const cursor = ts_query_cursor_new();
	ts_query_cursor_set_byte_range(cursor, (uint32_t) start, (uint32_t) end);
	ts_query_cursor_exec(cursor, query, root);

	TSQueryMatch match;
	uint32_t capture_index;

	while (ts_query_cursor_next_capture(cursor, &match, &capture_index)) {
		TSQueryCapture const capture = match.captures[capture_index];

		uint32_t name_len;
		char const* const name = ts_query_capture_name_for_id(query, capture.index, &name_len);
		uint8_t const color = highlight_color(name, name_len);

		if (color == 0) {
			continue;
		}

		size_t cap_start = ts_node_start_byte(capture.node);
		size_t cap_end = ts_node_end_byte(capture.node);

		if (cap_start < start) {
			cap_start = start;
		}

		if (cap_end > end) {
			cap_end = end;
		}

		for (size_t i = cap_start; i < cap_end; i++) {
			if (match.pattern_index >= winner[i - start]) {
				colors[i - start] = color;
				winner[i - start] = match.pattern_index;
			}
		}
	}

	ts_query_cursor_delete(cursor);
	free(winner);
	ts_query_delete(query);
}
