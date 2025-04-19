#include "vita/container/common.h"

struct VitaBaseArrayType *vt_array_new(struct VitaBaseAllocatorType *const alloctr) {
    struct VitaBaseArrayType *vbat = (struct VitaBaseArrayType*)(alloctr
        ? VT_ALLOCATOR_ALLOC(alloctr, sizeof(struct VitaBaseArrayType))
        : VT_CALLOC(sizeof(struct VitaBaseArrayType))
    );
    vbat->alloctr = alloctr;

    return vbat;
}

void vt_array_free(struct VitaBaseArrayType *vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // free the VitaBaseArrayType
    if (vbat->alloctr) {
        VT_ALLOCATOR_FREE(vbat->alloctr, vbat);
    } else {
        VT_FREE(vbat);
    }

    // reset to NULL
    vbat = NULL;
}

void *vt_array_head(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    
    return vbat->ptr;
}

size_t vt_array_len(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return vbat->len;
}

size_t vt_array_capacity(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return vbat->capacity;
}

size_t vt_array_has_space(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return (vbat->capacity - vbat->len);
}

size_t vt_array_elsize(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return vbat->elsize;
}

bool vt_array_is_view(const struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return vbat->is_view;
}

bool vt_array_has_alloctr(const struct VitaBaseArrayType *const vbat) {
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return !(vbat->alloctr == NULL);
}

void *vt_array_get(const struct VitaBaseArrayType *const vbat, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(
        at < vbat->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        vbat->len
    );

    return ((char*)(vbat->ptr) + at * vbat->elsize);
}

void vt_array_set(const struct VitaBaseArrayType *const vbat, const void *const val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < vbat->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        vbat->len
    );

    // copy val data to vt_str_t
    memcpy(((char*)(vbat->ptr) + at * vbat->elsize), val, vbat->elsize);
}

void *vt_array_slide_front(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check bounds
    if (vbat->slider_idx < vbat->len) {
        vbat->slider_idx++;
        return (char*)vbat->ptr + (vbat->slider_idx - 1) * vbat->elsize;
    }

    // reset the slider
    vt_array_slide_reset(vbat);

    return NULL;
}

void *vt_array_slide_back(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check bounds
    if (vbat->slider_idx < vbat->len) {
        vbat->slider_idx++;
        return (char*)vbat->ptr + (vbat->len - vbat->slider_idx - 2) * vbat->elsize;
    }

    // reset the slider
    vt_array_slide_reset(vbat);

    return NULL;
}

void vt_array_slide_reset(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    vbat->slider_idx = 0;
}

size_t vt_index_2d_to_1d(const size_t row, const size_t col, const size_t ncols) {
    return (ncols * row + col);
}

void vt_index_1d_to_2d(size_t *const row, size_t *const col, const size_t idx, const size_t ncols) {
    // check for invalid input
    VT_DEBUG_ASSERT(row != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(col != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    *row = (size_t)(idx / ncols);
    *col = (size_t)(idx % ncols);
}

size_t vt_index_3d_to_1d(const size_t row, const size_t col, const size_t depth, const size_t nrows, const size_t ncols) {
    return (depth * ncols * nrows) + row * ncols + col;
}

void vt_index_1d_to_3d(size_t *const row, size_t *const col, size_t *const depth, const size_t idx, const size_t nrows, const size_t ncols) {
    // check for invalid input
    VT_DEBUG_ASSERT(row != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(col != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(depth != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    *depth = (size_t)(idx / (nrows * ncols));
    const size_t idx_offset = idx - *depth * nrows * ncols;
    *row = (size_t)(idx_offset / ncols);
    *col = (size_t)(idx_offset % ncols);
}


