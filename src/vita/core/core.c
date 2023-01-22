#include "vita/core/core.h"

// generate vita error strings
static const char *const vita_error_str[] = {
    VT_i_GENERATE_VITA_ERRORS(VT_STRINGOF)
};

/* ------------- BASE CONTAINER TYPE ------------- */

void *vt_bct_head(const struct VitaBaseContainerType *const bct) {
    assert(bct != NULL);
    return bct->ptr;
}

size_t vt_bct_len(const struct VitaBaseContainerType *const bct) {
    assert(bct != NULL);
    return bct->len;
}

size_t vt_bct_capacity(const struct VitaBaseContainerType *const bct) {
    assert(bct != NULL);
    return bct->capacity;
}

size_t vt_bct_elsize(const struct VitaBaseContainerType *const bct) {
    assert(bct != NULL);
    return bct->elsize;
}

size_t vt_index_2d_to_1d(const size_t row, const size_t col, const size_t ncols) {
    return (ncols * row + col);
}

void vt_index_1d_to_2d(size_t *const row, size_t *const col, const size_t idx, const size_t ncols) {
    *row = (size_t)(idx / ncols);
    *col = (size_t)(idx % ncols);
}

/* ------------- OTHER FUNCTIONALITY ------------- */

bool vt_gswap(void* a, void* b, const size_t elsize) {
    // checks
    assert(a != NULL);
    assert(b != NULL);
    assert(elsize != 0);

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

void vt_get_current_timestamp(char *timebuf, const size_t len) {
    const size_t timestamp_size = 21;
    if(len < timestamp_size) {
        return;
    }

    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    timebuf[strftime(timebuf, timestamp_size, "%Y-%m-%d %H:%M:%S", stm)] = '\0';
}

const char *vt_get_vita_error_str(const enum VitaError e) {
    if(e < vt_ve_count) {
        return vita_error_str[e];
    }

    return NULL;
}
