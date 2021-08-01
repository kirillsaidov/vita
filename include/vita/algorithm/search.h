#ifndef VITA_SEARCH_H
#define VITA_SEARCH_H

/** VITA_SEARCH MODULE
	- search_linear_ptr
	- search_linear_int
	- search_linear_float
	- search_linear_double

	- search_binary_ptr
	- search_binary_int
	- search_binary_float
	- search_binary_double
*/

#include <stdio.h>
#include <stdint.h>

/** search_linear_type ==> linear search
	returns:
		uint54_t index 	: if value was found
		-1 				: if value was not found
*/
int64_t search_linear_ptr(const void* ptr, const void** list, const size_t length);
int64_t search_linear_int(const int value, const int* array, const size_t length);
int64_t search_linear_float(const float value, const float* array, const size_t length);
int64_t search_linear_double(const double value, const double* array, const size_t length);







/** search_binary_type ==> binary search (for sorted arrays)
	returns:
		uint54_t index 	: if value was found
		-1 				: if value was not found
*/
int64_t search_binary_ptr(const void* ptr, const void** list, const size_t length);
int64_t search_binary_int(const int value, const int* array, const size_t length);
int64_t search_binary_float(const float value, const float* array, const size_t length);
int64_t search_binary_double(const double value, const double* array, const size_t length);

#endif // VITA_SEARCH_H
























