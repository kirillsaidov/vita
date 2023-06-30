#include <assert.h>
#include "../../inc/vita/core/compiler.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {    
    const vt_version_t v = vt_compiler_get_version();

    #if defined(_WIN32) || defined(_WIN64)
        assert(vt_str_equals(v.str, "13.1.0"));
        printf("windows: %s\n", VT_COMPILER_NAME);
        assert(VT_COMPILER_ID == VT_COMPILER_ID_GCC);
    #elif defined(__linux__)
        assert(vt_str_equals(v.str, "11.3.0"));
        printf("linux: %s\n", VT_COMPILER_NAME);
        assert(VT_COMPILER_ID == VT_COMPILER_ID_GCC);
    #else
        assert(vt_str_equals(v.str, "13.0.0"));
        printf("macos: %s\n", VT_COMPILER_NAME);
        assert(VT_COMPILER_ID == VT_COMPILER_ID_CLANG);
    #endif
    
    return 0;
}