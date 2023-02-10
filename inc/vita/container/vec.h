#ifndef VITA_VEC_H
#define VITA_VEC_H

/** VITA_VEC MODULE (dynamic array)
    - vt_vec_new
    - vt_vec_ctor
    - vt_vec_dup
    - vt_vec_dtor
    - vt_vec_free
    - vt_vec_create
    - vt_vec_destroy
    - vt_vec_from ((u)i8, (u)i16, (u)i32, (u)i64, f, d versions as well)
    - vt_vec_len
    - vt_vec_capacity
    - vt_vec_has_space
    - vt_vec_is_empty
    - vt_vec_shrink
    - vt_vec_clear
    - vt_vec_reserve
    - vt_vec_resize
    - vt_vec_push ((u)i8, (u)i16, (u)i32, (u)i64, f, d versions as well)
    - vt_vec_pop
    - vt_vec_pop_get
    - vt_vec_set ((u)i8, (u)i16, (u)i32, (u)i64, f, d versions as well)
    - vt_vec_get ((u)i8, (u)i16, (u)i32, (u)i64, f, d versions as well)
    - vt_vec_insert
    - vt_vec_remove
    - vt_vec_contains
    - vt_vec_apply
*/

#include "../core/core.h"
#include "../util/debug.h"

// see core/core.h for definition
typedef struct VitaBaseContainerType vt_vec_t;

/** Allocates memory for vt_vec_t
    @returns `vt_vec_t*` upon success, `NULL` otherwise
*/
extern vt_vec_t *vt_vec_new(void);

/** Constructs vt_vec_t
    @param v vt_vec_t instance
    @param n number of elements
    @param elsize element size

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_ctor(vt_vec_t *const v, const size_t n, const size_t elsize);

/** Duplicates and returns a new dynamic array
    @param v vt_vec_t instance
    @returns vt_vec_t* instance upon success, `NULL` otherwise
*/
extern vt_vec_t *vt_vec_dup(const vt_vec_t *const v);

/** Destroys contents of vt_vec_t
    @param v vt_vec_t pointer
*/
extern void vt_vec_dtor(vt_vec_t *const v);

/** Frees the vt_vec_t instance
    @param v vt_vec_t pointer
*/
extern void vt_vec_free(vt_vec_t *v);

/** Allocates and constructs vt_vec_t
    @param n number of elements
    @param elsize element size

    @returns `vt_vec_t*` upon success, `NULL` otherwise
*/
extern vt_vec_t *vt_vec_create(const size_t n, const size_t elsize);

/** Deallocates and destroys vt_vec_t
    @param v vt_vec_t pointer
*/
extern void vt_vec_destroy(vt_vec_t *v);

/** Allocates and constructs vt_vec_t from an array
    @param ptr array 
    @param n number of elements
    @param elsize element size

    @returns `vt_vec_t*` upon success, `NULL` otherwise

    @note 
        If ptr == NULL, returns an empty `vt_vec_t` instance
*/
extern vt_vec_t *vt_vec_from(const void *const ptr, const size_t n, const size_t elsize);
extern vt_vec_t *vt_vec_fromi8(const int8_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromu8(const uint8_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromi16(const int16_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromu16(const uint16_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromi32(const int32_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromu32(const uint32_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromi64(const int64_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromu64(const uint64_t *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromf(const float *const ptr, const size_t n);
extern vt_vec_t *vt_vec_fromd(const double *const ptr, const size_t n);

/** Returns vt_vec_t length
    @param v vt_vec_t instance
    @returns vt_vec_t length
*/
extern size_t vt_vec_len(const vt_vec_t *const v);

/** Returns vt_vec_t capacity
    @param v vt_vec_t instance
    @returns vt_vec_t capacity
*/
extern size_t vt_vec_capacity(const vt_vec_t *const v);

/** Returns available space before new allocation is required
    @param v vt_vec_t instance
    @returns free space (capacity - length)
*/
extern size_t vt_vec_has_space(const vt_vec_t *const v);

/** Checks if length == 0
    @param v vt_vec_t instance
    @returns `true` if length == 0
*/
extern bool vt_vec_is_empty(const vt_vec_t *const v);

/** Shrinks vt_vec_t capacity to its length
    @param v vt_vec_t instance
    @returns enum VitaError code
*/
extern enum VitaError vt_vec_shrink(vt_vec_t *const v);

/** Clears the vt_vec_t (sets length to 0)
    @param v vt_vec_t instance
    @returns enum VitaError code
*/
extern enum VitaError vt_vec_clear(vt_vec_t *const v);

/** Reserves memory for vt_vec_t
    @param v vt_vec_t instance
    @param n how many elements to reserve

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_reserve(vt_vec_t *const v, const size_t n);

/** Resizes vt_vec_t length
    @param v vt_vec_t instance
    @param n new size

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_resize(vt_vec_t *const v, const size_t n);

/** Push an element at the end of vt_vec_t
    @param v vt_vec_t instance
    @param val value to push

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_push(vt_vec_t *const v, const void *const val);
extern enum VitaError vt_vec_pushi8(vt_vec_t *const v, const int8_t val);
extern enum VitaError vt_vec_pushu8(vt_vec_t *const v, const uint8_t val);
extern enum VitaError vt_vec_pushi16(vt_vec_t *const v, const int16_t val);
extern enum VitaError vt_vec_pushu16(vt_vec_t *const v, const uint16_t val);
extern enum VitaError vt_vec_pushi32(vt_vec_t *const v, const int32_t val);
extern enum VitaError vt_vec_pushu32(vt_vec_t *const v, const uint32_t val);
extern enum VitaError vt_vec_pushi64(vt_vec_t *const v, const int64_t val);
extern enum VitaError vt_vec_pushu64(vt_vec_t *const v, const uint64_t val);
extern enum VitaError vt_vec_pushf(vt_vec_t *const v, const float val);
extern enum VitaError vt_vec_pushd(vt_vec_t *const v, const double val);

/** Pops off the last element
    @param v vt_vec_t instance
    @returns enum VitaError code
*/
extern enum VitaError vt_vec_pop(vt_vec_t *const v);

/** Pops off and returns the last element
    @param v vt_vec_t instance
    @returns void*
*/
extern void *vt_vec_pop_get(vt_vec_t *const v);

/** Assigns a new value at an index
    @param v vt_vec_t instance
    @param val value
    @param at index to set the value

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_set(vt_vec_t *const v, const void *const val, const size_t at);
extern enum VitaError vt_vec_seti8(vt_vec_t *const v, const int8_t val, const size_t at);
extern enum VitaError vt_vec_setu8(vt_vec_t *const v, const uint8_t val, const size_t at);
extern enum VitaError vt_vec_seti16(vt_vec_t *const v, const int16_t val, const size_t at);
extern enum VitaError vt_vec_setu16(vt_vec_t *const v, const uint16_t val, const size_t at);
extern enum VitaError vt_vec_seti32(vt_vec_t *const v, const int32_t val, const size_t at);
extern enum VitaError vt_vec_setu32(vt_vec_t *const v, const uint32_t val, const size_t at);
extern enum VitaError vt_vec_seti64(vt_vec_t *const v, const int64_t val, const size_t at);
extern enum VitaError vt_vec_setu64(vt_vec_t *const v, const uint64_t val, const size_t at);
extern enum VitaError vt_vec_setf(vt_vec_t *const v, const float val, const size_t at);
extern enum VitaError vt_vec_setd(vt_vec_t *const v, const double val, const size_t at);

/** Returns value at index
    @param v vt_vec_t instance
    @param at index

    @returns value (depends on data type)
*/
extern void* vt_vec_get(const vt_vec_t *const v, const size_t at);
extern int8_t vt_vec_geti8(const vt_vec_t *const v, const size_t at);
extern uint8_t vt_vec_getu8(const vt_vec_t *const v, const size_t at);
extern int16_t vt_vec_geti16(const vt_vec_t *const v, const size_t at);
extern uint16_t vt_vec_getu16(const vt_vec_t *const v, const size_t at);
extern int32_t vt_vec_geti32(const vt_vec_t *const v, const size_t at);
extern uint32_t vt_vec_getu32(const vt_vec_t *const v, const size_t at);
extern int64_t vt_vec_geti64(const vt_vec_t *const v, const size_t at);
extern uint64_t vt_vec_getu64(const vt_vec_t *const v, const size_t at);
extern float vt_vec_getf(const vt_vec_t *const v, const size_t at);
extern double vt_vec_getd(const vt_vec_t *const v, const size_t at);

/** Inserts a new value at an index
    @param v vt_vec_t instance
    @param val value to insert
    @param at index to set the value

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_insert(vt_vec_t *const v, const void *const val, const size_t at);

/** Removes an element from vt_vec_t
    @param v vt_vec_t instance
    @param at index of the value
    @param rs choose a RemoveStrategy (see Notes)

    @returns enum VitaError code

    @note
        enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
        rs_stable: shifts all values by element size
          rs_fast: swaps the last value with the value of `at`
*/
extern enum VitaError vt_vec_remove(vt_vec_t *const v, const size_t at, const enum RemoveStrategy rs);

/** Checks if vt_vec_t contains the specified element
    @param v vt_vec_t instance
    @param val value to check

    @returns index to first val instance, `-1` upon failure
*/
extern int64_t vt_vec_contains(const vt_vec_t *const v, const void *const val);

/** Calls the specified function on each element
    @param v vt_vec_t instance
    @param func function to execute action on each element: func(pointer, for loop index)
*/
extern void vt_vec_apply(const vt_vec_t *const v, void (*func)(void*, size_t));

#endif // VITA_VEC_H
