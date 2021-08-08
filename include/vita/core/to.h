#ifndef VITA_TO_H
#define VITA_TO_H

/** VITA_TO MODULE
	- to_int
	- to_long
	- to_float
	- to_double
	- to_longdouble

	- future: maybe add the same for stdint types as well
*/

#include "as.h"

/** to_type ==> casts void pointer to the specified type pointer and accesses the value (a nice wrapper)
	params:
		const void* ptr
	returns:
		*(type*)ptr
*/
extern int to_int(const void* ptr);
extern long to_long(const void* ptr);
extern float to_float(const void* ptr);
extern double to_double(const void* ptr);
extern long double to_longdouble(const void* ptr);

#endif // VITA_TO_H
























