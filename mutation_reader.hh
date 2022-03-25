/*
 * Copyright (C) 2015-present ScyllaDB
 */

/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#pragma once

#include <vector>

#include <seastar/core/future.hh>
#include <seastar/core/future-util.hh>
#include <seastar/core/do_with.hh>
#include "tracing/trace_state.hh"
#include "readers/flat_mutation_reader.hh"
#include "readers/flat_mutation_reader_v2.hh"
#include "reader_concurrency_semaphore.hh"
#include <seastar/core/io_priority_class.hh>

/// Creates a compacting reader.
///
/// The compaction is done with a \ref mutation_compactor, using compaction-type
/// compaction (`compact_for_sstables::yes`).
///
/// \param source the reader whose output to compact.
///
/// Params \c compaction_time and \c get_max_purgeable are forwarded to the
/// \ref mutation_compactor instance.
///
/// Inter-partition forwarding: `next_partition()` and
/// `fast_forward_to(const dht::partition_range&)` is supported if the source
/// reader supports it
/// Intra-partition forwarding: `fast_forward_to(position_range)` is supported
/// if the source reader supports it
flat_mutation_reader_v2 make_compacting_reader(flat_mutation_reader_v2 source, gc_clock::time_point compaction_time,
        std::function<api::timestamp_type(const dht::decorated_key&)> get_max_purgeable,
        streamed_mutation::forwarding fwd = streamed_mutation::forwarding::no);
