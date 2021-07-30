#ifndef VITA_MEMORY_H
#define VITA_MEMORY_H

/* VITA_MEMORY MODULE
	- memory_malloc
    - memory_malloc_2d
    - memory_calloc
    - memory_calloc_2d
    - memory_realloc
    - memory_realloc_2d
    - memory_free
    - memory_free_2d
	- memory_summary
    - memory_summary_get_allocated
    - memory_summary_get_freed
    - memory_summary_get_inuse
*/

// std includes
#include <stdlib.h>
#include <stdbool.h>

// vita includes
#include "../logger.h"

/* memory_malloc ==> same as malloc, but tracks total number of allocations
	params:
		const size_t n
		const size_t size
	returns:
		void* 
*/
extern void* memory_malloc(const size_t n, const size_t size);

/* memory_malloc_2d ==> same as malloc, but tracks total number of allocations, allocates 2d array
	params:
		const size_t rows
		const size_t cols
		const size_t size
	returns:
		void** 
*/
extern void** memory_malloc_2d(const size_t rows, const size_t cols, const size_t size);

/* memory_calloc ==> same as calloc, but tracks total number of allocations
	params:
		const size_t n
		const size_t size
	returns:
		void* 
*/
extern void* memory_calloc(const size_t n, const size_t size);

/* memory_calloc_2d ==> same as calloc, but tracks total number of allocations, allocates 2d array
	params:
		const size_t rows
		const size_t cols
		const size_t size
	returns:
		void** 
*/
extern void** memory_calloc_2d(const size_t rows, const size_t cols, const size_t size);

/* memory_realloc ==> same as realloc, but tracks total number of allocations
	params:
		void** ptr
		const size_t n
		const size_t size
	returns:
		bool
	notes:
		 true - on success
		false - on failure
*/
extern bool memory_realloc(void** ptr, const size_t n, const size_t size);

/* memory_realloc_2d ==> same as realloc, but tracks total number of allocations, reallocates 2d array
	params:
		void*** ptr
		const size_t rows
		const size_t cols
		const size_t size
	returns:
		bool
	notes:
		 true - on success
		false - on failure
*/
extern bool memory_realloc_2d(void*** ptr, const size_t rows, const size_t cols, const size_t size);

/* memory_free ==> same as free, but tracks total number of allocations and frees
	params:
		void* ptr
	notes:
		if ptr is NULL, safely exits
*/
extern void memory_free(void* ptr);

/* memory_free_2d ==> same as free, but tracks total number of allocations and frees, frees 2d array
	params:
		void** ptr
	notes:
		if ptr is NULL, safely exits
*/
extern void memory_free_2d(void** ptr);

/* memory_summary ==> prints memory footprint:
    - total number of allocations
    - total number of deallocations
    - total memory in use (allocations - deallocatons)
*/
extern void memory_summary(void);

/* memory_summary_get_allocated
    returns:
        size_t total number of allocations
*/
extern size_t memory_summary_get_allocated(void);

/* memory_summary_get_freed
    returns:
        size_t total number of deallocations
*/
extern size_t memory_summary_get_freed(void);

/* memory_summary_get_inuse
    returns:
        size_t total number allocations - deallocations (memory that is still in use)
*/
extern size_t memory_summary_get_inuse(void);

#endif // VITA_MEMORY_H





















