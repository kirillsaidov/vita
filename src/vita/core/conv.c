#include "vita/core/conv.h"

int64_t conv_str_to_int64(const char *const z) {
    if(z == NULL) {
        DEBUG_ASSERT(z != NULL, "Value supplied is NULL!");
        return 0;
    }

    // check if value is numeric
    if(!str_is_numeric(z, MAX_PRECISION)) {
        DEBUG_PRINT("Must be digits only, not \"%s\".\n", z);
        return 0;
    }

    // save value
    return strtoll(z, NULL, 10);
}

uint64_t conv_str_to_uint64(const char *const z) {
    if(z == NULL) {
        DEBUG_ASSERT(z != NULL, "Value supplied is NULL!");
        return 0;
    }

    // check if value is numeric
    if(!str_is_numeric(z, MAX_PRECISION)) {
        DEBUG_PRINT("Must be digits only, not \"%s\".\n", z);
        return 0;
    }

    // save value
    return strtoull(z, NULL, 10);
}

double conv_str_to_double(const char *const z) {
    if(z == NULL) {
        DEBUG_ASSERT(z != NULL, "Value supplied is NULL!");
        return 0;
    }

    // check if value is numeric
    if(!str_is_numeric(z, MAX_PRECISION)) {
        DEBUG_PRINT("Must be digits only, not \"%s\".\n", z);
        return 0;
    }

    // save value
    return strtod(z, NULL);
}


