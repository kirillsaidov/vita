#include "vita/core/core.h"

// enum DataType strings
const char *const dtstr[] = {
    "dt_int",
    "dt_float",
    "dt_bool",
    "dt_char",
    "dt_str"
};

bool gswap(void* a, void* b, const size_t elsize) {
    if(a == NULL || b == NULL || elsize == 0) {
        return false;
    }

    // allocate a temporary variable
    void* temp = calloc(1, elsize);
    if(temp == NULL) {
        return false;
    }

    // swap values
    memcpy(temp, a, elsize);    // copy a to temp
    memcpy(a, b, elsize);       // copy b to a
    memcpy(b, temp, elsize);    // copy temp(a) to b

    // free temporary variable
    free(temp);

    return true;
}

const char *const dt_to_str(const enum DataType dt) {
    if(dt >= 0 && dt < dt_count) {
        return dtstr[dt];
    }

    return NULL;
}

