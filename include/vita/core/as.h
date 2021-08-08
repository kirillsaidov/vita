#ifndef VITA_AS_H
#define VITA_AS_H

/** VITA_AS MODULE
	- as_int
	- as_long
	- as_float
	- as_double
	- as_longdouble
	- as_str

	- future: maybe add the same for stdint types as well
*/

/** as_type ==> casts void pointer to the specified type pointer (fancy wrapper)
	params:
		const void* ptr
	returns:
		(type*)ptr
*/
extern int* as_int(const void* ptr);
extern long* as_long(const void* ptr);
extern float* as_float(const void* ptr);
extern double* as_double(const void* ptr);
extern long double* as_longdouble(const void* ptr);
extern char* as_str(const void* ptr);

#endif // VITA_AS_H
























