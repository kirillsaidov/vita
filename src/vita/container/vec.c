#include "vita/container/vec.h"

vec_t *vec_new(void) {
	vec_t *v = malloc(sizeof(vec_t));
	if(is_null(v)) {
		return NULL;
	}

	// default-init
	*v = (vec_t) {};

	return v;
}

enum ContainerError vec_ctor(vec_t *const v, const size_t n, const size_t elsize) {
	// check if v was allocated
	if(is_null(v)) {		
		return ce_container_is_null;
	}

	// vec_t init
	*v = (vec_t) {
		.ptr = calloc(n, elsize),
		.len = 0,
		.capacity = n,
		.elsize = elsize,
	};

	// checking if v->ptr was allocated
	if(is_null(v->ptr)) {
		return ce_error_allocation;
	}

	return ce_operation_success;
}

vec_t *vec_dup(const vec_t *const v) {
	if(is_null(v)) {
		return NULL;
	}

	// allocate a new vec_t instance
	vec_t *vdup = vec_new();
	if(is_null(vdup)) {
		return NULL;
	}

	// construct vdup and resize it
	if(vec_ctor(vdup, v->capacity, v->elsize) != ce_operation_success || vec_resize(vdup, v->len) != ce_operation_success) {
		vec_free(vdup);
		return NULL;
	}

	// copy values
	memcpy(vdup->ptr, v->ptr, v->len * v->elsize);

	return vdup;
}

void vec_dtor(vec_t *const v) {
	// if NULL, exit
	if(is_null(v)) {
		return;
	}

	// free vec_t contents
	free(v->ptr);

	// default-init
	*v = (vec_t) {};
}

void vec_free(vec_t *v) {
	// if NULL, exit
	if(is_null(v)) {
		return;
	}

	// free the vec_t
	free(v);

	// reset to NULL
	v = NULL;
}







vec_t *vec_create(const size_t n, const size_t elsize) {
	vec_t *v = vec_new();
	if(is_null(v)) {
		return NULL;
	}

	if(vec_ctor(v, n, elsize) != ce_operation_success) {
		vec_free(v);
		return NULL;
	}

	return v;
}

void vec_destroy(vec_t *v) {
	vec_dtor(v);
	vec_free(v);
}







size_t vec_len(const vec_t *const v) {
	return v->len;
}

size_t vec_capacity(const vec_t *const v) {
	return v->capacity;
}

size_t vec_has_space(const vec_t *const v) {
	return (v->capacity - v->len);
}

bool vec_is_empty(const vec_t *const v) {
	return !(v->len);
}







enum ContainerError vec_shrink(vec_t *const v) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	// if length and capacity are the same, exit the function
	if(v->len == v->capacity) {
		return ce_operation_success;
	}

	// shrink the array capacity to length
	void *newptr = realloc(v->ptr, v->len * v->elsize);
	if(is_null(newptr)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return ce_error_allocation;
	}

	// update values
	v->ptr = newptr;
	v->capacity = v->len;

	return ce_operation_success;
}

enum ContainerError vec_clear(vec_t *const v) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	// update length
	v->len = 0;

	return ce_operation_success;
}

enum ContainerError vec_reserve(vec_t *const v, const size_t n) {
	if(is_null(v) || !n) {
		return ce_container_is_null;
	}

	// reserve memory for additional n elements
	void *newptr = realloc(v->ptr, (v->capacity + n) * v->elsize);
	if(is_null(newptr)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return ce_error_allocation;
	}

	// update values
	v->ptr = newptr;
	v->capacity += n;

	return ce_operation_success;
}

enum ContainerError vec_resize(vec_t *const v, const size_t n) {
	if(is_null(v)) { // think about resizing to 0
		return ce_container_is_null;
	}

	if(n == v->capacity) {
		v->len = v->capacity;
		return ce_operation_success;
	}

	// resize vec_t
	void *newptr = realloc(v->ptr, n * v->elsize);
	if(is_null(newptr)) {
		return ce_error_allocation;
	}

	// update values
	v->ptr = newptr;
	v->len = v->capacity = n;

	return ce_operation_success;
}

enum ContainerError vec_push(vec_t *const v, const void *val) {
	if(is_null(v) || is_null(val)) {
		return ce_container_is_null;
	}

	// check if new memory needs to be allocated
	if(!vec_has_space(v) && vec_reserve(v, (size_t)(v->capacity * CONTAINER_GROWTH_RATE + 1)) != ce_operation_success) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return ce_error_allocation;
	}

	// copy val to vec_t
	memcpy((v->ptr + v->len++ * v->elsize), val, v->elsize);

	return ce_operation_success;
}

enum ContainerError vec_pushi32(vec_t *const v, const int val) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int32_t>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_push(v, &val);
}

enum ContainerError vec_pushi64(vec_t *const v, const long val) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int64_t>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_push(v, &val);
}

enum ContainerError vec_pushf(vec_t *const v, const float val) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <float>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_push(v, &val);
}

enum ContainerError vec_pushd(vec_t *const v, const double val) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <double>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_push(v, &val);
}

enum ContainerError vec_pop(vec_t *const v) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	// update length
	v->len--;

	return ce_operation_success;
}

enum ContainerError vec_set(vec_t *const v, const void *val, const size_t at) {
	if(is_null(v) || is_null(val)) {
		return ce_container_is_null;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	// copy val data to str_t
	memcpy((v->ptr + at * v->elsize), val, v->elsize);

	return ce_operation_success;
}

enum ContainerError vec_seti32(vec_t *const v, const int val, const size_t at) {
	if(is_null(v)) {
		return false;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int32_t>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_set(v, &val, at);
}

enum ContainerError vec_seti64(vec_t *const v, const long val, const size_t at) {
	if(is_null(v)) {
		return false;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int64_t>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_set(v, &val, at);
}

enum ContainerError vec_setf(vec_t *const v, const float val, const size_t at) {
	if(is_null(v)) {
		return false;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <float>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_set(v, &val, at);
}

enum ContainerError vec_setd(vec_t *const v, const double val, const size_t at) {
	if(is_null(v)) {
		return false;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <double>! Aborting operation.", __FUNCTION__);
		return ce_error_wrong_datatype;
	}

	return vec_set(v, &val, at);
}

void *vec_get(const vec_t *const v, const size_t at) {
	if(is_null(v) || !(at < v->len)) {
		return NULL;
	}

	return (v->ptr + at * v->elsize);
}

int32_t vec_geti32(const vec_t *const v, const size_t at) {
	return *(int32_t*)(vec_get(v, at));
}

int64_t vec_geti64(const vec_t *const v, const size_t at) {
	return *(int64_t*)(vec_get(v, at));
}

float vec_getf(const vec_t *const v, const size_t at) {
	return *(float*)(vec_get(v, at));
}

double vec_getd(const vec_t *const v, const size_t at) {
	return *(double*)(vec_get(v, at));
}

enum ContainerError vec_insert(vec_t *const v, const void *val, const size_t at) {
	if(is_null(v) || is_null(val)) {
		return ce_container_is_null;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	// check if new memory needs to be allocated
	if(!vec_has_space(v) && !vec_reserve(v, v->capacity * CONTAINER_GROWTH_RATE + 1)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return ce_error_allocation;
	}

	// shift values by one value to the end of the vec_t
	memmove((v->ptr + (at + 1) * v->elsize), (v->ptr + at * v->elsize), ((v->len - at) * v->elsize));

	// copy the str contents to str from the specified index
	memcpy((v->ptr + at * v->elsize), val, v->elsize);

	// set new length
	v->len++;

	return ce_operation_success;
}

enum ContainerError vec_remove(vec_t *const v, const size_t at, const enum RemoveStrategy rs) {
	if(is_null(v)) {
		return ce_container_is_null;
	}

	if(!(at < v->len)) {
		return ce_error_out_of_bounds_access;
	}

	// check remove strategy
	if(rs == rs_stable) {
		memmove(v->ptr + at * v->elsize, v->ptr + (at + 1) * v->elsize, (v->len - at) * v->elsize);
	} else {
		gswap(v->ptr + at * v->elsize, v->ptr + (v->len - 1) * v->elsize, v->elsize);
	}

	// set new length
	v->len--;

	return ce_operation_success;
}

int64_t vec_contains(const vec_t *const v, const void *val) {
	if(is_null(v) || is_null(val)) {
		return -1;
	}

	size_t i = 0;
	for (void *iter = v->ptr; iter != v->ptr + v->len * v->elsize; iter += v->elsize, i++) {
		if(memcmp(iter, val, v->elsize) == 0) {
			return i;
		}
	}

	return -1;
}

void vec_foreach(const vec_t *const v, void (*func)(void*, size_t)) {
	if(is_null(v) || is_null(func)) {
		return;
	}

	size_t i = 0;
	for (void *iter = v->ptr; iter != v->ptr + v->len * v->elsize; iter += v->elsize, i++) {
		func(iter, i);
	}
}




























