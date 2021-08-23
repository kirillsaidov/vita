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
	- plist_push *
	- plist_pop *
	- plist_remove *
	- plist_clear *
	- plist_foreach *
*/

#include "../core/core.h"
#include "../memory/memhandler.h"
#include "array.h"

// new pointer list type
typedef struct BaseArrayType* plist_pt; 

/** plist ==> creates an empty plist and allocates 7 elements by default
	params:
		const size_t elsize (element size)
	returns:
		plist_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		allocates 9 elements by default (calls plist_new)
		initializes plist pointers to NULL
*/
extern plist_pt plist();

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

#endif // VITA_PLIST_H












































