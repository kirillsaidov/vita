#include <assert.h>
#include "../../inc/vita/core/version.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {    
    const version_t v = get_vita_version();
    assert(str_equals(v.str, "0.3.0"));

    return 0;
}


