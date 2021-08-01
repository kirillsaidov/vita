#ifndef VITA_CORE_H
#define VITA_CORE_H

#include <stdio.h>
#include <stdbool.h>

/** VITA_CORE MODULE
	- is_null
*/

/** is_null ==> checks if a pointer is NULL
	params:
		const void* ptr
	returns:
		bool
	notes:
		true - if ptr is NULL
		false - if ptr is not NULL 
*/
extern bool is_null(const void* ptr);

#endif // VITA_CORE_H






















