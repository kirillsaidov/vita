#include "vita/core/to.h"

extern int to_int(const void* ptr) {
	return *as_int(ptr);
}

extern long to_long(const void* ptr) {
	return *as_long(ptr);
}

extern float to_float(const void* ptr) {
	return *as_float(ptr);
}

extern double to_double(const void* ptr) {
	return *as_double(ptr);
}

extern long double to_longdouble(const void* ptr) {
	return *as_longdouble(ptr);
}
