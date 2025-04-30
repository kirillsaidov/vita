#ifndef VITA_CONTAINER_VEC_H
#define VITA_CONTAINER_VEC_H

/** VEC MODULE (dynamic array)
    - vt_vec_create
    - vt_vec_create_from
    - vt_vec_create_from_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_destroy
    - vt_vec_dup
    - vt_vec_len
    - vt_vec_capacity
    - vt_vec_has_space
    - vt_vec_is_empty
    - vt_vec_shrink
    - vt_vec_clear
    - vt_vec_reserve
    - vt_vec_resize
    - vt_vec_push_front
    - vt_vec_push_front_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_push_back
    - vt_vec_push_back_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_pop
    - vt_vec_pop_get
    - vt_vec_pop_get_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_set
    - vt_vec_set_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_get
    - vt_vec_get_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_insert
    - vt_vec_insert_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_remove
    - vt_vec_can_find
    - vt_vec_can_find_T (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_vec_slide_front
    - vt_vec_slide_back
    - vt_vec_slide_reset
    - vt_vec_apply
*/

#include "vita/container/common.h"

/// vector object
/// see container/common.h for definitions
typedef struct VitaBaseArrayType vt_vec_t;

/** Allocates and constructs vt_vec_t
    @param n number of elements
    @param elsize element size
    @param alloctr allocator instance

    @returns `vt_vec_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then `vt_calloc/realloc/free` is used
*/
extern vt_vec_t *vt_vec_create(const size_t n, const size_t elsize, struct VitaBaseAllocatorType *const alloctr);

/** Allocates and constructs vt_vec_t from an array of elements
    @param n number of elements
    @param elsize element size
    @param vals array of values
    @param alloctr allocator instance

    @returns `vt_vec_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then `vt_calloc/realloc/free` is used
*/
extern vt_vec_t *vt_vec_create_from(const size_t n, const size_t elsize, const void *vals, struct VitaBaseAllocatorType *const alloctr);

/** Allocates and constructs vt_vec_t from an array of elements
    @param n number of elements
    @param elsize element size
    @param vals array of values
    @param alloctr allocator instance

    @returns `vt_vec_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then `vt_calloc/realloc/free` is used
*/
#define VT_PROTOTYPE_VEC_CREATE_FROM(T, t) extern vt_vec_t *vt_vec_create_from_##t(const size_t n, const T vals[], struct VitaBaseAllocatorType *const alloctr)
VT_PROTOTYPE_VEC_CREATE_FROM(int8_t, i8);
VT_PROTOTYPE_VEC_CREATE_FROM(uint8_t, u8);
VT_PROTOTYPE_VEC_CREATE_FROM(int16_t, i16);
VT_PROTOTYPE_VEC_CREATE_FROM(uint16_t, u16);
VT_PROTOTYPE_VEC_CREATE_FROM(int32_t, i32);
VT_PROTOTYPE_VEC_CREATE_FROM(uint32_t, u32);
VT_PROTOTYPE_VEC_CREATE_FROM(int64_t, i64);
VT_PROTOTYPE_VEC_CREATE_FROM(uint64_t, u64);
VT_PROTOTYPE_VEC_CREATE_FROM(float, f);
VT_PROTOTYPE_VEC_CREATE_FROM(double, d);
VT_PROTOTYPE_VEC_CREATE_FROM(real, r);
#undef VT_PROTOTYPE_VEC_CREATE_FROM

/** Deallocates and destroys vt_vec_t
    @param v vt_vec_t pointer
*/
extern void vt_vec_destroy(vt_vec_t *v);

/** Duplicates and returns a new dynamic array
    @param v vt_vec_t instance
    @param alloctr allocator instance

    @returns vt_vec_t* instance upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then `vt_calloc/realloc/free` is used
*/
extern vt_vec_t *vt_vec_dup(const vt_vec_t *const v, struct VitaBaseAllocatorType *const alloctr);

/** Returns vt_vec_t length
    @param v vt_vec_t instance
    @returns size_t length
*/
extern size_t vt_vec_len(const vt_vec_t *const v);

/** Returns vt_vec_t capacity
    @param v vt_vec_t instance
    @returns size_t capacity
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
*/
extern void vt_vec_shrink(vt_vec_t *const v);

/** Clears the vt_vec_t (sets length to 0)
    @param v vt_vec_t instance
*/
extern void vt_vec_clear(vt_vec_t *const v);

/** Reserves memory for vt_vec_t
    @param v vt_vec_t instance
    @param n how many elements to reserve
*/
extern void vt_vec_reserve(vt_vec_t *const v, const size_t n);

/** Resizes vt_vec_t length
    @param v vt_vec_t instance
    @param n new size
*/
extern void vt_vec_resize(vt_vec_t *const v, const size_t n);

/** Push an element at the begining
    @param v vt_vec_t instance
    @param val value to push
*/
extern void vt_vec_push_front(vt_vec_t *const v, const void *const val);

/** Push an element at the begining
    @param v vt_vec_t instance
    @param val value to push
*/
#define VT_PROTOTYPE_VEC_PUSH_FRONT(T, t) extern void vt_vec_push_front_##t(vt_vec_t *const v, const T val)
VT_PROTOTYPE_VEC_PUSH_FRONT(int8_t, i8);
VT_PROTOTYPE_VEC_PUSH_FRONT(uint8_t, u8);
VT_PROTOTYPE_VEC_PUSH_FRONT(int16_t, i16);
VT_PROTOTYPE_VEC_PUSH_FRONT(uint16_t, u16);
VT_PROTOTYPE_VEC_PUSH_FRONT(int32_t, i32);
VT_PROTOTYPE_VEC_PUSH_FRONT(uint32_t, u32);
VT_PROTOTYPE_VEC_PUSH_FRONT(int64_t, i64);
VT_PROTOTYPE_VEC_PUSH_FRONT(uint64_t, u64);
VT_PROTOTYPE_VEC_PUSH_FRONT(float, f);
VT_PROTOTYPE_VEC_PUSH_FRONT(double, d);
VT_PROTOTYPE_VEC_PUSH_FRONT(real, r);
#undef VT_PROTOTYPE_VEC_PUSH_FRONT

/** Push an element at the end of vt_vec_t
    @param v vt_vec_t instance
    @param val value to push
*/
extern void vt_vec_push_back(vt_vec_t *const v, const void *const val);

/** Push an element at the end of vt_vec_t
    @param v vt_vec_t instance
    @param val value to push
*/
#define VT_PROTOTYPE_VEC_PUSH_BACK(T, t) extern void vt_vec_push_back_##t(vt_vec_t *const v, const T val)
VT_PROTOTYPE_VEC_PUSH_BACK(int8_t, i8);
VT_PROTOTYPE_VEC_PUSH_BACK(uint8_t, u8);
VT_PROTOTYPE_VEC_PUSH_BACK(int16_t, i16);
VT_PROTOTYPE_VEC_PUSH_BACK(uint16_t, u16);
VT_PROTOTYPE_VEC_PUSH_BACK(int32_t, i32);
VT_PROTOTYPE_VEC_PUSH_BACK(uint32_t, u32);
VT_PROTOTYPE_VEC_PUSH_BACK(int64_t, i64);
VT_PROTOTYPE_VEC_PUSH_BACK(uint64_t, u64);
VT_PROTOTYPE_VEC_PUSH_BACK(float, f);
VT_PROTOTYPE_VEC_PUSH_BACK(double, d);
VT_PROTOTYPE_VEC_PUSH_BACK(real, r);
#undef VT_PROTOTYPE_VEC_PUSH_BACK

/** Pops off the last element
    @param v vt_vec_t instance
*/
extern void vt_vec_pop(vt_vec_t *const v);

/** Pops off and returns the last element
    @param v vt_vec_t instance
    @returns void* if len > 0 else NULL
*/
extern void *vt_vec_pop_get(vt_vec_t *const v);

/** Pops off and returns the last element
    @param v vt_vec_t instance
    @returns element of type T if len > 0 else 0
    
    @note use with `vt_vec_len()` if using a loop to check for length!
*/
#define VT_PROTOTYPE_VEC_POP_GET(T, t) extern T vt_vec_pop_get_##t(vt_vec_t *const v)
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
*/
extern void vt_vec_set(vt_vec_t *const v, const void *const val, const size_t at);

/** Assigns a new value at an index
    @param v vt_vec_t instance
    @param val value
    @param at index to set the value
*/
#define VT_PROTOTYPE_VEC_SET(T, t) extern void vt_vec_set_##t(vt_vec_t *const v, const T val, const size_t at)
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

    @returns void*
*/
extern void* vt_vec_get(const vt_vec_t *const v, const size_t at);

/** Returns value at index
    @param v vt_vec_t instance
    @param at index

    @returns value
*/
#define VT_PROTOTYPE_VEC_GET(T, t) extern T vt_vec_get_##t(const vt_vec_t *const v, const size_t at)
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
*/
extern void vt_vec_insert(vt_vec_t *const v, const void *const val, const size_t at);

/** Inserts a new value at an index
    @param v vt_vec_t instance
    @param val value to insert
    @param at index to set the value
*/
#define VT_PROTOTYPE_VEC_INSERT(T, t) extern void vt_vec_insert_##t(vt_vec_t *const v, const T val, const size_t at)
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

/** Removes an indexed element from vt_vec_t
    @param v vt_vec_t instance
    @param at index of the value
    @param rs choose a VitaRemoveStrategy (see Notes)

    @note
        enum VitaRemoveStrategy { VT_REMOVE_STRATEGY_STABLE = ordered removal, rs_fast = unordered removal };
        VT_REMOVE_STRATEGY_STABLE: shifts all values by element size;
        rs_fast: swaps the last value with the value of `at`.
*/
extern void vt_vec_remove(vt_vec_t *const v, const size_t at, const enum VitaRemoveStrategy rs);

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
#define VT_PROTOTYPE_VEC_CAN_FIND(T, t) extern int64_t vt_vec_can_find_##t(const vt_vec_t *const v, const T val)
VT_PROTOTYPE_VEC_CAN_FIND(int8_t, i8);
VT_PROTOTYPE_VEC_CAN_FIND(uint8_t, u8);
VT_PROTOTYPE_VEC_CAN_FIND(int16_t, i16);
VT_PROTOTYPE_VEC_CAN_FIND(uint16_t, u16);
VT_PROTOTYPE_VEC_CAN_FIND(int32_t, i32);
VT_PROTOTYPE_VEC_CAN_FIND(uint32_t, u32);
VT_PROTOTYPE_VEC_CAN_FIND(int64_t, i64);
VT_PROTOTYPE_VEC_CAN_FIND(uint64_t, u64);
VT_PROTOTYPE_VEC_CAN_FIND(float, f);
VT_PROTOTYPE_VEC_CAN_FIND(double, d);
VT_PROTOTYPE_VEC_CAN_FIND(real, r);
#undef VT_PROTOTYPE_VEC_CAN_FIND

/** Slides through the container elements one by one starting from the begining
    @param v vt_vec_t instance
    @returns container ptr head pointing to next element from the start

    @note returns `NULL` upon reaching the end
*/
extern void *vt_vec_slide_front(vt_vec_t *const v);

/** Slides through the container elements one by one starting from the end
    @param v vt_vec_t instance
    @returns container ptr head pointing to next element from the end

    @note returns `NULL` upon reaching the begining
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

#endif // VITA_CONTAINER_VEC_H
