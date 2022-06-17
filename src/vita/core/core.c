#include "vita/core/core.h"

// dt strings
const char *const dtstr[] = {
    "dt_int",
    "dt_float",
    "dt_char",
    "dt_str"
};

bool gswap(void* a, void* b, const size_t elsize) {
    if(a == NULL || b == NULL || elsize == 0) {
        return false;
    }

    // allocate a temporary variable
    void* temp = malloc(elsize);
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
    if(dt < dt_count) {
        return dtstr[dt];
    }

    return "";
}

