#ifndef VITA_PLIST_H
#define VITA_PLIST_H

/** VITA_PLIST MODULE (list of pointers)
	- plist *
	- plist_new *
	- plist_free *

	- plist_len *
	- plist_capacity *
	- plist_has_space *

	- plist_reserve *
	- plist_shrink *
	- plist_set *
	- plist_get *
	- plist_geti *
	- plist_getf *
	- plist_getd *
	- plist_push *
	- plist_pop *
	- plist_remove *
	- plist_foreach *
*/

#include "../core/core.h"
#include "../memory/memhandler.h"
#include "array.h"

// new pointer list type
typedef struct BaseArrayType* plist_pt; 

/** plist ==> creates an empty plist and allocates n+n/3 elements by default with n=DEFAULT_ALLOCATION_SIZE
	params:
		const size_t elsize (element size)
	returns:
		plist_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		allocates n+n/3 elements by default (calls plist_new)
		initializes plist pointers to NULL
*/
extern plist_pt plist(void);

/** plist_new ==> creates an empty plist and allocates N elements
	params:
		const size_t n
		const size_t elsize (element size)
	returns:
		plist_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		preallocates n/3 of the requested size
		initializes plist elements to zero
*/
extern plist_pt plist_new(const size_t n);

/** plist_free ==> frees the plist instance
	params:
		plist_pt plist
*/
extern void plist_free(plist_pt plist);







/** plist_len/capacity/has_length ==> returns length, capacity, available space (capacity - length)
	params:
		plist_pt plist
	returns:
		size_t
*/
extern size_t plist_len(const plist_pt plist);
extern size_t plist_capacity(const plist_pt plist);
extern size_t plist_has_space(const plist_pt plist);







/** plist_reserve ==> reserves additional memory for N elements
	params:
		plist_ptr plist
		const size_t n
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool plist_reserve(plist_pt plist, const size_t n);

/** plist_set ==> assigns a new array value
	params:
		plist_ptr plist
		const size_t index
		const void* ptr
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool plist_set(plist_pt plist, const size_t index, const void* ptr);

/** plist_get ==> returns the a pointer 
	params:
		const plist_ptr plist
		const size_t index
	returns:
		void*
	notes:
		valid ptr 	: upon success
		NULL		: upon failure
*/
extern void* plist_get(const plist_pt plist, const size_t index);

/** plist_push ==> push value at the end of an array  
	params:
		plist_ptr plist
		const void* ptr
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool plist_push(plist_pt plist, const void* ptr);

/** plist_pop ==> pop the last value at the end of an array
	params:
		plist_ptr plist
*/
extern void plist_pop(plist_pt plist);

/** plist_remove ==> removes value from an array
	params:
		plist_ptr plist
		const size_t index
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern void plist_remove(plist_pt plist, const size_t index);

/** plist_foreach ==> iterates through each element and calls func on that element
	params:
		const plist_ptr plist
		void (*func)(void* ptr)
*/
extern void plist_foreach(const plist_pt plist, void (*func)(void* ptr));


#endif // VITA_PLIST_H












































