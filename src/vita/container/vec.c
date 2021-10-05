#include "vita/container/vec.h"

vec_t *vec(const size_t n, const size_t elsize) {
	// allocate memory for a vec_t struct
	vec_t *v = malloc(sizeof(vec_t));

	// check if v was allocated
	if(is_null(v)) {		
		return NULL;
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
		free(v);
		return NULL;
	}

	return v;
}

vec_t *vec_dup(const vec_t *const v) {
	if(is_null(v)) {
		return NULL;
	}

	// create a new vec_t instance
	vec_t *vdup = vec(v->len, v->elsize);
	if(is_null(vdup)) {
		return NULL;
	}

	// resize its length to v->len
	if(!vec_resize(vdup, v->len)) {
		vec_free(vdup);
		return NULL;
	}

	// copy values
	memcpy(vdup->ptr, v->ptr, v->len * v->elsize);

	return vdup;
}

void vec_free(vec_t *v) {
	// if NULL, exit
	if(is_null(v)) {
		return;
	}

	// free the vec_t string and vec_t struct
	free(v->ptr);
	free(v);

	// reset to NULL
	v = NULL;
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







bool vec_shrink(vec_t *const v) {
	if(is_null(v)) {
		return false;
	}

	// if length and capacity are the same, exit the function
	if(v->len == v->capacity) {
		return true;
	}

	// shrink the array capacity to length
	void *newptr = realloc(v->ptr, v->len * v->elsize);
	if(is_null(newptr)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// update values
	v->ptr = newptr;
	v->capacity = v->len;

	return true;
}

bool vec_clear(vec_t *const v) {
	if(is_null(v)) {
		return false;
	}

	// update length
	v->len = 0;

	return true;
}

bool vec_reserve(vec_t *const v, const size_t n) {
	if(is_null(v) || !n) {
		return false;
	}

	// reserve memory for additional n elements
	void *newptr = realloc(v->ptr, (v->capacity + n) * v->elsize);
	if(is_null(newptr)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// update values
	v->ptr = newptr;
	v->capacity += n;

	return true;
}

bool vec_resize(vec_t *const v, const size_t n) {
	if(is_null(v)) { // think about resizing to 0
		return false;
	}

	if(n == v->capacity) {
		v->len = v->capacity;
		return true;
	}

	// resize vec_t
	void *newptr = realloc(v->ptr, n * v->elsize);
	if(is_null(newptr)) {
		return false;
	}

	// update values
	v->ptr = newptr;
	v->len = v->capacity = n;

	return true;
}

bool vec_push(vec_t *const v, const void *val) {
	if(is_null(v) || is_null(val)) {
		return false;
	}

	// check if new memory needs to be allocated
	if(!vec_has_space(v) && !vec_reserve(v, (size_t)(v->capacity * VEC_GROWTH_RATE + 1))) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// copy val to vec_t
	memcpy((v->ptr + v->len++ * v->elsize), val, v->elsize);

	return true;
}

bool vec_pushi32(vec_t *const v, const int val) {
	if(is_null(v)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int32_t>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_push(v, &val);
}

bool vec_pushi64(vec_t *const v, const long val) {
	if(is_null(v)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int64_t>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_push(v, &val);
}

bool vec_pushf(vec_t *const v, const float val) {
	if(is_null(v)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <float>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_push(v, &val);
}

bool vec_pushd(vec_t *const v, const double val) {
	if(is_null(v)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <double>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_push(v, &val);
}

bool vec_pop(vec_t *const v) {
	if(is_null(v)) {
		return false;
	}

	// update length
	v->len--;

	return true;
}

bool vec_set(vec_t *const v, const void *val, const size_t at) {
	if(is_null(v) || is_null(val) || !(at < v->len)) { // expand out-of-bounds access
		return false;
	}

	// copy val data to str_t
	memcpy((v->ptr + at * v->elsize), val, v->elsize);

	return true;
}

bool vec_seti32(vec_t *const v, const int val, const size_t at) {
	if(is_null(v) || !(at < v->len)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int32_t>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_set(v, &val, at);
}

bool vec_seti64(vec_t *const v, const long val, const size_t at) {
	if(is_null(v) || !(at < v->len)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <int64_t>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_set(v, &val, at);
}

bool vec_setf(vec_t *const v, const float val, const size_t at) {
	if(is_null(v) || !(at < v->len)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <float>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_set(v, &val, at);
}

bool vec_setd(vec_t *const v, const double val, const size_t at) {
	if(is_null(v) || !(at < v->len)) {
		return false;
	}

	if(v->elsize != sizeof(val)) {
		vita_warn("expects <double>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return vec_set(v, &val, at);
}

void* vec_get(const vec_t *const v, const size_t at) {
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

bool vec_insert(vec_t *const v, const void *val, const size_t at) {
	if(is_null(v) || is_null(val) || !(at < v->len)) {
		return false;
	}

	// check if new memory needs to be allocated
	if(!vec_has_space(v) && !vec_reserve(v, v->capacity * VEC_GROWTH_RATE + 1)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// shift values by one value to the end of the vec_t
	memmove((v->ptr + (at + 1) * v->elsize), (v->ptr + at * v->elsize), ((v->len - at) * v->elsize));

	// copy the str contents to str from the specified index
	memcpy((v->ptr + at * v->elsize), val, v->elsize);

	// set new length
	v->len++;

	return true;
}

bool vec_remove(vec_t *const v, const size_t at, const enum RemoveStrategy rs) {
	if(is_null(v) || !(at < v->len)) {
		return false;
	}

	// check remove strategy
	if(rs == rs_stable) {
		memmove(v->ptr + at * v->elsize, v->ptr + (at + 1) * v->elsize, (v->len - at) * v->elsize);
	} else {
		gswap(v->ptr + at * v->elsize, v->ptr + (v->len - 1) * v->elsize, v->elsize);
	}

	// set new length
	v->len--;

	return true;
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




























