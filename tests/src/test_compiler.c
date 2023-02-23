#include <assert.h>
#include "../../inc/vita/core/compiler.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {    
    const vt_version_t v = vt_compiler_get_version();
    assert(vt_str_equals(v.str, "11.3.0"));

    return 0;
}