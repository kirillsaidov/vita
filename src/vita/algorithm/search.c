#include "vita/algorithm/search.h"

int64_t search_linear(void* arr, const size_t len, const size_t elsize, const void* val, bool (*compare)(const void* a, const void* b)) {
	if(is_null(arr) || is_null(val)) {
		return -1;
	}

	// preparation
	size_t index = 0;
	const void* end = arr + len * elsize;

	// if the element that is sought for is found, break from the loop
	for(void* current = arr; current != end; current += elsize, index++) {
		if(is_null(compare) ? (memcmp(current, val, elsize) == 0) : (compare(current, val))) {
			return index;
		}
	}

	return -1;
}







int64_t search_binary(const int value, const int* array, const size_t length) {
	// variables
	size_t start = 0;
	size_t end = length-1;
	
	// binary search
	while(start <= end) {
		// find the middle index
		int64_t index = start + (end - start)/2;
		
		// check if the ptr we are looking for is present at index
		if(array[index] == value) {
			return index;
		} else if(array[index] < value) { 	// if ptr is on the right hand side of the array, ignore the left half
			start = index + 1;
		} else { 							// if ptr is on the left hand side of the array, ignore the right half
			end = index - 1;
		}
	}
	
	return -1;
}

int64_t search_binary_ptr(const void* ptr, const void** list, const size_t length) {
	// variables
	size_t start = 0;
	size_t end = length-1;
	
	// binary search
	while(start <= end) {
		// find the middle index
		int64_t index = start + (end - start)/2;
		
		// check if the ptr we are looking for is present at index
		if(list[index] == ptr) {
			return index;
		} else if(list[index] < ptr) {	// if ptr is on the right hand side of the list, ignore the left half
			start = index + 1;
		} else { 						// if ptr is on the left hand side of the list, ignore the right half
			end = index - 1;
		}
	}
	
	return -1;
}






































