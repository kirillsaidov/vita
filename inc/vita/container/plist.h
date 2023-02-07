#ifndef VITA_PLIST_H
#define VITA_PLIST_H

/** VITA_PLIST MODULE (dynamic array(list) of pointers)
    - vt_plist_new
    - vt_plist_ctor
    - vt_plist_dtor
    - vt_plist_free
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
    - vt_plist_apply
*/

#include "../core/core.h"
#include "../util/debug.h"

// new pointer list type
typedef struct VitaBaseContainerType vt_plist_t;

/** Allocates memory for vt_plist_t
    @returns `vt_plist_t*` upon success, `NULL` upon failure
*/
extern vt_plist_t *vt_plist_new(void);

/** Constructs vt_plist_t
    @param p vt_plist_t instance
    @param n number of elements

    @returns enum VitaError
*/
extern enum VitaError vt_plist_ctor(vt_plist_t *p, const size_t n);

/** Destroys contents of vt_plist_t
    @param p vt_plist_t instance
    @returns enum VitaError
*/
extern void vt_plist_dtor(vt_plist_t *p);

/** Frees the vt_plist_t instance
    @param p vt_plist_t pointer
*/
extern void vt_plist_free(vt_plist_t *p);

/** Allocates and constructs vt_plist_t
    @param n number of elements
    @returns `vt_plist_t*` upon success, `NULL` otherwise
*/
extern vt_plist_t *vt_plist_create(const size_t n);

/** Deallocates and destroys vt_plist_t
    @param p vt_plist_t pointer
*/
extern void vt_plist_destroy(vt_plist_t *p);

/** Allocates and constructs vt_plist_t from an array
    @param ptr array 
    @param n number of elements

    @returns `vt_plist_t*` upon success, `NULL` otherwise

    @note 
        If ptr == NULL, returns an empty `vt_plist_t` instance
*/
extern vt_plist_t *vt_plist_from(const void **const ptr, const size_t n);

/** Returns length, capacity, available space (capacity - length)
    @param p vt_plist_t pointer
    @returns size_t (length, capacity, available space (capacity - length))
*/
extern size_t vt_plist_len(const vt_plist_t *const p);
extern size_t vt_plist_capacity(const vt_plist_t *const p);
extern size_t vt_plist_has_space(const vt_plist_t *const p);

/** Checks if length == 0
    @param p vec_t instance
    @returns `true` if length == 0
*/
extern bool vt_plist_is_empty(const vt_plist_t *const p);

/** Reserves additional memory of n elements
    @param p vt_plist_t pointer
    @param n number of elements

    @returns enum VitaError
*/
extern enum VitaError vt_plist_reserve(vt_plist_t *const p, const size_t n);

/** Shrinks vt_plist_t capacity to its length
    @param p vt_plist_t pointer
    @returns enum VitaError
*/
extern enum VitaError vt_plist_shrink(vt_plist_t *const p);

/** Sets vt_plist_t length to 0
    @param p vt_plist_t pointer
    @returns enum VitaError
*/
extern enum VitaError vt_plist_clear(vt_plist_t *const p);

/** Assigns a new pointer at an index
    @param p vt_plist_t pointer
    @param ptr pointer value
    @param at index

    @returns enum VitaError
*/
extern enum VitaError vt_plist_set(vt_plist_t *const p, const void *const ptr, const size_t at);

/** Returns a pointer at an index
    @param p vt_plist_t pointer
    @param at index

    @returns void* pointer
*/
extern void *vt_plist_get(const vt_plist_t *const p, const size_t at);

/** Push value at the end
    @param p vt_plist_t pointer
    @param ptr pointer value

    @returns enum VitaError
*/
extern enum VitaError vt_plist_push(vt_plist_t *const p, const void *const ptr);

/** Pop the last value from the end
    @param p vt_plist_t pointer
    @returns enum VitaError
*/
extern enum VitaError vt_plist_pop(vt_plist_t *const p);

/** Get and pop the last value from the end
    @param p vt_plist_t pointer
    @returns void*
*/
extern void *vt_plist_pop_get(vt_plist_t *const p);

/** Removes a pointer from a plist
    @param p vt_plist_t pointer
    @param at index

    @returns enum VitaError

    @note
        enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
        rs_stable: shifts all values by element size
          rs_fast: swaps the last value with the value of `at`
*/
extern enum VitaError vt_plist_remove(vt_plist_t *const p, const size_t at, const enum VitaRemoveStrategy rs);

/** Calls the specified function on each element
    @param p vt_plist_t instance
    @param func function to execute upon each element: func(pointer, for loop index)
*/
extern void vt_plist_apply(const vt_plist_t *const p, void (*func)(void*, size_t));


#endif // VITA_PLIST_H
