#include "vita/core/core.h"

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

void get_current_timestamp(char *timebuf, const size_t len) {
    const size_t timestamp_size = 21;
    if(len < timestamp_size) {
        return;
    }

    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    timebuf[strftime(timebuf, timestamp_size, "%Y-%m-%d %H:%M:%S", stm)] = '\0';
}
