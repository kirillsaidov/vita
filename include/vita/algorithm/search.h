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
		bool (*compare)(const void* a, const void* b)
	returns:
		uint64_t index 	: if value was found
		-1 				: if value was not found
	notes:
		use NULL instead of compare function to use the direct a == b comparison.
*/
int64_t search_linear(void* arr, const size_t len, const size_t elsize, const void* val, bool (*compare)(const void* a, const void* b));







/** search_binary_type ==> binary search (for sorted arrays)
	params:
		const int value
		const int* array
		const size_t length
	returns:
		uint64_t index 	: if value was found
		-1 				: if value was not found
*/
int64_t search_binary(const int value, const int* array, const size_t length);
int64_t search_binary_ptr(const void* ptr, const void** list, const size_t length);

#endif // VITA_SEARCH_H
























