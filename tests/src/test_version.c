#include <assert.h>
#include "../../inc/vita/core/version.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {    
    const vt_version_t v = vt_version_get();
    assert(vt_str_equals_z(v.str, "0.5.0"));

    return 0;
}


