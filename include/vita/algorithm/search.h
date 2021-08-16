#ifndef VITA_SEARCH_H
#define VITA_SEARCH_H

/** VITA_SEARCH MODULE
	- search_linear
	- search_linear_ptr

	- search_binary
	- search_binary_ptr
*/

#include <stdio.h>
#include <stdint.h>

#include "../core/core.h"

/** search_linear ==> generic linear search
	params:
		const void* arr
		const size_t len
		const size_t elsize
		const void* val
		int (*compare)(const void* a, const void* b)
	returns:
		uint64_t index 	: if value was found
		-1 				: if value was not found
	notes:
		use NULL instead of compare function to use the memcmp comparison.
		compare function must return a boolean
*/
int64_t search_linear(void* arr, const size_t len, const size_t elsize, const void* val, int (*compare)(const void* a, const void* b));

/** search_binary ==> generic binary search
	params:
		const void* arr
		const size_t len
		const size_t elsize
		const void* val
		int (*compare)(const void* a, const void* b)
	returns:
		uint64_t index 	: if value was found
		-1 				: if value was not found
	notes:
		use NULL instead of compare function to use built-in memcmp comparison.
		compare function must return: 
			if a == b: 0
			if a < b: -1
*/
int64_t search_binary(void* arr, const size_t len, const size_t elsize, const void* val, int (*compare)(const void* a, const void* b));

#endif // VITA_SEARCH_H
























