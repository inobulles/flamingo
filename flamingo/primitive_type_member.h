// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <common.h>
#include <val.h>
#include <var.h>

static void primitive_type_member_init(flamingo_t* flamingo) {
	for (size_t i = 0; i < FLAMINGO_VAL_KIND_COUNT; i++) {
		flamingo->primitive_type_members[i].count = 0;
		flamingo->primitive_type_members[i].vars = NULL;
	}
}

static void primitive_type_member_free(flamingo_t* flamingo) {
	for (size_t type = 0; type < FLAMINGO_VAL_KIND_COUNT; type++) {
		size_t const count = flamingo->primitive_type_members[type].count;
		flamingo_var_t* const vars = flamingo->primitive_type_members[type].vars;

		for (size_t i = 0; i < count; i++) {
			flamingo_var_t* const var = &vars[i];
			free(var->key);
		}

		if (vars != NULL) {
			free(vars);
		}
	}
}

static int primitive_type_member_add(flamingo_t* flamingo, flamingo_val_kind_t type, size_t key_size, char* key, flamingo_ptm_cb_t cb) {
	// Make sure primitive type member doesn't already exist for this type.

	size_t count = flamingo->primitive_type_members[type].count;
	flamingo_var_t* vars = flamingo->primitive_type_members[type].vars;

	for (size_t i = 0; i < count; i++) {
		flamingo_var_t* const var = &vars[i];

		if (flamingo_strcmp(var->key, key, var->key_size, key_size) == 0) {
			// XXX A little hacky and I needn't forget to update this if 'val_type_str' gets more stuff but eh.

			flamingo_val_t const dummy = {
				.kind = type,
				.fn = {
						 .kind = FLAMINGO_FN_KIND_FUNCTION,
						 },
			};

			return error(flamingo, "primitive type member '%.*s' already exists on type %s", (int) key_size, key, val_type_str(&dummy));
		}
	}

	// If not, add an entry.

	vars = realloc(vars, ++count * sizeof *vars);
	assert(vars != NULL);
	flamingo_var_t* const var = &vars[count - 1];

	var->key_size = key_size;
	var->key = malloc(key_size);
	assert(var->key != NULL);
	memcpy(var->key, key, key_size);

	flamingo->primitive_type_members[type].count = count;
	flamingo->primitive_type_members[type].vars = vars;

	// Finally, create the actual value.

	flamingo_val_t* const val = val_alloc();
	var_set_val(var, val);

	val->kind = FLAMINGO_VAL_KIND_FN;
	val->fn.kind = FLAMINGO_FN_KIND_PTM;
	val->fn.ptm_cb = cb;
	val->fn.env = NULL;
	val->fn.src = NULL;

	return 0;
}

static int str_len(flamingo_t* flamingo, flamingo_val_t* self, flamingo_arg_list_t* args, flamingo_val_t** rv) {
	assert(self->kind == FLAMINGO_VAL_KIND_STR);

	if (args->count != 0) {
		return error(flamingo, "'str.len' expected 0 arguments, got %zu", args->count);
	}

	*rv = flamingo_val_make_int(self->str.size);
	return 0;
}

static int str_startswith(flamingo_t* flamingo, flamingo_val_t* self, flamingo_arg_list_t* args, flamingo_val_t** rv) {
	assert(self->kind == FLAMINGO_VAL_KIND_STR);

	// Check our arguments.

	if (args->count != 1) {
		return error(flamingo, "'str.startswith' expected 1 argument, got %zu", args->count);
	}

	flamingo_val_t* const start = args->args[0];

	if (start->kind != FLAMINGO_VAL_KIND_STR) {
		return error(flamingo, "'str.startswith' expected 'start' argument to be a string, got a %s", val_role_str(start));
	}

	// Actually check self's string starts with 'start'.

	bool startswith = false;

	if (start->str.size <= self->str.size) {
		startswith = memcmp(self->str.str, start->str.str, start->str.size) == 0;
	}

	*rv = flamingo_val_make_bool(startswith);

	return 0;
}

static int str_endswith(flamingo_t* flamingo, flamingo_val_t* self, flamingo_arg_list_t* args, flamingo_val_t** rv) {
	assert(self->kind == FLAMINGO_VAL_KIND_STR);

	// Check our arguments.

	if (args->count != 1) {
		return error(flamingo, "'str.endswith' expected 1 argument, got %zu", args->count);
	}

	flamingo_val_t* const end = args->args[0];

	if (end->kind != FLAMINGO_VAL_KIND_STR) {
		return error(flamingo, "'str.endswith' expected 'end' argument to be a string, got a %s", val_role_str(end));
	}

	// Actually check self's string ends with 'end'.

	bool endswith = false;

	if (end->str.size <= self->str.size) {
		size_t const delta = self->str.size - end->str.size;
		endswith = memcmp(self->str.str + delta, end->str.str, end->str.size) == 0;
	}

	*rv = flamingo_val_make_bool(endswith);

	return 0;
}

static int vec_len(flamingo_t* flamingo, flamingo_val_t* self, flamingo_arg_list_t* args, flamingo_val_t** rv) {
	assert(self->kind == FLAMINGO_VAL_KIND_VEC);

	if (args->count != 0) {
		return error(flamingo, "'vec.len' expected 0 arguments, got %zu", args->count);
	}

	*rv = flamingo_val_make_int(self->vec.count);
	return 0;
}

static int primitive_type_member_std(flamingo_t* flamingo) {
#define ADD(type, key, cb)                                                               \
	do {                                                                                  \
		if (primitive_type_member_add(flamingo, (type), strlen((key)), (key), (cb)) < 0) { \
			return -1;                                                                      \
		}                                                                                  \
	} while (0)

	ADD(FLAMINGO_VAL_KIND_STR, "len", str_len);
	ADD(FLAMINGO_VAL_KIND_STR, "endswith", str_endswith);
	ADD(FLAMINGO_VAL_KIND_STR, "startswith", str_startswith);

	ADD(FLAMINGO_VAL_KIND_VEC, "len", vec_len);

	return 0;
}
