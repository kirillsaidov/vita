#include "vita/core/core.h"

bool vitaWarnings = true;

bool is_null(const void* ptr) {
	return (ptr == NULL);
}

bool gswap(void* a, void* b, const size_t elsize) {
	if(is_null(a) || is_null(b) || elsize == 0) {
		return false;
	}

	// allocate a temporary variable
	void* temp = malloc(elsize);
	if(is_null(temp)) {
		return false;
	}

	// swap values
	memcpy(temp, a, elsize); 	// copy a to temp
	memcpy(a, b, elsize);		// copy b to a
	memcpy(b, temp, elsize);	// copy temp(a) to b
	
	// free temporary variable
	free(temp);

	return true;
}

void vita_warn(const char *const msg, const char *const topic) {
	if(is_null(msg) || !vitaWarnings) {
		return;
	}

	fprintf(stderr, "# vita_warn: [%s] => %s\n", (is_null(topic) ? "" : topic), msg);
}

















