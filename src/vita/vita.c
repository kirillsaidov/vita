#include "vita.h"

enum EVitaInit vita_init(void) {
	// create the internal memory handler for memhandler
	if(!memhandler_internal_create()) {
		return EVitaInit_MEMHANDLER_ID;
	}

	// create the internal memory handler for strbuf
	if(!strbuf_memhandler_internal_create()) {
		return EVitaInit_STRBUF_ID;
	}

	return EVitaInit_SUCCESS;
}

void vita_flush(void) {
	// free the internal memory handler data for memhandler
	memhandler_internal_destroy();

	// free the internal memory handler data for strbuf
	strbuf_memhandler_internal_destroy();
}









