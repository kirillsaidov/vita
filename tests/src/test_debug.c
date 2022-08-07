#include <assert.h>
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
	// prints to stderr
    DEBUG_ASSERT(0, "This is a debug assert error message!");

    return 0;
}
