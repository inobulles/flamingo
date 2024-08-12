// This Source Form is subject to the terms of the AQUA Software License,
// v. 1.0. Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>

static int parse_import_path(flamingo_t* flamingo, TSNode node) {
	assert(strcmp(ts_node_type(node), "import_path") == 0);
	assert(ts_node_child_count(node) == 2);

	return 0;
}

static int parse_import(flamingo_t* flamingo, TSNode node) {
	assert(strcmp(ts_node_type(node), "import") == 0);
	printf("%d\n", ts_node_child_count(node));
	assert(ts_node_child_count(node) == 2);

	TSNode const relative_node = ts_node_child_by_field_name(node, "relative", 8);
	bool const is_relative = !ts_node_is_null(relative_node);

	TSNode const path_node = ts_node_child_by_field_name(node, "path", 4);
	char const* const path_type = ts_node_type(path_node);

	if (strcmp(path_type, "import_path") != 0) {
		return error(flamingo, "expected import_path for path, got %s", path_type);
	}

	// Parse the import path into an actual string path we can use.

	(void) is_relative;
	(void) parse_import_path;

	return error(flamingo, "TODO");
}
