#ifndef VITA_PLIST_H
#define VITA_PLIST_H

/** VITA_PLIST MODULE (dynamic array(list) of pointers)
	- plist_t *
	- plist_free *

	- plist_len *
	- plist_capacity *
	- plist_has_space *

	- plist_reserve *
	- plist_shrink *
	- plist_set *
	- plist_get *
	- plist_push *
	- plist_pop *
	- plist_remove *
	- plist_foreach *
*/

#include "../core/core.h"

// new pointer list type
typedef struct BaseArrayType plist_t; 

/** 
Creates an empty plist_t

Params:
	n = number of elements

Returns: plist_t* instance, `NULL` upon failure
*/
extern plist_t *plist(const size_t n);

/** 
Frees the plist_t instance

Params:
	p = plist_t pointer
*/
extern void plist_free(plist_t *p);







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

Returns: `true` upon success
*/
extern bool plist_reserve(plist_t *const p, const size_t n);

/** 
Shrinks plist_t capacity to its length

Params:
	p = plist_t pointer
	n = number of elements

Returns: `true` upon success
*/
extern bool plist_shrink(plist_t *const p, const size_t n);

/** 
Assigns a new pointer at an index

Params:
	p = plist_t pointer
	ptr = pointer value
	at = index

Returns: `true` upon success
*/
extern bool plist_set(plist_t *const p, const void *ptr, const size_t at);

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

Returns: `true` upon success
*/
extern bool plist_push(plist_t *const p, const void *ptr);

/** 
Pop the last value from the end
Params:
	p = plist_t pointer
*/
extern void plist_pop(plist_t *const p);

/**
Removes a pointer from a plist

Params:
	p = plist_t pointer
	at = index

Returns: `true` upon success
*/
extern bool plist_remove(plist_t *const p, const size_t at);

/** plist_foreach ==> iterates through each element and calls func on that element
Params:
	p = plist_t pointer
	func = function to execute action on each element: func(pointer, for loop index)
*/
extern void plist_foreach(const plist_t *const p, void (*func)(void*, size_t));


#endif // VITA_PLIST_H












































