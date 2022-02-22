#ifndef VITA_VEC_H
#define VITA_VEC_H

/** VITA_VEC MODULE (dynamic array)
	- vec_new
	- vec_ctor
	- vec_dup
	- vec_dtor
	- vec_free
	- vec_create
	- vec_destroy
	- vec_len
	- vec_capacity
	- vec_has_space
	- vec_is_empty
	- vec_shrink
	- vec_clear
	- vec_reserve
	- vec_resize
	- vec_push
	- vec_pop
	- vec_pop_get
	- vec_set
	- vec_get
	- vec_insert
	- vec_remove
	- vec_contains
	- vec_foreach
*/

#include "../core/core.h"

// see core/core.h for definition
typedef struct BaseArrayType vec_t;

/**
Allocates memory for vec_t

Returns: `vec_t*` upon success, `NULL` otherwise
*/
extern vec_t *vec_new(void);

/**
Constructs vec_t

Params:
	v = vec_t instance
	n = number of elements
	elsize = element size

Returns: enum VitaError code
*/
extern enum VitaError vec_ctor(vec_t *const v, const size_t n, const size_t elsize);

/**
Duplicates and returns a new dynamic array

Params:
	v = vec_t instance

Returns: vec_t* instance upon success, `NULL` otherwise
*/
extern vec_t *vec_dup(const vec_t *const v);

/**
Destroys contents of vec_t

Params:
	v = vec_t pointer
*/
extern void vec_dtor(vec_t *const v);

/**
Frees the vec_t instance

Params:
	v = vec_t pointer
*/
extern vec_free(vec_t *v);

/**
Allocates and constructs vec_t

Params:
	n = number of elements
	elsize = element size

Returns: `vec_t*` upon success, `NULL` otherwise
*/
extern vec_t *vec_create(const size_t n, const size_t elsize);

/**
Deallocates and destroys vec_t

Params:
	v = vec_t pointer
*/
extern void vec_destroy(vec_t *v);

/**
Returns vec_t length

Params:
	v = vec_t instance

Returns: vec_t length
*/
extern size_t vec_len(const vec_t *const v);

/**
Returns vec_t capacity

Params:
	v = vec_t instance

Returns: vec_t capacity
*/
extern size_t vec_capacity(const vec_t *const v);

/**
Returns available space before new allocation is required

Params:
	v = vec_t instance

Returns: free space (capacity - length)
*/
extern size_t vec_has_space(const vec_t *const v);

/**
Checks if string is emty ("")

Params:
	v = vec_t instance

Returns: `true` if length == 0
*/
extern bool vec_is_empty(const vec_t *const v);

/**
Shrinks vec_t capacity to its length

Params:
	v = vec_t instance

Returns: enum VitaError code
*/
extern enum VitaError vec_shrink(vec_t *const v);

/**
Clears the vec_t (sets length to 0)

Params:
	v = vec_t instance

Returns: enum VitaError code
*/
extern enum VitaError vec_clear(vec_t *const v);

/**
Reserves memory for vec_t

Params:
	v = vec_t instance
	n = how many elements to reserve

Returns: enum VitaError code
*/
extern enum VitaError vec_reserve(vec_t *const v, const size_t n);

/**
Resizes vec_t length

Params:
	v = vec_t instance
	n = new size

Returns: enum VitaError code
*/
extern enum VitaError vec_resize(vec_t *const v, const size_t n);

/**
Push an element at the end of vec_t

Params:
	v = vec_t instance
	val = value to push

Returns: enum VitaError code
*/
extern enum VitaError vec_push(vec_t *const v, const enum VitaError *val);
extern enum VitaError vec_pushi32(vec_t *const v, const int val);
extern enum VitaError vec_pushi64(vec_t *const v, const long val);
extern enum VitaError vec_pushf(vec_t *const v, const float val);
extern enum VitaError vec_pushd(vec_t *const v, const double val);

/**
Pops off the last element

Params:
	v = vec_t instance

Returns: enum VitaError code
*/
extern enum VitaError vec_pop(vec_t *const v);

/**
Pops off and returns the last element

Params:
	v = vec_t instance

Returns: void*
*/
extern void *vec_pop_get(vec_t *const v);

/**
Assigns a new value at an index

Params:
	v = vec_t instance
	val = value
	at = index to set the value

Returns: enum VitaError code
*/
extern enum VitaError vec_set(vec_t *const v, const enum VitaError *val, const size_t at);
extern enum VitaError vec_seti32(vec_t *const v, const int val, const size_t at);
extern enum VitaError vec_seti64(vec_t *const v, const long val, const size_t at);
extern enum VitaError vec_setf(vec_t *const v, const float val, const size_t at);
extern enum VitaError vec_setd(vec_t *const v, const double val, const size_t at);

/**
Returns value at index

Params:
	v = vec_t instance
	at = index

Returns: value (depends on data type)
*/
extern void* vec_get(const vec_t *const v, const size_t at);
extern int32_t vec_geti32(const vec_t *const v, const size_t at);
extern int64_t vec_geti64(const vec_t *const v, const size_t at);
extern float vec_getf(const vec_t *const v, const size_t at);
extern double vec_getd(const vec_t *const v, const size_t at);

/**
Inserts a new value at an index

Params:
	v = vec_t instance
	val = value to insert
	at = index to set the value

Returns: enum VitaError code
*/
extern enum VitaError vec_insert(vec_t *const v, const enum VitaError *val, const size_t at);

/**
Removes an element from vec_t

Params:
	v = vec_t instance
	at = index of the value
	rs = choose a RemoveStrategy (see Notes)

Notes:
	enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
	rs_stable:	shifts all values by element size
	rs_fast:	swaps the last value with the value of `at`

Returns: enum VitaError code
*/
extern enum VitaError vec_remove(vec_t *const v, const size_t at, const enum RemoveStrategy rs);

/**
Checks if vec_t contains the specified element

Params:
	v = vec_t instance
	val = value to check

Returns: index to first val instance, `-1` upon failure
*/
extern int64_t vec_contains(const vec_t *const v, const enum VitaError *val);

/**
Calls the specified function on each element

Params:
	v = vec_t instance
	func = function to execute action on each element: func(pointer, for loop index)
*/
extern void vec_foreach(const vec_t *const v, enum VitaError (*func)(void*, size_t));


#endif // VITA_VEC_H
