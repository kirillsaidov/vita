#include <assert.h>

#include "../../inc/vita/util/log.h"

int main(void) {
    assert(log_get_level_string(ll_fatal) == "FATAL");
    assert(log_get_level_string(-1) == "INFO" && log_get_level_string(ll_count) == "INFO");
    
    // ...

    return 0;
}


