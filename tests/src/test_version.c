#include <assert.h>
#include "../../inc/vita/core/version.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {    
    const vt_version_t v = vt_version_get();
    assert(vt_str_equals(v.str, "0.3.1"));

    return 0;
}


