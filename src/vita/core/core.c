#include "vita/core/core.h"

// generate vita error strings
#define X(a) VT_STRING_OF(a),
static const char *const vita_error_str[] = {
    VT_i_GENERATE_VITA_STATUS(X)
};
#undef X

/* -------------- MEMORY MANAGEMENT -------------- */

void *vt_malloc(const size_t bytes, const char *const file, const char *const func, const size_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr = malloc(bytes);
    if(ptr == NULL) {
        fprintf(stderr, "%s %s:%s:%zu: %s\n", "MEMORY ALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *vt_calloc(const size_t bytes, const char *const file, const char *const func, const size_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr = calloc(1, bytes);
    if(ptr == NULL) {
        fprintf(stderr, "%s %s:%s:%zu: %s\n", "MEMORY ALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *vt_realloc(void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr_new = realloc(ptr, bytes);
    if(ptr_new == NULL) {
        fprintf(stderr, "%s %s:%s:%zu: %s\n", "MEMORY REALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr_new;
}

void vt_free(void *ptr) {
    if(ptr == NULL) {
        return;
    }

    free(ptr);
}

/* ------------- OTHER FUNCTIONALITY ------------- */

bool vt_gswap(void* a, void* b, const size_t elsize) {
    // checks
    assert(a != NULL);
    assert(b != NULL);
    assert(elsize != 0);

    // allocate a temporary variable
    void* temp = VT_CALLOC(elsize);
    if(temp == NULL) {
        return false;
    }

    // swap values
    memcpy(temp, a, elsize);    // copy a to temp
    memcpy(a, b, elsize);       // copy b to a
    memcpy(b, temp, elsize);    // copy temp(a) to b

    // free temporary variable
    VT_FREE(temp);

    return true;
}

const char *vt_get_vita_error_str(const enum VitaStatus e) {
    if(e < vt_status_count) {
        return vita_error_str[e];
    }

    return NULL;
}
