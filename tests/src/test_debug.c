#include <assert.h>
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
	// prints to stderr
    // DEBUG_ASSERT(0, "This is a debug assert error message!");

    debug_mh_handler_default_create();

    debug_mh_t *mh = debug_mh_handler_default_get_handler();
    
    debug_mh_handler_default_destroy();
    return 0;
}
