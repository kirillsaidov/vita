#include <assert.h>

// #define NDEBUG
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
    // prints to stderr
    // DEBUG_ASSERT(10 > 13, "This is a debug assert error message!"); // must fail
    // output: 2022-09-09 21:13:32 DEBUG ASSERTION FAILURE (10 > 13) test_debug.c:main:9: This is a debug assert error message!


    DEBUG_DEFAULT_INIT();

        debug_handler_t *mh = debug_handler_default_get_handler();
        assert(mh != NULL);
        
        int *a = DEBUG_MALLOC(sizeof(int));
        a = DEBUG_REALLOC(a, 40);
        a = DEBUG_REALLOC(a, 12);

        #ifndef NDEBUG
            assert(DEBUG_BYTES_CURRENTLY_ALOCATED == 12);
        #endif
        
        a = DEBUG_REALLOC(a, 30);
        DEBUG_FREE(a);
        
        #ifndef NDEBUG
            assert(DEBUG_BYTES_CURRENTLY_ALOCATED == 0);
        #endif

    DEBUG_DEFAULT_QUIT();
    return 0;
}
