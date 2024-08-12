// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>

#include <errno.h>
#include <sys/stat.h>

static int import(flamingo_t* flamingo, char* path) {
	// TODO This is really relative to the caller, not the current file.
	// Should it be relative to the current file though?
	// If we do it like that, then a.b.c wouldn't be able to import a.b for example.
	// Maybe we should do it relative to the first file that was parsed, and then relative to the import path when importing non-relatively.

	int rv = 0;

	// Read file.

	struct stat sb;

	if (stat(path, &sb) < 0) {
		rv = error(flamingo, "failed to import '%s': stat: %s\n", path, strerror(errno));
		goto err_stat;
	}

	size_t const src_size = sb.st_size;

	FILE* const f = fopen(path, "r");

	if (f == NULL) {
		rv = error(flamingo, "failed to import '%s': fopen: %s\n", path, strerror(errno));
		goto err_fopen;
	}

	char* const src = malloc(src_size);
	assert(src != NULL);

	if (fread(src, 1, src_size, f) != src_size) {
		rv = error(flamingo, "failed to import '%s': fread: %s\n", path, strerror(errno));
		goto err_fread;
	}

	// Create new flamingo engine.
	// TODO There should be a way to pass it the current scope, so that it can hook into this one.

	flamingo_t imported_flamingo;

	if (flamingo_create(&imported_flamingo, flamingo->progname, src, src_size) < 0) {
		rv = error(flamingo, "failed to import '%s': flamingo_create: %s\n", path, strerror(errno));
		goto err_flamingo_create;
	}

	flamingo_register_cb_call(&imported_flamingo, flamingo->cb_call, NULL);

	// Run the imported program.

	if (flamingo_run(&imported_flamingo) < 0) {
		rv = error(flamingo, "failed to import '%s': flamingo_run: %s\n", path, strerror(errno));
		goto err_flamingo_run;
	}

	// TODO What about just appending the scope of the imported program to the current one instead of adding the imported program to our current scope?
	// That would prevent the imported program from accessing stuff in our program, which is desirable.
	// However I don't know how functions would work exactly in that case.
	// Maybe when we call a function we'd have to remember which flamingo engine it was part of.
	// And that actually makes things difficult if that function modifies state in its module's scope, as we'd have to keep track of that too.

err_flamingo_run:
err_flamingo_create:
err_fread:

	free(src);
	fclose(f);

err_fopen:
err_stat:

	free(path);

	return rv;
}

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
	int rv = asprintf(&import_path, "%.*s.fl", (int) path_len - 1, path);
	free(path);

	if (rv < 0 || import_path == NULL) {
		assert(rv < 0 && import_path == NULL);
		return error(flamingo, "failed to allocate memory for import path");
	}

	// We don't support global imports yet.

	if (!is_relative) {
		return error(flamingo, "global imports are not supported yet (trying to import '%s')", import_path);
	}

	rv = import(flamingo, import_path);
	free(import_path);

	return rv;
}
