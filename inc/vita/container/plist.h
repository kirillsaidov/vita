#ifndef VITA_CONTAINER_PLIST_H
#define VITA_CONTAINER_PLIST_H

/** PLIST MODULE (dynamic array(list) of pointers)
    - vt_plist_create
    - vt_plist_destroy
    - vt_plist_len
    - vt_plist_capacity
    - vt_plist_has_space
    - vt_plist_is_empty
    - vt_plist_reserve
    - vt_plist_shrink
    - vt_plist_clear
    - vt_plist_set
    - vt_plist_get
    - vt_plist_push
    - vt_plist_pop
    - vt_plist_pop_get
    - vt_plist_remove
    - vt_plist_slide_front
    - vt_plist_slide_back
    - vt_plist_slide_reset
    - vt_plist_apply
*/

#include "common.h"

// new pointer list type
typedef struct VitaBaseArrayType vt_plist_t;

/** Allocates and constructs vt_plist_t
    @param n number of elements
    @param alloctr allocator instance

    @returns `vt_plist_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used
*/
extern vt_plist_t *vt_plist_create(const size_t n, struct VitaBaseAllocatorType *const alloctr);

/** Transfer memory ownership to allocator
    @param p vt_plist_t instance
    @param alloctr allocator instance
*/

/** Deallocates and destroys vt_plist_t
    @param p vt_plist_t pointer
*/
extern void vt_plist_destroy(vt_plist_t *p);

/** Duplicates and returns a new plist instance
    @param p vt_plist_t instance
    @param alloctr allocator instance

    @returns vt_vec_t* instance upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used
*/
extern vt_plist_t *vt_plist_dup(const vt_plist_t *const p, struct VitaBaseAllocatorType *const alloctr);

/** Returns length
    @param p vt_plist_t pointer
    @returns size_t length
*/
extern size_t vt_plist_len(const vt_plist_t *const p);

/** Returns capacity
    @param p vt_plist_t pointer
    @returns size_t capacity
*/
extern size_t vt_plist_capacity(const vt_plist_t *const p);

/** Returns available space (capacity - length)
    @param p vt_plist_t pointer
    @returns size_t available space (capacity - length)
*/
extern size_t vt_plist_has_space(const vt_plist_t *const p);

/** Checks if length == 0
    @param p vec_t instance
    @returns `true` if length == 0
*/
extern bool vt_plist_is_empty(const vt_plist_t *const p);

/** Reserves additional memory of n elements
    @param p vt_plist_t pointer
    @param n number of elements
*/
extern void vt_plist_reserve(vt_plist_t *const p, const size_t n);

/** Shrinks vt_plist_t capacity to its length
    @param p vt_plist_t pointer
*/
extern void vt_plist_shrink(vt_plist_t *const p);

/** Resizes vt_plist_t length
    @param p vt_plist_t instance
    @param n new size
*/
extern void vt_plist_resize(vt_plist_t *const p, const size_t n);

/** Inserts a pointer at an index
    @param p vt_plist_t instance
    @param ptr value to insert
    @param at index to set the value
*/
extern void vt_plist_insert(vt_plist_t *const p, const void *const ptr, const size_t at);

/** Sets vt_plist_t length to 0
    @param p vt_plist_t pointer
*/
extern void vt_plist_clear(vt_plist_t *const p);

/** Assigns a new pointer at an index
    @param p vt_plist_t pointer
    @param ptr pointer value
    @param at index
*/
extern void vt_plist_set(vt_plist_t *const p, const void *const ptr, const size_t at);

/** Returns a pointer at an index
    @param p vt_plist_t pointer
    @param at index

    @returns void* pointer
*/
extern void *vt_plist_get(const vt_plist_t *const p, const size_t at);

/** Push value at the end
    @param p vt_plist_t pointer
    @param ptr pointer value
*/
extern void vt_plist_push(vt_plist_t *const p, const void *const ptr);

/** Pop the last value from the end
    @param p vt_plist_t pointer
*/
extern void vt_plist_pop(vt_plist_t *const p);

/** Get and pop the last value from the end
    @param p vt_plist_t pointer
    @returns void*
*/
extern void *vt_plist_pop_get(vt_plist_t *const p);

/** Removes a pointer from a plist
    @param p vt_plist_t pointer
    @param at index

    @note
        enum VitaRemoveStrategy { VT_REMOVE_STRATEGY_STABLE = ordered removal, rs_fast = unordered removal }
        VT_REMOVE_STRATEGY_STABLE: shifts all values by element size
        rs_fast: swaps the last value with the value of `at`
*/
extern void vt_plist_remove(vt_plist_t *const p, const size_t at, const enum VitaRemoveStrategy rs);

/** Checks if vt_plist_t contains the specified element (pointer)
    @param p vt_plist_t instance
    @param ptr value to check

    @returns index to first val instance, `-1` upon failure
*/
extern int64_t vt_plist_can_find(const vt_plist_t *const p, const void *const ptr);

/** Slides through the container elements one by one starting from the begining
    @param p vt_plist_t pointer
    @returns ptr head pointing to next element from the start

    @note returns `NULL` upon reaching the end
*/
extern void *vt_plist_slide_front(vt_plist_t *const p);

/** Slides through the container elements one by one starting from the end
    @param p vt_plist_t pointer
    @returns ptr head pointing to next element from the end

    @note returns `NULL` upon reaching the begining
*/
extern void *vt_plist_slide_back(vt_plist_t *const p);

/** Resets the slider
    @param p vt_plist_t pointer
*/
extern void vt_plist_slide_reset(vt_plist_t *const p);

/** Calls the specified function on each element
    @param p vt_plist_t instance
    @param func function to execute upon each element: func(pointer, for loop index)

    @note function: func(void*, size_t)
*/
extern void vt_plist_apply(const vt_plist_t *const p, void (*func)(void*, size_t));


#endif // VITA_CONTAINER_PLIST_H
