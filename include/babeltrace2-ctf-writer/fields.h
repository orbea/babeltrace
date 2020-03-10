/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2010-2019 EfficiOS Inc. and Linux Foundation
 */

#ifndef BABELTRACE2_CTF_WRITER_FIELDS_H
#define BABELTRACE2_CTF_WRITER_FIELDS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bt_ctf_field;
struct bt_ctf_event_class;
struct bt_ctf_event;
struct bt_ctf_field_type;
struct bt_ctf_field_type_enumeration_mapping_iterator;

extern struct bt_ctf_field *bt_ctf_field_create(
		struct bt_ctf_field_type *field_type);

extern struct bt_ctf_field_type *bt_ctf_field_get_type(
		struct bt_ctf_field *field);

extern enum bt_ctf_field_type_id bt_ctf_field_get_type_id(
		struct bt_ctf_field *field);

extern struct bt_ctf_field *bt_ctf_field_copy(struct bt_ctf_field *field);

extern int bt_ctf_field_integer_signed_get_value(
		struct bt_ctf_field *integer_field, int64_t *value);

/* Pre-2.0 CTF writer compatibility */
static inline
int bt_ctf_field_signed_integer_set_value(
		struct bt_ctf_field *integer_field, int64_t *value)
{
	return bt_ctf_field_integer_signed_get_value(integer_field, value);
}

extern int bt_ctf_field_integer_signed_set_value(
		struct bt_ctf_field *integer_field, int64_t value);

extern int bt_ctf_field_integer_unsigned_get_value(
		struct bt_ctf_field *integer_field, uint64_t *value);

extern int bt_ctf_field_integer_unsigned_set_value(
		struct bt_ctf_field *integer_field, uint64_t value);

/* Pre-2.0 CTF writer compatibility */
static inline
int bt_ctf_field_unsigned_integer_get_value(
		struct bt_ctf_field *integer_field, uint64_t *value)
{
	return bt_ctf_field_integer_unsigned_get_value(integer_field, value);
}

extern int bt_ctf_field_floating_point_get_value(
		struct bt_ctf_field *float_field, double *value);

extern int bt_ctf_field_floating_point_set_value(
		struct bt_ctf_field *float_field, double value);

extern struct bt_ctf_field *bt_ctf_field_enumeration_get_container(
		struct bt_ctf_field *enum_field);

extern const char *bt_ctf_field_string_get_value(
		struct bt_ctf_field *string_field);

extern int bt_ctf_field_string_set_value(struct bt_ctf_field *string_field,
		const char *value);

extern int bt_ctf_field_string_append(struct bt_ctf_field *string_field,
		const char *value);

extern int bt_ctf_field_string_append_len(
		struct bt_ctf_field *string_field, const char *value,
		unsigned int length);

extern struct bt_ctf_field *bt_ctf_field_structure_get_field_by_name(
		struct bt_ctf_field *struct_field, const char *name);

/* Pre-2.0 CTF writer compatibility */
static inline
struct bt_ctf_field *bt_ctf_field_structure_get_field(
		struct bt_ctf_field *structure, const char *name)
{
	return bt_ctf_field_structure_get_field_by_name(structure, name);
}

extern struct bt_ctf_field *bt_ctf_field_structure_get_field_by_index(
		struct bt_ctf_field *struct_field, uint64_t index);

extern struct bt_ctf_field *bt_ctf_field_array_get_field(
		struct bt_ctf_field *array_field, uint64_t index);

extern struct bt_ctf_field *bt_ctf_field_sequence_get_field(
		struct bt_ctf_field *sequence_field, uint64_t index);

extern int bt_ctf_field_sequence_set_length(struct bt_ctf_field *sequence_field,
		struct bt_ctf_field *length_field);

extern struct bt_ctf_field *bt_ctf_field_variant_get_field(
		struct bt_ctf_field *variant_field,
		struct bt_ctf_field *tag_field);

extern struct bt_ctf_field *bt_ctf_field_variant_get_current_field(
		struct bt_ctf_field *variant_field);

#ifdef __cplusplus
}
#endif

#endif /* BABELTRACE2_CTF_WRITER_FIELDS_H */
