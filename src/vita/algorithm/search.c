#include "vita/algorithm/search.h"

int64_t search_linear_ptr(const void* ptr, const void** list, const size_t length) {
	// search for an element iteratively
	for(size_t i = 0; i < length; i++) {
		if(list[i] == ptr) {
			return i;
		}
	}
	
	return -1;
}

int64_t search_linear_int(const int value, const int* array, const size_t length) {
	// search for an element iteratively
	for(size_t i = 0; i < length; i++) {
		if(array[i] == value) {
			return i;
		}
	}
	
	return -1;
}

int64_t search_linear_float(const float value, const float* array, const size_t length) {
	// search for an element iteratively
	for(size_t i = 0; i < length; i++) {
		if(array[i] == value) {
			return i;
		}
	}
	
	return -1;
}

int64_t search_linear_double(const double value, const double* array, const size_t length) {
	// search for an element iteratively
	for(size_t i = 0; i < length; i++) {
		if(array[i] == value) {
			return i;
		}
	}
	
	return -1;
}







int64_t algorithm_search_binary_ptr(const void* ptr, const void** list, const size_t length) {
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

int64_t algorithm_search_binary_int(const int value, const int* array, const size_t length) {
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

int64_t algorithm_search_binary_float(const float value, const float* array, const size_t length) {
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

int64_t algorithm_search_binary_double(const double value, const double* array, const size_t length) {
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





































