#include "vita/core/core.h"

bool is_null(const void* ptr) {
	return (ptr == NULL);
}

bool gswap(void* a, void* b, const size_t elsize) {
	if(is_null(a) || is_null(b) || elsize == 0) {
		return false;
	}

	void* temp = malloc(elsize);
	if(is_null(temp)) {
		return false;
	}

	// swap values
	memcpy(temp, a, elsize); 	// copy a to temp
	memcpy(a, b, elsize);		// copy b to a
	memcpy(b, temp, elsize);	// copy temp(a) to b
	
	free(temp);

	return true;
}

















