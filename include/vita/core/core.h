#ifndef VITA_CORE_H
#define VITA_CORE_H

/** VITA_CORE MODULE
	- is_null
	- gswap
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define CONTAINER_GROWTH_RATE 1/3

enum RemoveStrategy {
	rs_stable,	// keep ordering
	rs_fast,	// ordering doesn't matter
	rs_count	// number of elements
};

enum ContainerError {
	ce_error_allocation,			// failed to allocate or reallocate memory
	ce_error_wrong_datatype,		// trying to add different datatypes 
	ce_error_out_of_bounds_access,	// accessing an memory beyond container size
	ce_error_unknown, 				// other errors
	ce_container_is_null,			// attempting to work with non-initialized container
	ce_operation_failure,			// failed to perform an action (find an element)
	ce_operation_success,			// all good
	ce_count						// number of elements
};

// array struct wrapper
struct BaseArrayType {
	union {
		void *ptr;
		void **ptr2;
	};

	union {
		struct {
			size_t len;
			size_t capacity;
		};

		struct {
			size_t rows;
			size_t cols;
		};
	};

	size_t elsize;
};

// set to false to silence warnings and stop aborting the program
extern bool g_vitaWarnings;

/** 
Checks if a pointer is NULL

Params:
	ptr = pointer 

Returns: a boolean 
*/
extern bool is_null(const void* ptr);

/** 
Generic swap

Params:
	a = first value
	b = second value
	elsize = element size

Returns: `true` upon success
*/
extern bool gswap(void* a, void* b, const size_t elsize);

/** 
Prints the specified message to stderr using fprintf

Params:
	msg = error message
	topic = a topic or place (function name) related to msg
*/
extern void vita_warn(const char *const msg, const char *const topic);

#endif // VITA_CORE_H






















