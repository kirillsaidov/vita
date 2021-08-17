#ifndef VITA_CORE_H
#define VITA_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** VITA_CORE MODULE
	- is_null
*/

// array struct wrapper
struct BaseArrayType {
	void* ptr;

	size_t len;
	size_t capacity;
	size_t elsize;
};

/** is_null ==> checks if a pointer is NULL
	params:
		const void* ptr
	returns:
		bool
	notes:
		true 	: NULL
		false 	: otherwise 
*/
extern bool is_null(const void* ptr);

/** gswap ==> swaps values
	params:
		void* a
		void* b
		const size_t elsize
	returns:
		bool
	notes:
		true 	: upon success
		false 	: upon failure
*/
extern bool gswap(void* a, void* b, const size_t elsize);

#endif // VITA_CORE_H






















