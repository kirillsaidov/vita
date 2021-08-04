#include "vita/container/array.h"

// array struct
struct Array {
	void* ptr;

	size_t len;
	size_t capacity;
	size_t elsize;
};

// array internal memory handler
static memhandler_pt arrayMemhandlerInternal = NULL;

// array memory management strategy
static bool arrayManualCollect = false;

void array_manual_collect(const bool status) {
	arrayManualCollect = status;
}







bool array_memhandler_internal_create(void) {
	arrayMemhandlerInternal = ((is_null(arrayMemhandlerInternal)) ? (memhandler_create()) : (arrayMemhandlerInternal));
	if(is_null(arrayMemhandlerInternal)) {
		return false;
	}

	return true;
}

void array_memhandler_internal_destroy(void) {
	if(!is_null(arrayMemhandlerInternal)) {
		memhandler_destroy(arrayMemhandlerInternal);
	}
}

const memhandler_pt array_memhandler_internal(void) {
	return arrayMemhandlerInternal;
}







array_pt array(const size_t elsize) {
	return array_new(9, elsize);
}

array_pt array_new(const size_t n, const size_t elsize) {
	// create an array instance
	array_pt arr = ((arrayManualCollect) ? 
		(mem_malloc(1, sizeof(struct Array))) : 
		(memhandler_malloc(arrayMemhandlerInternal, 1, sizeof(struct Array)))
	);

	// checking if array was allocated
	if(is_null(arr)) {
		logger_error(str("unable to create an array!"), str("array_new"));
		return NULL;
	}

	// allocate memory for array elements
	*arr = (struct Array) {
		.ptr = ((arrayManualCollect) ? 
			(mem_calloc((n + n/3), elsize)) : 
			(memhandler_calloc(arrayMemhandlerInternal, (n + n/3), elsize))),
		.len = 0,
		.capacity = n + n/3,
		.elsize = elsize
	};

	// check if array elements were allocated
	if(is_null(arr->ptr)) {
		logger_error(str("unable to allocate memory for array elements!"), str("array_new"));
		return NULL;
	}

	return arr;
}

array_pt array_dup(const array_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("the provided array is NULL! returning NULL..."), str("array_dup"));
		return NULL;
	}

	// create a new array
	array_pt arrdup = array_new(arr->len, arr->elsize);
	if(is_null(arrdup)) {
		logger_warn(str("failed to create an array duplicate instance!"), str("array_dup"));
		return NULL;
	}

	// copy array elements
	memcpy(arrdup->ptr, arr->ptr, (arr->len * arr->elsize));
	arrdup->len = arr->len;

	return arrdup;
}

void array_free(array_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL, safely exiting..."), str("array_free"));
		return;
	}

	// free the array elements and array instance
	if(arrayManualCollect) {
		mem_free(arr->ptr);
		mem_free(arr);
	} else {
		memhandler_free(arrayMemhandlerInternal, arr->ptr);
		memhandler_free(arrayMemhandlerInternal, arr);
	}
}







size_t array_len(const array_pt arr) {
	return arr->len;
}

size_t array_capacity(const array_pt arr) {
	return arr->capacity;
}

size_t array_has_space(const array_pt arr) {
	return (arr->capacity - arr->len);
}







bool array_shrink(array_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_shrink"));
		return false;
	}

	// if length and capacity are the same, exit the function
	if(arr->len == arr->capacity) {
		return true;
	}

	// shrink the array capacity to length
	bool success = ((arrayManualCollect) ? 
		(mem_realloc((void**)&arr->ptr, arr->len, arr->elsize)) : 
		(memhandler_realloc(arrayMemhandlerInternal, (void**)&arr->ptr, arr->len, arr->elsize))
	);
	

	// check if the operation was successfull
	if(!success) {
		logger_error(str("unable to shrink the array!"), str("array_shrink"));
	} else {
		arr->capacity = arr->len;
	}

	return success;
}

bool array_reserve(array_pt arr, const size_t n) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_reserve"));
		return false;
	}

	// reserve memory for additional n elements
	bool success = ((arrayManualCollect) ? 
		(mem_realloc((void**)&arr->ptr, (arr->capacity + n), arr->elsize)) : 
		(memhandler_realloc(arrayMemhandlerInternal, (void**)&arr->ptr, (arr->capacity + n), arr->elsize))
	);
	
	// check if the operation was successfull
	if(!success) {
		logger_error(str("cannot reserve memory for array!"), str("array_reserve"));
	} else {
		// set byte values to zero
		memset(arr->ptr + arr->elsize * arr->capacity, 0, n * arr->elsize);

		// update the capacity
		arr->capacity += n;
	}

	return success;
}

bool array_resize(array_pt arr, const size_t n) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_resize"));
		return false;
	}

	if(arr->capacity == n) {
		arr->len = n;
		return true;
	}

	// reallocate memory
	bool success = ((arrayManualCollect) ? 
		(mem_realloc((void**)&arr->ptr, n, arr->elsize)) : 
		(memhandler_realloc(arrayMemhandlerInternal, (void**)&arr->ptr, n, arr->elsize))
	);
	

	// check if the operation was successfull
	if(!success) {
		logger_error(str("unable to shrink the array!"), str("array_resize"));
	} else {
		if(arr->capacity < n) {
			// set byte values to zero
			memset(arr->ptr + arr->elsize * arr->capacity, 0, (n - arr->capacity) * arr->elsize);
		}

		// update capacity and array length
		arr->capacity = arr->len = n;
	}

	return success;
}

int64_t array_find(const array_pt arr, const void* valptr, bool (*compare)(const void* ptr, const void* valptr)) {
	int64_t index = -1;

	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_find"));
		return index;
	}

	if(is_null(valptr)) {
		logger_warn(str("value that is sought after is NULL; exiting..."), str("array_find"));
		return index;
	}


	index = 0;
	const size_t step = arr->elsize;
	const void* end = arr->ptr + arr->len * step;

	// if the element that is sought for is found, break from the loop
	for(void* ptr = arr->ptr; ptr != end; ptr += step, index++) {
		if(is_null(compare) ? (memcmp(ptr, valptr, step) == 0) : (compare(ptr, valptr))) {
			break;
		}
	}

	return index;
}

bool array_push(array_pt arr, const void* valptr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_push"));
		return false;
	}

	if(is_null(valptr)) {
		logger_warn(str("value that should be pushed is NULL; exiting..."), str("array_push"));
		return false;
	}

	// check if we have enough memory 
	if(!((arr->len >= arr->capacity) && array_reserve(arr, arr->len))) {
		logger_warn(str("unable to reserve memory to push a value; exiting..."), str("array_push"));
		return false;
	}

	// push the value
	memcpy(arr->ptr + arr->elsize * arr->len++, valptr, arr->elsize);

	return true;
}

void array_pop(array_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_pop"));
	} else {
		arr->len--;
	}
}

void array_set(array_pt arr, const size_t index, const void* valptr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_set"));
		return;
	}

	if(index >= arr->len) {
		logger_warn(str("out-of-bounds array access; exiting..."), str("array_set"));
		return;
	}

	if(is_null(valptr)) {
		logger_warn(str("value that should be assigned is NULL; exiting..."), str("array_set"));
		return;
	}

	// set the value
	memcpy(arr->ptr + arr->elsize * index, valptr, arr->elsize);
}

void* array_get(array_pt arr, const size_t index) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; returning NULL..."), str("array_get"));
		return NULL;
	}

	if(index >= arr->len) {
		logger_warn(str("out-of-bounds array access; returning NULL..."), str("array_get"));
		return NULL;
	}

	return (arr->ptr + arr->elsize * index);
}

void array_insert(array_pt arr, const size_t index, const void* valptr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_insert"));
		return;
	}

	if(is_null(valptr)) {
		logger_warn(str("value that should be assigned is NULL; exiting..."), str("array_insert"));
		return;
	}

	// check if we need to allocate additional memory
	if(index >= arr->capacity) {
		array_resize(arr, (index + 1));

		// insert the value at the end
		memcpy((arr->ptr + arr->elsize * index), valptr, arr->elsize);
	} else if(arr->len < arr->capacity && index >= arr->len) {
		arr->len += index - arr->len + 1;

		// insert the value at the end
		memcpy((arr->ptr + arr->elsize * index), valptr, arr->elsize);
	} else { // if(arr->len < arr->capacity && index < arr->len)
		arr->len += 1;

		// copy values to the end of the array
		memcpy((arr->ptr + arr->elsize * (index + 1)), (arr->ptr + arr->elsize * index), (arr->len - index - 1) * arr->elsize);
		
		// insert the value at index position
		memcpy((arr->ptr + arr->elsize * index), valptr, arr->elsize);
	}
}

void array_remove(array_pt arr, const size_t index) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_remove"));
		return;
	}
	
	if(index >= arr->len) {
		logger_warn(str("out-of-bounds array access; exiting..."), str("array_remove"));
		return;
	}

	gswap((arr->ptr + index * arr->elsize), (arr->ptr + (arr->len - 1) * arr->elsize), arr->elsize);
	arr->len -= 1;
}

void array_clear(array_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("array is NULL; exiting..."), str("array_clear"));
	} else {
		// set byte values to zero
		memset(arr->ptr, 0, arr->capacity * arr->elsize);
	}
}








































