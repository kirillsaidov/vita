#ifndef VITA_ALGORITHM_SEARCH_H
#define VITA_ALGORITHM_SEARCH_H

/* VITA_ALGORITHM_SEARCH MODULE
	- algorithm_search_binary_ptr
	- algorithm_search_binary_int
	- algorithm_search_linear_ptr
	- algorithm_search_linear_int
*/

// std includes
#include <stdio.h>
#include <stdint.h>

/* search_binary_ptr ==> looks for a location of a memory address in a sorted list of memory addresses
	params:
		const void* ptr
		const void** list
		const size_t length
	returns:
		int64_t index
	notes:
		-1 => no such element found
*/
extern int64_t algorithm_search_binary_ptr(const void* ptr, const void** list, const size_t length);

/* search_binary_int ==> looks for a location of a value in an sorted array
	params:
		const int value
		const int* array
		const size_t length
	returns:
		int64_t index
	notes:
		-1 => no such element found
*/
extern int64_t algorithm_search_binary_int(const int value, const int* array, const size_t length);

/* search_linear_int ==> looks for a location of a ptr in an unsorted list of memory addresses
	params:
		const void* ptr
		const void** list
		const size_t length
	returns:
		int64_t index
	notes:
		-1 => no such element found
*/
extern int64_t algorithm_search_linear_ptr(const void* ptr, const void** list, const size_t length);

/* search_linear_int ==> looks for a location of a value in an unsorted array
	params:
		const int value
		const int* array
		const size_t length
	returns:
		int64_t index
	notes:
		-1 => no such element found
*/
extern int64_t algorithm_search_linear_int(const int value, const int* array, const size_t length);

#endif // VITA_ALGORITHM_SEARCH_H

















