// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#include "flamingo/flamingo.h"

#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__linux__)
# include <sys/prctl.h>
#endif

static char const* init_name = "flamingo";

static void usage(void) {
#if defined(__FreeBSD__)
	char const* const progname = getprogname();
#elif defined(__linux__)
	char progname[16];
	strncpy(progname, init_name, sizeof progname);

	if (prctl(PR_GET_NAME, progname, NULL, NULL, NULL) < 0) {
		fprintf(stderr, "prctl(PR_GET_NAME): %s", strerror(errno));
	}
#else
	char const* const progname = init_name;
#endif

	fprintf(stderr, "usage: %1$s source_filename\n", progname);

	exit(EXIT_FAILURE);
}

static int call_cb(flamingo_t* flamingo, char* name, void* data) {
	printf("function call: %s\n", name);
	return 0;
}

int main(int argc, char* argv[]) {
	init_name = *argv;

	// parse arguments

	if (argc != 2) {
		usage();
	}

	int rv = EXIT_FAILURE;

	char* const rel_path = argv[1];
	char* const path = realpath(rel_path, NULL);

	if (path == NULL) {
		fprintf(stderr, "realpath(\"%s\"): %s\n", rel_path, strerror(errno));
		goto err_realpath;
	}

	// read source file

	struct stat sb;

	if (stat(path, &sb) < 0) {
		fprintf(stderr, "stat(\"%s\"): %s\n", rel_path, strerror(errno));
		goto err_stat;
	}

	size_t const src_size = sb.st_size;

	FILE* const f = fopen(path, "r");

	if (f == NULL) {
		fprintf(stderr, "fopen(\"%s\"): %s\n", rel_path, strerror(errno));
		goto err_fopen;
	}

	char* const src = malloc(src_size);
	assert(src != NULL);

	if (fread(src, 1, src_size, f) != src_size) {
		fprintf(stderr, "fread(\"%s\"): %s\n", rel_path, strerror(errno));
		goto err_fread;
	}

	// create flamingo engine

	flamingo_t flamingo;

	if (flamingo_create(&flamingo, basename(path), src, src_size) < 0) {
		fprintf(stderr, "flamingo: %s\n", flamingo_err(&flamingo));
		goto err_flamingo_create;
	}

	flamingo_register_cb_call(&flamingo, call_cb, NULL);

	// run program

	if (flamingo_run(&flamingo) < 0) {
		fprintf(stderr, "flamingo: %s\n", flamingo_err(&flamingo));
		goto err_flamingo_run;
	}

	// print out all top-level scope variables

	flamingo_scope_t* const scope = flamingo.scope_stack[0];

	for (size_t j = 0; j < scope->vars_size; j++) {
		flamingo_var_t* const var = &scope->vars[j];

		printf("var: %.*s\n", (int) var->key_size, var->key);
	}

	// finished everything successfully!

	rv = EXIT_SUCCESS;

err_flamingo_run:

	flamingo_destroy(&flamingo);

err_flamingo_create:
err_fread:

	free(src);
	fclose(f);

err_fopen:
err_stat:

	free(path);

err_realpath:

	return rv;
}
