// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>

static int parse_import_path(flamingo_t* flamingo, TSNode node, char** path_ref, size_t* path_len_ref) {
	assert(strcmp(ts_node_type(node), "import_path") == 0);
	assert(ts_node_child_count(node) == 1 || ts_node_child_count(node) == 3);

	// Get current path component (bit).

	TSNode const bit_node = ts_node_child_by_field_name(node, "bit", 3);
	char const* const bit_type = ts_node_type(bit_node);

	if (strcmp(bit_type, "identifier") != 0) {
		return error(flamingo, "expected identifier for bit, got %s", bit_type);
	}

	// Get the rest of the path.

	TSNode const rest_node = ts_node_child_by_field_name(node, "rest", 4);
	bool const has_rest = !ts_node_is_null(rest_node);

	if (has_rest) {
		char const* const rest_type = ts_node_type(rest_node);

		if (strcmp(rest_type, "import_path") != 0) {
			return error(flamingo, "expected import_path for rest, got %s", rest_type);
		}
	}

	// Get the actual path component and add it to our path accumulator.

	size_t const start = ts_node_start_byte(bit_node);
	size_t const end = ts_node_end_byte(bit_node);

	char const* const bit = flamingo->src + start;
	size_t const size = end - start;

	*path_ref = realloc(*path_ref, *path_len_ref + size + 1);

	if (*path_ref == NULL) {
		free(*path_ref);
		return error(flamingo, "failed to allocate memory for import path");
	}

	memcpy(*path_ref + *path_len_ref, bit, size);
	*path_len_ref += size + 1;
	(*path_ref)[*path_len_ref - 1] = '/';

	// Recursively parse the rest of the path.

	if (has_rest && parse_import_path(flamingo, rest_node, path_ref, path_len_ref) < 0) {
		return -1;
	}

	return 0;
}

static int parse_import(flamingo_t* flamingo, TSNode node) {
	assert(strcmp(ts_node_type(node), "import") == 0);
	assert(ts_node_child_count(node) == 3);

	// Is the import relative to the current file?
	// If so, it means we need to follow the import file relative to the current one.
	// Otherwise, we'll have to iterate through the import paths to find it.

	TSNode const relative_node = ts_node_child_by_field_name(node, "relative", 8);
	bool const is_relative = !ts_node_is_null(relative_node);

	// Get the path of the import.

	TSNode const path_node = ts_node_child_by_field_name(node, "path", 4);
	char const* const path_type = ts_node_type(path_node);

	if (strcmp(path_type, "import_path") != 0) {
		return error(flamingo, "expected import_path for path, got %s", path_type);
	}

	// Parse the import path into an actual string path we can use.

	(void) is_relative;

	char* path = NULL;
	size_t path_len = 0;

	if (parse_import_path(flamingo, path_node, &path, &path_len) < 0) {
		free(path);
		return -1;
	}

	for (size_t i = 0; i < path_len; i++) {
		if (path[i] == '\0') {
			free(path);
			return error(flamingo, "one of the import path components contains a null byte somehow");
		}
	}

	char* import_path = NULL;
	int const rv = asprintf(&import_path, "%.*s.fl", (int) path_len - 1, path);
	free(path);

	if (rv < 0 || import_path == NULL) {
		assert(rv < 0 && import_path == NULL);
		return error(flamingo, "failed to allocate memory for import path");
	}

	printf("Import path: %s\n", import_path);
	free(import_path);

	return error(flamingo, "TODO");
}
