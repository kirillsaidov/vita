#include "vita/core/core.h"

// generate vita error strings
#define X(a) VT_STRING_OF(a),
static const char *const vt_error_str[] = {
    VT_i_GENERATE_VITA_STATUS(X)
};
#undef X

/* -------------- MEMORY MANAGEMENT -------------- */

void *vt_malloc(const size_t bytes, const char *const file, const char *const func, const size_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr = malloc(bytes);
    if (ptr == NULL) {
        fprintf(stderr, "%s %s:%s:%zu: %s\n", "MEMORY ALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *vt_calloc(const size_t bytes, const char *const file, const char *const func, const size_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr = calloc(1, bytes);
    if (ptr == NULL) {
        fprintf(stderr, "%s %s:%s:%zu: %s\n", "MEMORY ALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *vt_realloc(void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr_new = realloc(ptr, bytes);
    if (ptr_new == NULL) {
        fprintf(stderr, "%s %s:%s:%zu: %s\n", "MEMORY REALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr_new;
}

void vt_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    free(ptr);
}

void *vt_memmove(void *dest, const void *const src, const size_t bytes) {
    assert(src != NULL);
    assert(dest != NULL);

    // do nothing
    if (bytes == 0) {
        return dest;
    }

    // copy data
    memmove(dest, src, bytes);

    return dest;
}

void *vt_memcopy(void *dest, const void *const src, const size_t bytes) {
    assert(src != NULL);
    assert(dest != NULL);

    // do nothing
    if (bytes == 0) {
        return dest;
    }

    // copy data
    memcpy(dest, src, bytes);

    return dest;
}

void *vt_memset(void *ptr, const int32_t value, const size_t bytes) {
    assert(ptr != NULL);

    // do nothing
    if (bytes == 0) {
        return ptr;
    }

    // set value
    memset(ptr, value, bytes);

    return ptr;
}

bool vt_memcmp(const void *lhs, const void *rhs, const size_t bytes) {
    assert(lhs != NULL);
    assert(rhs != NULL);

    if (bytes == 0) {
        return false;
    }

    return (memcmp(lhs, rhs, bytes) == 0);
}

size_t vt_strnlen(const char *const z, const size_t max_len) {
    assert(z != NULL);

    // examine string length
    size_t len = 0;
    while (len < max_len && z[len] != '\0') {
        len++;
    }
    return len;
}

const char *vt_strnstr(const char *const haystack, const size_t haystack_len, const char *const needle, const size_t needle_len) {
    assert(haystack != NULL);
    assert(needle != NULL);
    
    // check for invalid inputs
    if (!needle_len) return haystack;
    if (haystack_len < needle_len) return NULL;

    // find needle in haystack
    size_t max_index = haystack_len - needle_len;
    for (size_t i = 0; i <= max_index; i++) {
        if (vt_memcmp(haystack + i, needle, needle_len)) {
            return (haystack + i);
        }
    }

    return NULL;
}

/* ------------- OTHER FUNCTIONALITY ------------- */

void vt_gswap(void* a, void* b, const size_t elsize) {
    assert(a != NULL);
    assert(b != NULL);
    assert(elsize != 0);

    // allocate a temporary variable
    void *temp = VT_CALLOC(elsize);

    // swap values
    memcpy(temp, a, elsize);    // copy a to temp
    memcpy(a, b, elsize);       // copy b to a
    memcpy(b, temp, elsize);    // copy temp(a) to b

    // free temporary variable
    VT_FREE(temp);
}

void vt_pswap(void **a, void **b) {
    assert(a != NULL);
    assert(b != NULL);

    // swap values
    void *temp = *a;
    *a = *b;
    *b = temp;
}

const char *vt_status_to_str(const enum VitaStatus e) {
    if (e < VT_STATUS_COUNT) {
        return vt_error_str[e];
    }

    return NULL;
}
