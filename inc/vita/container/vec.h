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

/** Allocates and constructs vt_vec_t from an array
    @param ptr array 
    @param n number of elements

    @returns `vt_vec_t*` upon success, `NULL` otherwise

    @note 
        If ptr == NULL, returns an empty `vt_vec_t` instance
*/
#define VT_PROTOTYPE_VEC_FROM(T, t) extern vt_vec_t *vt_vec_from##t(const T *const ptr, const size_t n)
VT_PROTOTYPE_VEC_FROM(int8_t, i8);
VT_PROTOTYPE_VEC_FROM(uint8_t, u8);
VT_PROTOTYPE_VEC_FROM(int16_t, i16);
VT_PROTOTYPE_VEC_FROM(uint16_t, u16);
VT_PROTOTYPE_VEC_FROM(int32_t, i32);
VT_PROTOTYPE_VEC_FROM(uint32_t, u32);
VT_PROTOTYPE_VEC_FROM(int64_t, i64);
VT_PROTOTYPE_VEC_FROM(uint64_t, u64);
VT_PROTOTYPE_VEC_FROM(float, f);
VT_PROTOTYPE_VEC_FROM(double, d);
VT_PROTOTYPE_VEC_FROM(real, r);
#undef VT_PROTOTYPE_VEC_FROM

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

/** Push an element at the end of vt_vec_t
    @param v vt_vec_t instance
    @param val value to push

    @returns enum VitaError code
*/
#define VT_PROTOTYPE_VEC_PUSH(T, t) extern enum VitaError vt_vec_push##t(vt_vec_t *const v, const T val)
VT_PROTOTYPE_VEC_PUSH(int8_t, i8);
VT_PROTOTYPE_VEC_PUSH(uint8_t, u8);
VT_PROTOTYPE_VEC_PUSH(int16_t, i16);
VT_PROTOTYPE_VEC_PUSH(uint16_t, u16);
VT_PROTOTYPE_VEC_PUSH(int32_t, i32);
VT_PROTOTYPE_VEC_PUSH(uint32_t, u32);
VT_PROTOTYPE_VEC_PUSH(int64_t, i64);
VT_PROTOTYPE_VEC_PUSH(uint64_t, u64);
VT_PROTOTYPE_VEC_PUSH(float, f);
VT_PROTOTYPE_VEC_PUSH(double, d);
VT_PROTOTYPE_VEC_PUSH(real, r);
#undef VT_PROTOTYPE_VEC_PUSH

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
extern enum VitaError vt_vec_setr(vt_vec_t *const v, const real val, const size_t at);

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
extern real vt_vec_getr(const vt_vec_t *const v, const size_t at);

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
    @param rs choose a VitaRemoveStrategy (see Notes)

    @returns enum VitaError code

    @note
        enum VitaRemoveStrategy { vt_rs_stable = ordered removal, rs_fast = unordered removal }
        vt_rs_stable: shifts all values by element size
          rs_fast: swaps the last value with the value of `at`
*/
extern enum VitaError vt_vec_remove(vt_vec_t *const v, const size_t at, const enum VitaRemoveStrategy rs);

/** Checks if vt_vec_t contains the specified element
    @param v vt_vec_t instance
    @param val value to check

    @returns index to first val instance, `-1` upon failure
*/
extern int64_t vt_vec_contains(const vt_vec_t *const v, const void *const val);

/** Slides through the container elements one by one
    @param v vt_vec_t instance
    @returns container ptr head pointing to next element from the start

    @note returns `NULL` upon reaching the end
*/
extern void *vt_vec_slide_front(vt_vec_t *const v);

/** Slides through the container elements one by one
    @param v vt_vec_t instance
    @returns container ptr head pointing to next element from the end

    @note returns `NULL` upon reaching the end
*/
extern void *vt_vec_slide_back(vt_vec_t *const v);

/** Resets the slider
    @param v vt_vec_t instance
*/
extern void vt_vec_slide_reset(vt_vec_t *const v);

/** Calls the specified function on each element
    @param v vt_vec_t instance
    @param func function to execute action on each element: func(pointer, for loop index)
*/
extern void vt_vec_apply(const vt_vec_t *const v, void (*func)(void*, size_t));

#endif // VITA_VEC_H
