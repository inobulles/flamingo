// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#include "flamingo/flamingo.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
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
#if defined(__FreeBSD__) || defined(__APPLE__)
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

static int external_fn_cb(flamingo_t* flamingo, flamingo_val_t* callable, void* data, flamingo_arg_list_t* args, flamingo_val_t** rv) {
	char* const name = callable->name;
	size_t const name_size = callable->name_size;

	if (flamingo_cstrcmp(name, "test_return_number", name_size) == 0) {
		*rv = flamingo_val_make_int(420);
	}

	else if (flamingo_cstrcmp(name, "test_return_bool", name_size) == 0) {
		*rv = flamingo_val_make_bool(true);
	}

	else if (flamingo_cstrcmp(name, "test_return_str", name_size) == 0) {
		*rv = flamingo_val_make_cstr("zonnebloemgranen");
	}

	else if (flamingo_cstrcmp(name, "test_return_none", name_size) == 0) {
		*rv = flamingo_val_make_none();
	}

	else if (flamingo_cstrcmp(name, "test_do_literally_nothing", name_size) == 0) {
	}

	else if (flamingo_cstrcmp(name, "test_sub", name_size) == 0) {
		if (args->count != 2) {
			return flamingo_raise_error(flamingo, "test_sub: expected 2 arguments, got %zu", args->count);
		}

		flamingo_val_t* const a = args->args[0];
		flamingo_val_t* const b = args->args[1];

		if (a->kind != FLAMINGO_VAL_KIND_INT) {
			return flamingo_raise_error(flamingo, "test_sub: expected 'a' to be an integer");
		}

		if (b->kind != FLAMINGO_VAL_KIND_INT) {
			return flamingo_raise_error(flamingo, "test_sub: expected 'b' to be an integer");
		}

		*rv = flamingo_val_make_int(a->integer.integer - b->integer.integer);
	}

	else {
		return flamingo_raise_error(flamingo, "runtime does not support the '%.*s' external function call (%zu arguments passed)", (int) name_size, name, args->count);
	}

	return 0;
}

static int class_decl_cb(flamingo_t* flamingo, flamingo_val_t* class, void* data) {
	flamingo_scope_t* const scope = class->fn.scope;

	if (flamingo_cstrcmp(class->name, "ExternalClass", class->name_size) == 0) {
		for (size_t i = 0; i < scope->vars_size; i++) {
			flamingo_var_t* const var = &scope->vars[i];

			assert(var->val->owner == scope);
			assert(var->val->owner->owner == class);

			if (flamingo_cstrcmp(var->key, "will_be_modified", var->key_size) == 0) {
				var->val->integer.integer = 420;
			}
		}
	}

	return 0;
}

static int class_inst_cb(flamingo_t* flamingo, flamingo_val_t* inst, void* data, flamingo_arg_list_t* args) {
	flamingo_val_t* const class = inst->inst.class;

	if (flamingo_cstrcmp(class->name, "ExternalClass", class->name_size) == 0) {
		if (args->count != 1) {
			return flamingo_raise_error(flamingo, "ExternalClass: expected 1 argument, got %zu", args->count);
		}

		if (args->args[0]->kind != FLAMINGO_VAL_KIND_INT) {
			return flamingo_raise_error(flamingo, "ExternalClass: expected argument to be an integer");
		}

		if (args->args[0]->integer.integer != 420) {
			return flamingo_raise_error(flamingo, "ExternalClass: expected argument to be 420, got %" PRId64, args->args[0]->integer.integer);
		}
	}

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

	flamingo_register_external_fn_cb(&flamingo, external_fn_cb, NULL);
	flamingo_register_class_decl_cb(&flamingo, class_decl_cb, NULL);
	flamingo_register_class_inst_cb(&flamingo, class_inst_cb, NULL);

	flamingo_add_import_path(&flamingo, "tests/import_path");

	// run program

	if (flamingo_run(&flamingo) < 0) {
		fprintf(stderr, "flamingo: %s\n", flamingo_err(&flamingo));
		goto err_flamingo_run;
	}

	// print out all top-level scope variables

	flamingo_scope_t* const scope = flamingo.env->scope_stack[0];

	for (size_t i = 0; i < scope->vars_size; i++) {
		flamingo_var_t* const var = &scope->vars[i];

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
