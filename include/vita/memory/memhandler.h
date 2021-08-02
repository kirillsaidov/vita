#ifndef VITA_MEMHANDLER_H
#define VITA_MEMHANDLER_H

/** VITA_MEMHANDLER_H MODULE (memory management)
	- memhandler_create
	- memhandler_destroy
	- memhandler_internal_create
	- memhandler_internal_destroy
	- memhandler_internal

	- memhandler_add
	- memhandler_remove

	- memhandler_malloc
	- memhandler_calloc
	- memhandler_realloc
	- memhandler_free
	
	- memhandler_malloc_2d
	- memhandler_calloc_2d
	- memhandler_realloc_2d
	- memhandler_free_2d
*/

// std includes
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// vita includes
#include "mem.h"
#include "../core/core.h"
#include "../logger/logger.h"
#include "../algorithm/search.h"

/** Memhandler ==> structure for handling memory
    notes:
        it is a custom container of pointers, may be used to free all added memory at once
 */
typedef struct Memhandler* memhandler_pt;

/** memhandler_create ==> creates a memory handler
	returns:
		memhandler_pt	upon success
		NULL			upon failure
*/
extern memhandler_pt memhandler_create(void);

/** memhandler_destroy ==> destroys a memory handler
	params:
		memhandler_pt mh
*/
extern void memhandler_destroy(memhandler_pt mh);

/** memhandler_internal_create ==> creates an internal memory handler
	returns:
		bool
	notes:
		true 	upon success
		false 	upon failure
*/
extern bool memhandler_internal_create(void);

/** memhandler_internal_destroy ==> destroys an internal memory handler
*/
extern void memhandler_internal_destroy(void);

/** memhandler_internal ==> returns a pointer to the internal memory handler
	returns:
		memhandler_pt
	notes:
		to destroy the internal memory handler use `memhandler_destroy(memhandler_internal())`
*/
extern const memhandler_pt memhandler_internal(void);







/** memhandler_add ==> adds a pointer to memory handler
	params:
		memhandler_pt mh
		const void* ptr
	returns:
		bool
	notes:
		true	upon success
		false	upon failure
*/
extern bool memhandler_add(memhandler_pt mh, const void* ptr);

/** memhandler_remove ==> removes a pointer from memory handler
	params:
		memhandler_pt mh
		const void* ptr
	returns:
		bool
	notes:
		true 	upon success
		false	upon failure
*/
extern bool memhandler_remove(memhandler_pt mh, const void* ptr);







/** memhandler_malloc ==> allocates and adds the ptr to memory handler automatically
	params:
		memhandler_pt mh
		const size_t n
		const size_t size
	retuns:
		void* ptr 	to a valid memory address
		NULL 		upon failure
	notes:
		if failes to add new memory to memory handler, frees the memory and returns NULL
*/
extern void* memhandler_malloc(memhandler_pt mh, const size_t n, const size_t size);

/** memhandler_calloc ==> allocates and adds the ptr to memory handler automatically (initializes with zeros)
	params:
		memhandler_pt mh
		const size_t n
		const size_t size
	retuns:
		void* ptr 	to a valid memory address
		NULL 		upon failure
	notes:
		if failes to add new memory to memory handler, frees the memory and returns NULL
*/
extern void* memhandler_calloc(memhandler_pt mh, const size_t n, const size_t size);

/** memhandler_realloc ==> reallocates memory in sync with memory handler
	params:
		memhandler_pt mh
		const void** ptr
		const size_t n
		const size_t size
	retuns:
		bool
	notes:
		true 	upon success
		false	upon failure
*/
extern bool memhandler_realloc(memhandler_pt mh, const void** ptr, const size_t n, const size_t size);

/** memhandler_free ==> frees the ptr and removes it from memory handler automatically
	params:
		memhandler_pt mh
		const void* ptr
	notes:
		if it failes to remove a pointer from memory handler, then it DOES NOT free that pointer
*/
extern void memhandler_free(memhandler_pt mh, const void* ptr);







/** memhandler_malloc_2d ==> allocates a 2d array and adds the ptr to memory handler automatically
	params:
		memhandler_pt mh
		const size_t rows
		const size_t cols
		const size_t size
	retuns:
		void** ptr 	to a valid memory address
		NULL 		upon failure
	notes:
		if failes to add new memory to memory handler, frees the memory and returns NULL
*/
void** memhandler_malloc_2d(memhandler_pt mh, const size_t rows, const size_t cols, const size_t size);

/** memhandler_calloc_2d ==> allocates a 2d array and adds the ptr to memory handler automatically (initializes with zeros)
	params:
		memhandler_pt mh
		const size_t rows
		const size_t cols
		const size_t size
	retuns:
		void** ptr 	to a valid memory address
		NULL 		upon failure
	notes:
		if failes to add new memory to memory handler, frees the memory and returns NULL
*/
void** memhandler_calloc_2d(memhandler_pt mh, const size_t rows, const size_t cols, const size_t size);

/** memhandler_realloc_2d ==> reallocates a 2d array in sync with memory handler
	params:
		memhandler_pt mh
		const void*** ptr
		const size_t rows
		const size_t cols
		const size_t size
	retuns:
		bool
	notes:
		true 	upon success
		false	upon failure

		the function removes ptrs from memory handler, reallocs, add them back to memory handler;
		in case of failure, it adds ptrs back to memory handler
*/
bool memhandler_realloc_2d(memhandler_pt mh, void*** ptr, const size_t rows, const size_t cols, const size_t size);

/** memhandler_free_2d ==> frees the 2d array ptrs and removes them from memory handler automatically
	params:
		memhandler_pt mh
		const void** ptr
	notes:
		if it failes to remove a pointer from memory handler, then it DOES NOT free that pointer
*/
void memhandler_free_2d(memhandler_pt mh, void** ptr);






#endif // VITA_MEMHANDLER_H
















