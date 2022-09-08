#ifndef VITA_PLIST_H
#define VITA_PLIST_H

/** VITA_PLIST MODULE (dynamic array(list) of pointers)
    - plist_new
    - plist_ctor
    - plist_dtor
    - plist_free
    - plist_create
    - plist_destroy
    - plist_len
    - plist_capacity
    - plist_has_space
    - plist_reserve
    - plist_shrink
    - plist_clear
    - plist_set
    - plist_get
    - plist_push
    - plist_pop
    - plist_pop_get
    - plist_remove
    - plist_apply
*/

#include "../core/core.h"
#include "../util/debug.h"

// new pointer list type
typedef struct BaseContainerType plist_t;

/** Allocates memory for plist_t
    @returns `plist_t*` upon success, `NULL` upon failure
*/
extern plist_t *plist_new(void);

/** Constructs plist_t
    @param p plist_t instance
    @param n number of elements

    @returns enum VitaError
*/
extern enum VitaError plist_ctor(plist_t *p, const size_t n);

/** Destroys contents of plist_t
    @param p plist_t instance
    @returns enum VitaError
*/
extern void plist_dtor(plist_t *p);

/** Frees the plist_t instance
    @param p plist_t pointer
*/
extern void plist_free(plist_t *p);

/** Allocates and constructs plist_t
    @param n number of elements
    @returns `plist_t*` upon success, `NULL` otherwise
*/
extern plist_t *plist_create(const size_t n);

/** Deallocates and destroys plist_t
    @param p plist_t pointer
*/
extern void plist_destroy(plist_t *p);

/** Allocates and constructs plist_t from an array
    @param ptr array 
    @param n number of elements

    @returns `plist_t*` upon success, `NULL` otherwise

    @note 
        If ptr == NULL, returns an empty `plist_t` instance
*/
extern plist_t *plist_from(const void *const ptr, const size_t n);

/** Returns length, capacity, available space (capacity - length)
    @param p plist_t pointer
    @returns size_t (length, capacity, available space (capacity - length))
*/
extern size_t plist_len(const plist_t *const p);
extern size_t plist_capacity(const plist_t *const p);
extern size_t plist_has_space(const plist_t *const p);

/** Reserves additional memory of n elements
    @param p plist_t pointer
    @param n number of elements

    @returns enum VitaError
*/
extern enum VitaError plist_reserve(plist_t *const p, const size_t n);

/** Shrinks plist_t capacity to its length
    @param p plist_t pointer
    @returns enum VitaError
*/
extern enum VitaError plist_shrink(plist_t *const p);

/** Sets plist_t length to 0
    @param p plist_t pointer
    @returns enum VitaError
*/
extern enum VitaError plist_clear(plist_t *const p);

/** Assigns a new pointer at an index
    @param p plist_t pointer
    @param ptr pointer value
    @param at index

    @returns enum VitaError
*/
extern enum VitaError plist_set(plist_t *const p, const void *const ptr, const size_t at);

/** Returns a pointer at an index
    @param p plist_t pointer
    @param at index

    @returns void* pointer
*/
extern void *plist_get(const plist_t *const p, const size_t at);

/** Push value at the end
    @param p plist_t pointer
    @param ptr pointer value

    @returns enum VitaError
*/
extern enum VitaError plist_push(plist_t *const p, const void *const ptr);

/** Pop the last value from the end
    @param p plist_t pointer
    @returns enum VitaError
*/
extern enum VitaError plist_pop(plist_t *const p);

/** Get and pop the last value from the end
    @param p plist_t pointer
    @returns void*
*/
extern void *plist_pop_get(plist_t *const p);

/** Removes a pointer from a plist
    @param p plist_t pointer
    @param at index

    @returns enum VitaError

    @note
        enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
        rs_stable: shifts all values by element size
          rs_fast: swaps the last value with the value of `at`
*/
extern enum VitaError plist_remove(plist_t *const p, const size_t at, const enum RemoveStrategy rs);

/** Calls the specified function on each element
    @param p plist_t instance
    @param func function to execute upon each element: func(pointer, for loop index)
*/
extern void plist_apply(const plist_t *const p, void (*func)(void*, size_t));


#endif // VITA_PLIST_H
