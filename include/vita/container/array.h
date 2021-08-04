#ifndef VITA_ARRAY_H
#define VITA_ARRAY_H

/** VITA_ARRAY MODULE
	- array_manual_collect

    - array_memhandler_internal_create
    - array_memhandler_internal_destroy
    - array_memhandler_internal

    - array
	- array_new
	- array_dup
	- array_free

	- array_len
	- array_capacity
	- array_has_space

	- array_shrink
	- array_reserve
	- array_resize
	- array_find
	- array_push
	- array_pop
	- array_set
	- array_get
	- array_insert
	- array_remove
	- array_clear *
*/

#include <stdio.h>
#include <stdint.h>

#include "../core/core.h"
#include "../memory/memhandler.h"

// new array type
typedef struct Array* array_pt;

/** array_manual_collect ==> memory management type: manual or through the array internal memory handler
*/
extern void array_manual_collect(const bool status);







/** array_memhandler_internal_create ==> creates an internal memory handler for array
*/
extern bool array_memhandler_internal_create(void);

/** array_memhandler_internal_destroy ==> destroys the array internal memory handler
*/
extern void array_memhandler_internal_destroy(void);

/** array_memhandler_internal ==> returns the array internal memory handler
*/
extern const memhandler_pt array_memhandler_internal(void);







/** array ==> creates an empty array and allocates 12 elements by default
	params:
		const size_t elsize (element size)
	returns:
		array_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		allocates 10 elements by default (calls array_new)
		initializes array elements to zero
*/
extern array_pt array(const size_t elsize);

/** array_new ==> creates an empty array and allocates N elements
	params:
		const size_t n
		const size_t elsize (element size)
	returns:
		array_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		preallocates n/3 of the requested size
		initializes array elements to zero
*/
extern array_pt array_new(const size_t n, const size_t elsize);

/** array_dup ==> duplicates an array
	params:
		const array_ptr arr
	returns:
		array_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure
*/
extern array_pt array_dup(const array_pt arr);

/** array_free ==> frees the array and its elements
	params:
		array_ptr arr
*/
extern void array_free(array_pt arr);







/** array_len/capacity/has_length ==> returns length, capacity, available space (capacity - length)
	params:
		array_ptr arr
	returns:
		size_t
*/
extern size_t array_len(const array_pt arr);
extern size_t array_capacity(const array_pt arr);
extern size_t array_has_space(const array_pt arr);







/** array_shrink ==> shrinks arrays capacity to array size
	params:
		array_ptr arr
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool array_shrink(array_pt arr);

/** array_reserve ==> reserves additional memory for N elements
	params:
		array_ptr arr
		const size_t n
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool array_reserve(array_pt arr, const size_t n);

/** array_resize ==> resizes array to the specified length 
	params:
		array_ptr arr
		const size_t n
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool array_resize(array_pt arr, const size_t n);

/** array_find ==> looks for a value in an array and returns the array index to that element
	params:
		const array_pt arr
		const void* valptr
		bool (*compare)(const void* ptr, const void* valptr)
	returns:
		int64_t
	notes:
		valid index 	: upon success
		-1				: upon failure

		1. uses linear search method
		2. if `compare` function is NULL, uses `memcmp` instead
*/
extern int64_t array_find(const array_pt arr, const void* valptr, bool (*compare)(const void* ptr, const void* valptr));

/** array_push ==> push value at the end of an array  
	params:
		array_ptr arr
		const void* valptr
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool array_push(array_pt arr, const void* valptr);

/** array_pop ==> pop the last value at the end of an array
	params:
		array_ptr arr
*/
extern void array_pop(array_pt arr);



/** array_set ==> assigns a new array value
	params:
		array_ptr arr
		const size_t index
		const void* valptr
*/
extern void array_set(array_pt arr, const size_t index, const void* valptr);

/** array_get ==> returns the pointer to array value
	params:
		array_ptr arr
		const size_t index
	returns:
		void* ptr
	notes:
		valid ptr 	: upon success
		NULL		: upon failure
*/
extern void* array_get(array_pt arr, const size_t index);

/** array_insert ==> inserts value at an index
	params:
		array_ptr arr
		const size_t index
		const void* valptr
*/
extern void array_insert(array_pt arr, const size_t index, const void* valptr);

/** array_remove ==> removes value from an array
	params:
		array_ptr arr
		const size_t index
*/
extern void array_remove(array_pt arr, const size_t index);

/** array_clear ==> memset everthing to zero
	params:
		array_ptr arr
	notes:
		all elements up to array capacity are set to zero
*/
extern void array_clear(array_pt arr);

#endif // VITA_ARRAY_H


