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
	- plist_foreach
*/

#include "../core/core.h"

// new pointer list type
typedef struct BaseArrayType plist_t;

/**
Allocates memory for plist_t

Returns: `plist_t*` upon success, `NULL` upon failure
*/
extern plist_t *plist_new(void);

/**
Constructs plist_t

Params:
	p = plist_t instance
	n = number of elements

Returns: enum ContainerError
*/
extern enum ContainerError plist_ctor(plist_t *p, const size_t n);

/**
Destroys contents of plist_t

Params:
	p = plist_t instance

Returns: enum ContainerError
*/
extern void plist_dtor(plist_t *p);

/**
Frees the plist_t instance

Params:
	p = plist_t pointer
*/
extern void plist_free(plist_t *p);

/**
Allocates and constructs plist_t

Params:
	n = number of elements

Returns: `plist_t*` upon success, `NULL` otherwise
*/
extern plist_t *plist_create(const size_t n);

/**
Deallocates and destroys plist_t

Params:
	p = plist_t pointer
*/
extern void plist_destroy(plist_t *p);

/**
Returns length, capacity, available space (capacity - length)

Params:
	p = plist_t pointer

Returns: size_t (length, capacity, available space (capacity - length))
*/
extern size_t plist_len(const plist_t *const p);
extern size_t plist_capacity(const plist_t *const p);
extern size_t plist_has_space(const plist_t *const p);

/**
Reserves additional memory of n elements

Params:
	p = plist_t pointer
	n = number of elements

Returns: enum ContainerError
*/
extern enum ContainerError plist_reserve(plist_t *const p, const size_t n);

/**
Shrinks plist_t capacity to its length

Params:
	p = plist_t pointer

Returns: enum ContainerError
*/
extern enum ContainerError plist_shrink(plist_t *const p);

/**
Sets plist_t length to 0

Params:
	p = plist_t pointer

Returns: enum ContainerError
*/
extern enum ContainerError plist_clear(plist_t *const p);

/**
Assigns a new pointer at an index

Params:
	p = plist_t pointer
	ptr = pointer value
	at = index

Returns: enum ContainerError
*/
extern enum ContainerError plist_set(plist_t *const p, const void *ptr, const size_t at);

/**
Returns a pointer at an index

Params:
	p = plist_t pointer
	at = index

Returns: void* pointer
*/
extern void *plist_get(const plist_t *const p, const size_t at);

/**
Push value at the end

Params:
	p = plist_t pointer
	ptr = pointer value

Returns: enum ContainerError
*/
extern enum ContainerError plist_push(plist_t *const p, const void *ptr);

/**
Pop the last value from the end
Params:
	p = plist_t pointer

Returns: enum ContainerError
*/
extern enum ContainerError plist_pop(plist_t *const p);

/**
Get and pop the last value from the end
Params:
	p = plist_t pointer

Returns: void*
*/
extern void *plist_pop_get(plist_t *const p);

/**
Removes a pointer from a plist

Params:
	p = plist_t pointer
	at = index

Returns: enum ContainerError

Notes:
	enum RemoveStrategy { rs_stable = ordered removal, rs_fast = unordered removal }
	rs_stable:	shifts all values by element size
	rs_fast:	swaps the last value with the value of `at`
*/
extern enum ContainerError plist_remove(plist_t *const p, const size_t at, const enum RemoveStrategy rs);

/**
Calls the specified function on each element

Params:
	p = plist_t instance
	func = function to execute upon each element: func(pointer, for loop index)
*/
extern void plist_foreach(const plist_t *const p, void (*func)(void*, size_t));


#endif // VITA_PLIST_H
