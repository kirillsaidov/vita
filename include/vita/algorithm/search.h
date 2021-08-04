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

/** search_linear_type ==> linear search
	params:
		const int value
		const int* array
		const size_t length
	returns:
		uint64_t index 	: if value was found
		-1 				: if value was not found
*/
int64_t search_linear(const int value, const int* array, const size_t length);
int64_t search_linear_ptr(const void* ptr, const void** list, const size_t length);







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
























