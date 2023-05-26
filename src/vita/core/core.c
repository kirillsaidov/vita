#include "vita/core/core.h"

// generate vita error strings
#define X(a) VT_STRING_OF(a),
static const char *const vita_error_str[] = {
    VT_i_GENERATE_VITA_STATUS(X)
};
#undef X

/* ------------- BASE CONTAINER TYPE ------------- */

struct VitaBaseArrayType *vt_array_new(void) {
    return (struct VitaBaseArrayType*)VT_CALLOC(sizeof(struct VitaBaseArrayType));
}

void vt_array_free(struct VitaBaseArrayType *vbat) {
    // check for invalid input
    assert(vbat != NULL);

    // free allocator
    VT_FREE(vbat->alloctr);
    vbat->alloctr = NULL;

    // free the VitaBaseArrayType
    VT_FREE(vbat);

    // reset to NULL
    vbat = NULL;
}

void *vt_array_head(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);
    
    return vbat->ptr;
}

size_t vt_array_len(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    return vbat->len;
}

size_t vt_array_capacity(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    return vbat->capacity;
}

size_t vt_array_has_space(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    return (vbat->capacity - vbat->len);
}

size_t vt_array_elsize(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    return vbat->elsize;
}

void *vt_array_slide_front(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    // check bounds
    if(vbat->slider_idx < vbat->len) {
        vbat->slider_idx++;
        return (char*)vbat->ptr + (vbat->slider_idx - 1) * vbat->elsize;
    }

    // reset the slider
    vt_array_slide_reset(vbat);

    return NULL;
}

void *vt_array_slide_back(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    // check bounds
    if(vbat->slider_idx < vbat->len) {
        vbat->slider_idx++;
        return (char*)vbat->ptr + (vbat->len - vbat->slider_idx - 2) * vbat->elsize;
    }

    // reset the slider
    vt_array_slide_reset(vbat);

    return NULL;
}

void vt_array_slide_reset(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    assert(vbat != NULL);
    assert(vbat->ptr != NULL);

    vbat->slider_idx = 0;
}

size_t vt_index_2d_to_1d(const size_t row, const size_t col, const size_t ncols) {
    return (ncols * row + col);
}

void vt_index_1d_to_2d(size_t *const row, size_t *const col, const size_t idx, const size_t ncols) {
    // check for invalid input
    assert(row != NULL);
    assert(col != NULL);
    
    *row = (size_t)(idx / ncols);
    *col = (size_t)(idx % ncols);
}

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
