#include <assert.h>
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
	// prints to stderr
    // DEBUG_ASSERT(0, "This is a debug assert error message!");

    int *a = DEBUG_MALLOC(sizeof(int));
    *a = 5;
    printf("%d\n", *a);
    DEBUG_FREE(a);

    debug_mh_cleanup();
    return 0;
}
