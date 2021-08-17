#include "vita/container/array2d.h"

array2d_pt array2d(const size_t elsize) {
	return array2d_new(7, 7, elsize);
}

array2d_pt array2d_new(const size_t rows, const size_t cols, const size_t elsize) {
	// create an array instance
	array2d_pt arr = ((array_manual_collect_status()) ? 
		(mem_malloc(1, sizeof(struct BaseArrayType))) : 
		(memhandler_malloc(array_memhandler_internal(), 1, sizeof(struct BaseArrayType)))
	);

	// checking if array was allocated
	if(is_null(arr)) {
		logger_error(str("unable to create an array2d!"), str("array2d_new"));

		if(!array_manual_collect_status()) {
			logger_info(str("ManualCollect is false? Then don't forget to initialize the module\'s internal memhandler."));
		}

		return NULL;
	}

	// allocate memory for array elements
	*arr = (struct BaseArrayType) {
		.ptr = ((array_manual_collect_status()) ? 
			(mem_calloc(rows*cols, elsize)) : 
			(memhandler_calloc(array_memhandler_internal(), rows*cols, elsize))),
		.len = rows,		// NOTE: here len and capacity are used as rows and cols
		.capacity = cols,
		.elsize = elsize
	};

	// check if array elements were allocated
	if(is_null(arr->ptr)) {
		logger_error(str("unable to allocate memory for array2d elements!"), str("array2d_new"));
		return NULL;
	}

	return arr;
}

array2d_pt array2d_dup(const array2d_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("the provided array2d is NULL! returning NULL..."), str("array2d_dup"));
		return NULL;
	}

	// create a new array
	array2d_pt arrdup = array2d_new(arr->len, arr->capacity, arr->elsize);
	if(is_null(arrdup)) {
		logger_warn(str("failed to create an array2d duplicate!"), str("array2d_dup"));
		return NULL;
	}

	// copy array elements
	memcpy(arrdup->ptr, arr->ptr, (arr->len * arr->capacity * arr->elsize));

	return arrdup;
}

void array2d_free(array2d_pt arr) {
	array_free(arr);
}







size_t array2d_rows(const array2d_pt arr) {
	return array_len(arr);
}

size_t array2d_cols(const array2d_pt arr) {
	return array_capacity(arr);
}

size_t array2d_size(const array2d_pt arr) {
	if(is_null(arr)) {
		return 0;
	} else {
		return (arr->len * arr->capacity);
	}
}







bool array2d_resize(array2d_pt arr, const size_t rows, const size_t cols) {
	if(is_null(arr)) {
		logger_warn(str("array2d is NULL; exiting..."), str("array2d_resize"));
		return false;
	}

	if(arr->len == rows || arr->capacity == cols) {
		return true;
	}

	// reallocate memory
	bool success = array_resize(arr, rows*cols);
	if(!success) {
		logger_error(str("unable to resize array2d!"), str("array2d_resize"));
	} else {
		arr->len = rows;
		arr->capacity = cols;
	}

	return success;
}

bool array2d_set(array2d_pt arr, const size_t row, const size_t col, const void* valptr) {
	if(is_null(arr)) {
		logger_warn(str("array2d is NULL; exiting..."), str("array2d_set"));
		return false;
	}

	if(is_null(valptr)) {
		logger_warn(str("value that should be assigned is NULL; exiting..."), str("array2d_set"));
		return false;
	}

	if(row >= arr->len || col >= arr->capacity) {
		logger_warn(str("out-of-bounds array2d access; exiting..."), str("array2d_set"));
		return false;
	}

	// set the value
	memcpy((arr->ptr + (row * arr->capacity + col) * arr->elsize), valptr, arr->elsize);

	return true;
}

void* array2d_get(const array2d_pt arr, const size_t row, const size_t col) {
	if(is_null(arr)) {
		logger_warn(str("array2d is NULL; returning NULL..."), str("array2d_get"));
		return NULL;
	}

	if(row >= arr->len || col >= arr->capacity) {
		logger_warn(str("out-of-bounds array2d access; returning NULL..."), str("array2d_get"));
		return NULL;
	}

	return (arr->ptr + (row * arr->capacity + col) * arr->elsize);
}

void array2d_clear(array2d_pt arr) {
	if(is_null(arr)) {
		logger_warn(str("array2d is NULL; exiting..."), str("array2d_clear"));
	} else {
		// set byte values to zero
		memset(arr->ptr, 0, arr->len * arr->capacity * arr->elsize);
	}
}

void array2d_foreach(const array2d_pt arr, void (*func)(void* ptr)) {
	if(is_null(arr)) {
		logger_warn(str("array2d is NULL; exiting..."), str("array2d_foreach"));
	}

	if(is_null(func)) {
		logger_warn(str("func is NULL; exiting..."), str("array2d_foreach"));
	}

	for(size_t i = 0; i < arr->len; i++) {
		for(size_t j = 0; j < arr->capacity; j++) {
			func(arr->ptr + (i * arr->capacity + j) * arr->elsize);
		}
	}
}

































