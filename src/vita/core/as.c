#include "vita/core/as.h"

int* as_int(const void* ptr) {
	return (int*)ptr;
}

long* as_long(const void* ptr) {
	return (long*)ptr;
}

float* as_float(const void* ptr) {
	return (float*)ptr;
}

double* as_double(const void* ptr) {
	return (double*)ptr;
}

long double* as_longdouble(const void* ptr) {
	return (long double*)ptr;
}

char* as_str(const void* ptr) {
	return (char*)ptr;
}































