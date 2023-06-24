#include "vita/container/common.h"

struct VitaBaseArrayType *vt_array_new(struct VitaBaseAllocatorType *const alloctr) {
    if(alloctr == NULL) {
        return (struct VitaBaseArrayType*)VT_CALLOC(sizeof(struct VitaBaseArrayType));
    }

    // use allocator
    struct VitaBaseArrayType *vbat = (struct VitaBaseArrayType*)VT_ALLOCATOR_ALLOC(alloctr, sizeof(struct VitaBaseArrayType));
    vbat->alloctr = alloctr;

    return vbat;
}

void vt_array_free(struct VitaBaseArrayType *vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // free the VitaBaseArrayType
    if(vbat->alloctr) {
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

bool vt_array_has_alloctr(const struct VitaBaseArrayType *const vbat) {
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return !(vbat->alloctr == NULL);
}

void *vt_array_slide_front(struct VitaBaseArrayType *const vbat) {
    // check for invalid input
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

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
    VT_DEBUG_ASSERT(vbat != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vbat->ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

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


