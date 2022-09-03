#include "vita/core/core.h"

// generate vita error strings
#define X(a) STRINGOF(a),
const char *const vita_error_str[] = {
    ___GENERATE_VITA_ERRORS(X)
};
#undef X

/* ------------- BASE CONTAINER TYPE ------------- */

void *bct_head(const struct BaseContainerType *const bct) {
    return (bct == NULL) ? NULL : bct->ptr;
}

size_t bct_len(const struct BaseContainerType *const bct) {
    return (bct == NULL) ? 0 : bct->len;
}

size_t bct_capacity(const struct BaseContainerType *const bct) {
    return (bct == NULL) ? 0 : bct->capacity;
}

size_t bct_rows(const struct BaseContainerType *const bct) {
    return (bct == NULL) ? 0 : bct->rows;
}

size_t bct_cols(const struct BaseContainerType *const bct) {
    return (bct == NULL) ? 0 : bct->cols;
}

size_t bct_elsize(const struct BaseContainerType *const bct) {
    return (bct == NULL) ? 0 : bct->elsize;
}

/* ------------- OTHER FUNCTIONALITY ------------- */

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

const char *const get_vita_error_str(const enum VitaError e) {
    if(e >= 0 && e < ve_count) {
        return vita_error_str[e];
    }

    return NULL;
}
