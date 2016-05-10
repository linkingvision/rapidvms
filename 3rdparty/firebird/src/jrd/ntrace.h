/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ntrace.h
 *	DESCRIPTION:	Trace API header
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *  2008 Khorsun Vladyslav
 *
 */

#ifndef FIREBIRD_NTRACE_H
#define FIREBIRD_NTRACE_H

#include "firebird/Interface.h"

const int DBB_max_rel_count = 8; // must be the same as DBB_max_count from jrd.h

// Performance counters for entire database

enum {
	DBB_fetches_count = 0,
	DBB_reads_count,
	DBB_marks_count,
	DBB_writes_count,
	DBB_max_dbb_count
};

// Performance counters for individual table
typedef int ntrace_relation_t;
struct TraceCounts
{
	ntrace_relation_t	trc_relation_id;	// Relation ID
	const char*			trc_relation_name;	// Relation name
	const ISC_INT64*	trc_counters;	    // Pointer to allow easy addition of new counters
};

// Performance statistics for operation
struct PerformanceInfo
{
	ISC_INT64 pin_time;				// Total operation time in milliseconds
	ISC_INT64* pin_counters;		// Pointer to allow easy addition of new counters

	size_t pin_count;				// Number of relations involved in analysis
	struct TraceCounts* pin_tables; // Pointer to array with table stats

	ISC_INT64 pin_records_fetched;	// records fetched from statement/procedure
};

typedef unsigned int ntrace_result_t;
typedef unsigned char ntrace_byte_t;
typedef ISC_UINT64 ntrace_counter_t;
typedef unsigned int ntrace_process_state_t;
typedef ISC_UINT64 ntrace_mask_t;

#endif	// FIREBIRD_NTRACE_H
