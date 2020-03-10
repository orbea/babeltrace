/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2017-2018 Philippe Proulx <pproulx@efficios.com>
 * Copyright 2013, 2014 Jérémie Galarneau <jeremie.galarneau@efficios.com>
 */

#ifndef BABELTRACE_TRACE_IR_STREAM_CLASS_INTERNAL_H
#define BABELTRACE_TRACE_IR_STREAM_CLASS_INTERNAL_H

#include "common/assert.h"
#include "common/common.h"
#include <babeltrace2/trace-ir/stream-class.h>
#include "lib/object.h"
#include "lib/object-pool.h"
#include "common/macros.h"
#include <glib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "field-class.h"
#include "utils.h"

struct bt_stream_class {
	struct bt_object base;

	/* Owned by this */
	struct bt_value *user_attributes;

	struct {
		GString *str;

		/* NULL or `str->str` above */
		const char *value;
	} name;

	uint64_t id;
	bool assigns_automatic_event_class_id;
	bool assigns_automatic_stream_id;
	bool supports_packets;
	bool packets_have_beginning_default_clock_snapshot;
	bool packets_have_end_default_clock_snapshot;
	bool supports_discarded_events;
	bool supports_discarded_packets;
	bool discarded_events_have_default_clock_snapshots;
	bool discarded_packets_have_default_clock_snapshots;
	struct bt_field_class *packet_context_fc;
	struct bt_field_class *event_common_context_fc;
	struct bt_clock_class *default_clock_class;

	/* Array of `struct bt_event_class *` */
	GPtrArray *event_classes;

	/* Pool of `struct bt_field_wrapper *` */
	struct bt_object_pool packet_context_field_pool;

	bool frozen;
};

BT_HIDDEN
void _bt_stream_class_freeze(const struct bt_stream_class *stream_class);

#ifdef BT_DEV_MODE
# define bt_stream_class_freeze		_bt_stream_class_freeze
#else
# define bt_stream_class_freeze(_sc)
#endif

static inline
struct bt_trace_class *bt_stream_class_borrow_trace_class_inline(
		const struct bt_stream_class *stream_class)
{
	BT_ASSERT_DBG(stream_class);
	return (void *) bt_object_borrow_parent(&stream_class->base);
}

#endif /* BABELTRACE_TRACE_IR_STREAM_CLASS_INTERNAL_H */
