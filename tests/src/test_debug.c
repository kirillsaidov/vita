#include <assert.h>

// #define NDEBUG
#include "../../inc/vita/util/debug.h"

int32_t main(void) {
    // prints to stderr
    // VT_DEBUG_ASSERT(10 > 13, "This is a debug assert error message!"); // must fail
    // output: 2022-09-09 21:13:32 DEBUG ASSERTION FAILURE [10 > 13] test_debug.c:main:9: This is a debug assert error message!

    VT_CHECK(1 == 2, "If you read this, then the condition failed(%s)!", "\\:-:/");
    VT_DEBUG_PRINTF("This is a DEBUG message(%d)!\n", 123);

    VT_NOTE("This is a note.");
    // VT_REQUIRED("AAA module is required to run this."); // outputs msg and exits

    // exits the program
    // VT_TODO("Will be added later.");
    // VT_UNIMPLEMENTED("Wait for the next release.");
    
    return 0;
}
