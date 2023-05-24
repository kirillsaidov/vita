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
    - vt_vec_from
    - vt_vec_fromT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_len
    - vt_vec_capacity
    - vt_vec_has_space
    - vt_vec_is_empty
    - vt_vec_shrink
    - vt_vec_clear
    - vt_vec_reserve
    - vt_vec_resize
    - vt_vec_push
    - vt_vec_pushT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_pop
    - vt_vec_pop_get
    - vt_vec_pop_getT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_set
    - vt_vec_setT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_get
    - vt_vec_getT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_insert
    - vt_vec_insertT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_remove
    - vt_vec_can_find
    - vt_vec_containsT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_apply
*/

#include "../core/core.h"
#include "../util/debug.h"

// see core/core.h for definition
typedef struct VitaBaseArrayType vt_vec_t;

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

/** Pops off and returns the last element
    @param v vt_vec_t instance
    @returns element of type T
*/
#define VT_PROTOTYPE_VEC_POP_GET(T, t) extern T vt_vec_pop_get##t(vt_vec_t *const v)
VT_PROTOTYPE_VEC_POP_GET(int8_t, i8);
VT_PROTOTYPE_VEC_POP_GET(uint8_t, u8);
VT_PROTOTYPE_VEC_POP_GET(int16_t, i16);
VT_PROTOTYPE_VEC_POP_GET(uint16_t, u16);
VT_PROTOTYPE_VEC_POP_GET(int32_t, i32);
VT_PROTOTYPE_VEC_POP_GET(uint32_t, u32);
VT_PROTOTYPE_VEC_POP_GET(int64_t, i64);
VT_PROTOTYPE_VEC_POP_GET(uint64_t, u64);
VT_PROTOTYPE_VEC_POP_GET(float, f);
VT_PROTOTYPE_VEC_POP_GET(double, d);
VT_PROTOTYPE_VEC_POP_GET(real, r);
#undef VT_PROTOTYPE_VEC_POP_GET

/** Assigns a new value at an index
    @param v vt_vec_t instance
    @param val value
    @param at index to set the value

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_set(vt_vec_t *const v, const void *const val, const size_t at);

/** Assigns a new value at an index
    @param v vt_vec_t instance
    @param val value
    @param at index to set the value

    @returns enum VitaError code
*/
#define VT_PROTOTYPE_VEC_SET(T, t) extern enum VitaError vt_vec_set##t(vt_vec_t *const v, const T val, const size_t at)
VT_PROTOTYPE_VEC_SET(int8_t, i8);
VT_PROTOTYPE_VEC_SET(uint8_t, u8);
VT_PROTOTYPE_VEC_SET(int16_t, i16);
VT_PROTOTYPE_VEC_SET(uint16_t, u16);
VT_PROTOTYPE_VEC_SET(int32_t, i32);
VT_PROTOTYPE_VEC_SET(uint32_t, u32);
VT_PROTOTYPE_VEC_SET(int64_t, i64);
VT_PROTOTYPE_VEC_SET(uint64_t, u64);
VT_PROTOTYPE_VEC_SET(float, f);
VT_PROTOTYPE_VEC_SET(double, d);
VT_PROTOTYPE_VEC_SET(real, r);
#undef VT_PROTOTYPE_VEC_SET

/** Returns value at index
    @param v vt_vec_t instance
    @param at index

    @returns value (depends on data type)
*/
extern void* vt_vec_get(const vt_vec_t *const v, const size_t at);

/** Returns value at index
    @param v vt_vec_t instance
    @param at index

    @returns value
*/
#define VT_PROTOTYPE_VEC_GET(T, t) extern T vt_vec_get##t(const vt_vec_t *const v, const size_t at)
VT_PROTOTYPE_VEC_GET(int8_t, i8);
VT_PROTOTYPE_VEC_GET(uint8_t, u8);
VT_PROTOTYPE_VEC_GET(int16_t, i16);
VT_PROTOTYPE_VEC_GET(uint16_t, u16);
VT_PROTOTYPE_VEC_GET(int32_t, i32);
VT_PROTOTYPE_VEC_GET(uint32_t, u32);
VT_PROTOTYPE_VEC_GET(int64_t, i64);
VT_PROTOTYPE_VEC_GET(uint64_t, u64);
VT_PROTOTYPE_VEC_GET(float, f);
VT_PROTOTYPE_VEC_GET(double, d);
VT_PROTOTYPE_VEC_GET(real, r);
#undef VT_PROTOTYPE_VEC_GET

/** Inserts a new value at an index
    @param v vt_vec_t instance
    @param val value to insert
    @param at index to set the value

    @returns enum VitaError code
*/
extern enum VitaError vt_vec_insert(vt_vec_t *const v, const void *const val, const size_t at);

/** Inserts a new value at an index
    @param v vt_vec_t instance
    @param val value to insert
    @param at index to set the value

    @returns enum VitaError code
*/
#define VT_PROTOTYPE_VEC_INSERT(T, t) extern enum VitaError vt_vec_insert##t(vt_vec_t *const v, const T val, const size_t at)
VT_PROTOTYPE_VEC_INSERT(int8_t, i8);
VT_PROTOTYPE_VEC_INSERT(uint8_t, u8);
VT_PROTOTYPE_VEC_INSERT(int16_t, i16);
VT_PROTOTYPE_VEC_INSERT(uint16_t, u16);
VT_PROTOTYPE_VEC_INSERT(int32_t, i32);
VT_PROTOTYPE_VEC_INSERT(uint32_t, u32);
VT_PROTOTYPE_VEC_INSERT(int64_t, i64);
VT_PROTOTYPE_VEC_INSERT(uint64_t, u64);
VT_PROTOTYPE_VEC_INSERT(float, f);
VT_PROTOTYPE_VEC_INSERT(double, d);
VT_PROTOTYPE_VEC_INSERT(real, r);
#undef VT_PROTOTYPE_VEC_INSERT

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
extern int64_t vt_vec_can_find(const vt_vec_t *const v, const void *const val);

/** Checks if vt_vec_t contains the specified element
    @param v vt_vec_t instance
    @param val value to check

    @returns index to first val instance, `-1` upon failure
*/
#define VT_PROTOTYPE_VEC_CONTAINS(T, t) extern int64_t vt_vec_can_find##t(const vt_vec_t *const v, const T val)
VT_PROTOTYPE_VEC_CONTAINS(int8_t, i8);
VT_PROTOTYPE_VEC_CONTAINS(uint8_t, u8);
VT_PROTOTYPE_VEC_CONTAINS(int16_t, i16);
VT_PROTOTYPE_VEC_CONTAINS(uint16_t, u16);
VT_PROTOTYPE_VEC_CONTAINS(int32_t, i32);
VT_PROTOTYPE_VEC_CONTAINS(uint32_t, u32);
VT_PROTOTYPE_VEC_CONTAINS(int64_t, i64);
VT_PROTOTYPE_VEC_CONTAINS(uint64_t, u64);
VT_PROTOTYPE_VEC_CONTAINS(float, f);
VT_PROTOTYPE_VEC_CONTAINS(double, d);
VT_PROTOTYPE_VEC_CONTAINS(real, r);
#undef VT_PROTOTYPE_VEC_CONTAINS

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
