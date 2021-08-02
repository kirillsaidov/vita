#include "vita.h"

enum EVitaInit vita_init(void) {
	// create the internal memory handler
	if(!memhandler_internal_create()) {
		return EVitaInit_MEMHANDLER_ID;
	}

	return EVitaInit_SUCCESS;
}

void vita_flush(void) {
	// free the internal memory handler data
	memhandler_internal_destroy();
}









