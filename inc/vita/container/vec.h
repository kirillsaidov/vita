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
    - vec_from (i32, i64, d, f versions as well)
    - vec_len
    - vec_capacity
    - vec_has_space
    - vec_is_empty
    - vec_shrink
    - vec_clear
    - vec_reserve
    - vec_resize
    - vec_push (i32, i64, d, f versions as well)
    - vec_pop (i32, i64, d, f versions as well)
    - vec_pop_get
    - vec_set (i32, i64, d, f versions as well)
    - vec_get (i32, i64, d, f versions as well)
    - vec_insert
    - vec_remove
    - vec_contains
    - vec_apply
*/

#include "../core/core.h"
#include "../util/debug.h"

// see core/core.h for definition
typedef struct BaseContainerType vec_t;

/** Allocates memory for vec_t
    @returns `vec_t*` upon success, `NULL` otherwise
*/
extern vec_t *vec_new(void);

/** Constructs vec_t
    @param v vec_t instance
    @param n number of elements
    @param elsize element size

    @returns enum VitaError code
*/
extern enum VitaError vec_ctor(vec_t *const v, const size_t n, const size_t elsize);

/** Duplicates and returns a new dynamic array
    @param v vec_t instance
    @returns vec_t* instance upon success, `NULL` otherwise
*/
extern vec_t *vec_dup(const vec_t *const v);

/** Destroys contents of vec_t
    @param v vec_t pointer
*/
extern void vec_dtor(vec_t *const v);

/** Frees the vec_t instance
    @param v vec_t pointer
*/
extern void vec_free(vec_t *v);

/** Allocates and constructs vec_t
    @param n number of elements
    @param elsize element size

    @returns `vec_t*` upon success, `NULL` otherwise
*/
extern vec_t *vec_create(const size_t n, const size_t elsize);

/** Deallocates and destroys vec_t
    @param v vec_t pointer
*/
extern void vec_destroy(vec_t *v);

/** Allocates and constructs vec_t from an array
    @param ptr array 
    @param n number of elements
    @param elsize element size

    @returns `vec_t*` upon success, `NULL` otherwise

    @note 
        If ptr == NULL, returns an empty `vec_t` instance
*/
extern vec_t *vec_from(const void *const ptr, const size_t n, const size_t elsize);
extern vec_t *vec_fromi32(const int32_t *const ptr, const size_t n);
extern vec_t *vec_fromi64(const int64_t *const ptr, const size_t n);
extern vec_t *vec_fromf(const float *const ptr, const size_t n);
extern vec_t *vec_fromd(const double *const ptr, const size_t n);

/** Returns vec_t length
    @param v vec_t instance
    @returns vec_t length
*/
extern size_t vec_len(const vec_t *const v);

/** Returns vec_t capacity
    @param v vec_t instance
    @returns vec_t capacity
*/
extern size_t vec_capacity(const vec_t *const v);

/** Returns available space before new allocation is required
    @param v vec_t instance
    @returns free space (capacity - length)
*/
extern size_t vec_has_space(const vec_t *const v);

/** Checks if length == 0
    @param v vec_t instance
    @returns `true` if length == 0
*/
extern bool vec_is_empty(const vec_t *const v);

/** Shrinks vec_t capacity to its length
    @param v vec_t instance
    @returns enum VitaError code
*/
extern enum VitaError vec_shrink(vec_t *const v);

/** Clears the vec_t (sets length to 0)
    @param v vec_t instance
    @returns enum VitaError code
*/
extern enum VitaError vec_clear(vec_t *const v);

/** Reserves memory for vec_t
    @param v vec_t instance
    @param n how many elements to reserve

    @returns enum VitaError code
*/
extern enum VitaError vec_reserve(vec_t *const v, const size_t n);

/** Resizes vec_t length
    @param v vec_t instance
    @param n new size

    @returns enum VitaError code
*/
extern enum VitaError vec_resize(vec_t *const v, const size_t n);

/** Push an element at the end of vec_t
    @param v vec_t instance
    @param val value to push

    @returns enum VitaError code
*/
extern enum VitaError vec_push(vec_t *const v, const void *const val);
extern enum VitaError vec_pushi32(vec_t *const v, const int32_t val);
extern enum VitaError vec_pushi64(vec_t *const v, const int64_t val);
extern enum VitaError vec_pushf(vec_t *const v, const float val);
extern enum VitaError vec_pushd(vec_t *const v, const double val);

/** Pops off the last element
    @param v vec_t instance
    @returns enum VitaError code
*/
extern enum VitaError vec_pop(vec_t *const v);

/** Pops off and returns the last element
    @param v vec_t instance
    @returns void*
*/
extern void *vec_pop_get(vec_t *const v);

/** Assigns a new value at an index
    @param v vec_t instance
    @param val value
    @param at index to set the value

    @returns enum VitaError code
*/
extern enum VitaError vec_set(vec_t *const v, const void *const val, const size_t at);
extern enum VitaError vec_seti32(vec_t *const v, const int32_t val, const size_t at);
extern enum VitaError vec_seti64(vec_t *const v, const int64_t val, const size_t at);
extern enum VitaError vec_setf(vec_t *const v, const float val, const size_t at);
extern enum VitaError vec_setd(vec_t *const v, const double val, const size_t at);

/** Returns value at index
    @param v vec_t instance
    @param at index

    @returns value (depends on data type)
*/
extern void* vec_get(const vec_t *const v, const size_t at);
extern int32_t vec_geti32(const vec_t *const v, const size_t at);
extern int64_t vec_geti64(const vec_t *const v, const size_t at);
extern float vec_getf(const vec_t *const v, const size_t at);
extern double vec_getd(const vec_t *const v, const size_t at);

/** Inserts a new value at an index
    @param v vec_t instance
    @param val value to insert
    @param at index to set the value

    @returns enum VitaError code
*/
extern enum VitaError vec_insert(vec_t *const v, const void *const val, const size_t at);

/** Removes an element from vec_t
    @param v vec_t instance
    @param at index of the value
    @param rs choose a RemoveStrategy (see Notes)

    @returns enum VitaError code

    @note
        enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
        rs_stable: shifts all values by element size
          rs_fast: swaps the last value with the value of `at`
*/
extern enum VitaError vec_remove(vec_t *const v, const size_t at, const enum RemoveStrategy rs);

/** Checks if vec_t contains the specified element
    @param v vec_t instance
    @param val value to check

    @returns index to first val instance, `-1` upon failure
*/
extern int64_t vec_contains(const vec_t *const v, const void *const val);

/** Calls the specified function on each element
    @param v vec_t instance
    @param func function to execute action on each element: func(pointer, for loop index)
*/
extern void vec_apply(const vec_t *const v, void (*func)(void*, size_t));

#endif // VITA_VEC_H
