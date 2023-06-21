#include <assert.h>
#include "../../inc/vita/core/compiler.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {    
    const vt_version_t v = vt_compiler_get_version();

    #if defined(_WIN32) || defined(_WIN64)
        assert(vt_str_equals(v.str, "13.1.0"));
    #elif defined(__linux__)
        assert(vt_str_equals(v.str, "11.3.0"));
    #else
        //...
    #endif
    
    return 0;
}