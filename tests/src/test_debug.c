#include <assert.h>

// #define NDEBUG
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
    // prints to stderr
    // VT_DEBUG_ASSERT(10 > 13, "This is a debug assert error message!"); // must fail
    // output: 2022-09-09 21:13:32 DEBUG ASSERTION FAILURE (10 > 13) test_debug.c:main:9: This is a debug assert error message!


    VT_DEBUG_DEFAULT_INIT();

        vt_debug_handler_t *mh = vt_debug_handler_default_get_handler();
        assert(mh != NULL);
        
        int *a = VT_DEBUG_MALLOC(sizeof(int));
        a = VT_DEBUG_REALLOC(a, 40);
        a = VT_DEBUG_REALLOC(a, 12);

        #ifndef NDEBUG
            assert(VT_DEBUG_BYTES_CURRENTLY_ALOCATED == 12);
        #endif
        
        a = VT_DEBUG_REALLOC(a, 30);
        VT_VT_DEBUG_FREE(a);
        
        #ifndef NDEBUG
            assert(VT_DEBUG_BYTES_CURRENTLY_ALOCATED == 0);
        #endif

    VT_DEBUG_DEFAULT_QUIT();
    return 0;
}
