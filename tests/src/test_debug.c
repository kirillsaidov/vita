#include <assert.h>
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
    // prints to stderr
    // DEBUG_ASSERT(0, "This is a debug assert error message!");

    debug_mh_handler_default_create();

        debug_mh_t *mh = debug_mh_handler_default_get_handler();
        assert(mh != NULL);
        
        int *a = DEBUG_MALLOC(sizeof(int));
        a = DEBUG_REALLOC(a, 40);
        a = DEBUG_REALLOC(a, 12);
        assert(DEBUG_BYTES_CURRENTLY_ALOCATED == 12);

        a = DEBUG_REALLOC(a, 30);
        DEBUG_FREE(a);
        assert(DEBUG_BYTES_CURRENTLY_ALOCATED == 0);

    debug_mh_handler_default_destroy();
    return 0;
}
