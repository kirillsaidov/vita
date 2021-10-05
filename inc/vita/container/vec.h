#ifndef VITA_VEC_H
#define VITA_VEC_H

/** VITA_VEC MODULE (dynamic array)
	- vec  
	- vec_dup					
	- vec_free 

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
	- vec_set
	- vec_get		
	- vec_insert
	- vec_remove			
	- vec_contains 	
	- vec_foreach		
*/

#include "../core/core.h"

#define VEC_GROWTH_RATE 1/3

// see core/core.h for definition
typedef struct BaseArrayType vec_t;

enum RemoveStrategy {
	rs_stable,	// keep ordering
	rs_fast,	// ordering doesn't matter
	rs_count	// number of elements
};

/**
Creates a new dynamic zero-initialized array of length of 0 and capacity of n

Params:
	n = number of elements
	elsize = element size

Returns: `vec_t*` upon success, `NULL` otherwise
*/
extern vec_t *vec(const size_t n, const size_t elsize);

/** 
Duplicates and returns a new dynamic array

Params:
	v = vec_t instance

Returns: vec_t* instance upon success, `NULL` otherwise
*/
extern vec_t *vec_dup(const vec_t *const v);

/** 
Frees the vec_t instance

Params:
	v = vec_t instance
*/
extern void vec_free(vec_t *v);







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
Checks if string is emtpy ("")

Params:
	v = vec_t instance

Returns: `true` if length == 0
*/
extern bool vec_is_empty(const vec_t *const v);







/**
Shrinks vec_t capacity to its length

Params:
	v = vec_t instance

Returns: `true` upon success
*/
extern bool vec_shrink(vec_t *const v);

/**
Clears the vec_t (sets length to 0)

Params:
	v = vec_t instance

Returns: `true` upon success
*/
extern bool vec_clear(vec_t *const v);

/** 
Reserves memory for vec_t

Params:
	v = vec_t instance
	n = how many elements to reserve

Returns: `true` upon success
*/
extern bool vec_reserve(vec_t *const v, const size_t n);

/** 
Resizes vec_t length

Params:
	v = vec_t instance
	n = new size

Returns: `true` upon success
*/
extern bool vec_resize(vec_t *const v, const size_t n);

/** 
Push an element at the end of vec_t

Params:
	v = vec_t instance
	val = value to push

Returns: `true` upon success
*/
extern bool vec_push(vec_t *const v, const void *val);
extern bool vec_pushi32(vec_t *const v, const int val);
extern bool vec_pushi64(vec_t *const v, const long val);
extern bool vec_pushf(vec_t *const v, const float val);
extern bool vec_pushd(vec_t *const v, const double val);

/** 
Pops off the last element

Params:
	v = vec_t instance

Returns: `true` upon success
*/
extern bool vec_pop(vec_t *const v);

/** 
Assigns a new value at an index

Params:
	v = vec_t instance
	val = value
	at = index to set the value

Returns: `true` upon success
*/
extern bool vec_set(vec_t *const v, const void *val, const size_t at);
extern bool vec_seti32(vec_t *const v, const int val, const size_t at);
extern bool vec_seti64(vec_t *const v, const long val, const size_t at);
extern bool vec_setf(vec_t *const v, const float val, const size_t at);
extern bool vec_setd(vec_t *const v, const double val, const size_t at);

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

Returns: `true` upon success
*/
extern bool vec_insert(vec_t *const v, const void *val, const size_t at);

/**
Removes an element from vec_t

Params:
	v = vec_t instance
	at = index of the value
	rs = choose a RemoveStrategy (see Notes)

Returns: `true` upon success

Notes:
	enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
	rs_stable:	shifts all values by element size
	rs_fast:	swaps the last value with the value of `at`
*/
extern bool vec_remove(vec_t *const v, const size_t at, const enum RemoveStrategy rs);

/** 
Checks if vec_t contains the specified element

Params:
	v = vec_t instance
	val = value to check

Returns: index to first val instance, `-1` upon failure
*/
extern int64_t vec_contains(const vec_t *const v, const void *val);

/** 
Calls the specified function on each element

Params:
	v = vec_t instance
	func = function to execute action on each element: func(pointer, for loop index)
*/
extern void vec_foreach(const vec_t *const v, void (*func)(void*, size_t));


#endif // VITA_VEC_H































