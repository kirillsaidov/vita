#include "vita/container/plist.h"

vt_plist_t *vt_plist_create(const size_t n, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // allocate a new vt_plist_t instance
    vt_plist_t *p = vt_array_new(alloctr);
    *p = (vt_plist_t) {
        .alloctr = alloctr,
        .ptr2 = alloctr ? VT_ALLOCATOR_ALLOC(alloctr, n * sizeof(void*)) : VT_CALLOC(n * sizeof(void*)),
        .len = 0,
        .capacity = n,
        .elsize = sizeof(void*),
    };

    return p;
}

void vt_plist_destroy(vt_plist_t *p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // free vt_plist_t contents
    if(p->alloctr) {
        VT_ALLOCATOR_FREE(p->alloctr, p->ptr2);
    } else {
        VT_FREE(p->ptr2);
    }
    p->ptr2 = NULL; // reset to zero


    // free plist_t instance itself
    vt_array_free(p);
    p = NULL;
}

vt_plist_t *vt_plist_dup(const vt_plist_t *const p, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // allocate a new vt_vec_t instance
    vt_plist_t *pdup = vt_plist_create(p->len, alloctr);
    pdup->len = p->len;

    // copy values
    memcpy(pdup->ptr2, p->ptr2, p->len * p->elsize);

    return pdup;
}

size_t vt_plist_len(const vt_plist_t *const p) {
    return vt_array_len(p);
}

size_t vt_plist_capacity(const vt_plist_t *const p) {
    return vt_array_capacity(p);
}

size_t vt_plist_has_space(const vt_plist_t *const p) {
    return vt_array_has_space(p);
}

bool vt_plist_is_empty(const vt_plist_t *const p) {
    return !vt_array_len(p);
}

void vt_plist_reserve(vt_plist_t *const p, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // reserve memory for additional n elements
    p->ptr2 = p->alloctr 
        ? VT_ALLOCATOR_REALLOC(p->alloctr, p->ptr2, (p->capacity + n) * p->elsize) 
        : VT_REALLOC(p->ptr2, (p->capacity + n) * p->elsize);

    // update
    p->capacity += n;
}

void vt_plist_shrink(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // if length and capacity are the same, exit the function
    if(p->len == p->capacity) {
        return;
    }

    // shrink to length
    p->ptr2 = p->alloctr 
        ? VT_ALLOCATOR_REALLOC(p->alloctr, p->ptr2, p->len * p->elsize) 
        : VT_REALLOC(p->ptr2, p->len * p->elsize);

    
    // update
    p->capacity = p->len;
}

void vt_plist_resize(vt_plist_t *const p, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    if(n == p->capacity) {
        p->len = p->capacity;
        return;
    }

    // resize vt_vec_t
    p->ptr2 = p->alloctr 
        ? VT_ALLOCATOR_REALLOC(p->alloctr, p->ptr2, n * p->elsize) 
        : VT_REALLOC(p->ptr2, n * p->elsize);

    // update
    p->len = p->capacity = n;
}

void vt_plist_insert(vt_plist_t *const p, const void *const ptr, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_status_error_out_of_bounds_access), 
        at, 
        p->len
    );

    // check if new memory needs to be allocated
    if(!vt_plist_has_space(p)) {
        vt_plist_reserve(p, p->capacity * VT_ARRAY_DEFAULT_GROWTH_RATE);
    }

    // shift values by one value to the end of the vt_vec_t
    memmove(((char*)(p->ptr2) + (at + 1) * p->elsize), ((char*)(p->ptr2) + at * p->elsize), ((p->len - at) * p->elsize));

    // copy the str contents to str from the specified index
    ((char**)p->ptr2)[at] = (char*)ptr;

    // set new length
    p->len++;
}

void vt_plist_clear(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // update length
    p->len = 0;
}

void vt_plist_set(vt_plist_t *const p, const void *const ptr, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_status_error_out_of_bounds_access), 
        at, 
        p->len
    );

    // add ptr to vt_plist_t
    p->ptr2[at] = (void*)ptr;
}

void *vt_plist_get(const vt_plist_t *const p, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_status_error_out_of_bounds_access), 
        at, 
        p->len
    );

    return p->ptr2[at];
}

void vt_plist_push(vt_plist_t *const p, const void *ptr) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    
    // check if new memory needs to be allocated
    if(!vt_plist_has_space(p)) {
        vt_plist_reserve(p, p->capacity * VT_ARRAY_DEFAULT_GROWTH_RATE);
    }

    // add ptr to vt_plist_t
    p->ptr2[p->len++] = (void*)ptr;
}

void vt_plist_pop(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // pop the last element
    if(p->len > 0) {
        p->len--;
    }
}

void *vt_plist_pop_get(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // pop the last element
    if(p->len > 0) {
        return ((char**)p->ptr2)[--p->len];
    }

    return p->ptr2;
}

void vt_plist_remove(vt_plist_t *const p, const size_t at, const enum VitaRemoveStrategy rs) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(rs < vt_remove_stategy_count, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_status_error_out_of_bounds_access), 
        at, 
        p->len
    );

    // check remove strategy
    if(rs == vt_remove_stategy_stable) {
        memmove((char**)(p->ptr2) + at * p->elsize, (char**)(p->ptr2) + (at + 1) * p->elsize, (p->len - at) * p->elsize);
    } else {
        vt_gswap((char**)(p->ptr2) + at * p->elsize, (char**)(p->ptr2) + (p->len - 1) * p->elsize, p->elsize);
    }

    // update length
    p->len--;
}

void **vt_plist_slide_front(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    return (void**)vt_array_slide_front(p);
}

void **vt_plist_slide_back(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    return (void**)vt_array_slide_back(p);
}

void vt_plist_slide_reset(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    vt_array_slide_reset(p);
}

void vt_plist_apply(const vt_plist_t *const p, void (*func)(void*, size_t)) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(func != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // iterate
    const size_t len = vt_plist_len(p);
    for(size_t i = 0; i < len; i++) {
        func(((char**)p->ptr2)[i], i);
    }
}
