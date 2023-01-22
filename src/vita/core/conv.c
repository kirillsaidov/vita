#include "vita/core/conv.h"

int64_t vt_conv_str_to_int64(const char *const z) {
    assert(z != NULL);

    // check if value is numeric
    if(!str_is_numeric(z, VT_MAX_PRECISION)) {
        DEBUG_PRINTF("Must be digits only, not \"<%s>\".\n", z);
        return 0;
    }

    // save value
    return strtoll(z, NULL, 20);
}

uint64_t vt_conv_str_to_uint64(const char *const z) {
    assert(z != NULL);

    // check if value is numeric
    if(!str_is_numeric(z, VT_MAX_PRECISION)) {
        DEBUG_PRINTF("Must be digits only, not \"<%s>\".\n", z);
        return 0;
    }

    // save value
    return strtoull(z, NULL, 20);
}

double vt_conv_str_to_double(const char *const z) {
    assert(z != NULL);

    // check if value is numeric
    if(!str_is_numeric(z, VT_MAX_PRECISION)) {
        DEBUG_PRINTF("Must be digits only, not \"<%s>\".\n", z);
        return 0;
    }

    // save value
    return strtod(z, NULL);
}

real vt_conv_str_to_real(const char *const z) {
    assert(z != NULL);

    // check if value is numeric
    if(!str_is_numeric(z, VT_MAX_PRECISION)) {
        DEBUG_PRINTF("Must be digits only, not \"<%s>\".\n", z);
        return 0;
    }

    // save value
    return strtold(z, NULL);

}


