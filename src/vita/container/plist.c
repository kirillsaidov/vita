#include "vita/container/plist.h"

vt_plist_t *vt_plist_new(void) {
    vt_plist_t *p = VT_DEBUG_CALLOC(sizeof(vt_plist_t));
    if(p == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    return p;
}

enum VitaError vt_plist_ctor(vt_plist_t *p, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // vt_plist_t init
    *p = (vt_plist_t) {
        .ptr2 = VT_DEBUG_CALLOC(n * sizeof(void*)),
        .len = 0,
        .capacity = n,
        .elsize = sizeof(void*),
    };

    // checking if p->ptr2 was allocated
    if(p->ptr2 == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    return vt_ve_operation_success;
}

void vt_plist_dtor(vt_plist_t *p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // free vt_plist_t contents
    VT_DEBUG_FREE(p->ptr2);

    // default-init
    *p = (vt_plist_t) {0};
}

void vt_plist_free(vt_plist_t *p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // free the vt_plist_t itself
    VT_DEBUG_FREE(p);

    // reset to NULL
    p = NULL;
}

vt_plist_t *vt_plist_create(const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate a new plist_t instance
    vt_plist_t *p = vt_plist_new();
    if(p == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }
    
    // construct plist_t instance
    if(vt_plist_ctor(p, n) != vt_ve_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        vt_plist_free(p);
        return NULL;
    }

    return p;
}

void vt_plist_destroy(vt_plist_t *p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // destroy
    vt_plist_dtor(p);
    vt_plist_free(p);
}

vt_plist_t *vt_plist_from(const void **const ptr, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(n > 0 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    vt_plist_t *p = vt_plist_create(n);
    if(p == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // copy values
    memmove(p->ptr2, ptr, n);
    p->len = n;

    return p;
}

size_t vt_plist_len(const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return p->len;
}

size_t vt_plist_capacity(const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return p->capacity;
}

size_t vt_plist_has_space(const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return (p->capacity - p->len);
}

bool vt_plist_is_empty(const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return !(p->len);
}

enum VitaError vt_plist_reserve(vt_plist_t *const p, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // reserve memory for additional n elements
    void **newptr = VT_DEBUG_REALLOC(p->ptr2, (p->capacity + n) * p->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // update values
    p->ptr2 = newptr;
    p->capacity += n;

    return vt_ve_operation_success;
}

enum VitaError vt_plist_shrink(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // if length and capacity are the same, exit the function
    if(p->len == p->capacity) {
        return vt_ve_operation_success;
    }

    // shrink to length
    void **newptr = VT_DEBUG_REALLOC(p->ptr2, p->len * p->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // update values
    p->ptr2 = newptr;
    p->capacity = p->len;

    return vt_ve_operation_success;
}

enum VitaError vt_plist_clear(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // update length
    p->len = 0;

    return vt_ve_operation_success;
}

enum VitaError vt_plist_set(vt_plist_t *const p, const void *const ptr, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        s->len
    );

    // add ptr to vt_plist_t
    p->ptr2[at] = (void*)ptr;

    return vt_ve_operation_success;
}

void *vt_plist_get(const vt_plist_t *const p, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        s->len
    );

    return p->ptr2[at];
}

enum VitaError vt_plist_push(vt_plist_t *const p, const void *ptr) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    
    // check if new memory needs to be allocated
    if(!vt_plist_has_space(p) && vt_plist_reserve(p, p->capacity * CONTAINER_GROWTH_RATE) != vt_ve_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // add ptr to vt_plist_t
    p->ptr2[p->len++] = (void*)ptr;

    return vt_ve_operation_success;
}

enum VitaError vt_plist_pop(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // pop the last element
    if(p->len > 0) {
        p->len--;
    }

    return vt_ve_operation_success;
}

void *vt_plist_pop_get(vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // pop the last element
    if(p->len > 0) {
        return ((char**)p->ptr2)[--p->len];
    }

    return p->ptr2;
}

enum VitaError vt_plist_remove(vt_plist_t *const p, const size_t at, const enum RemoveStrategy rs) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(rs < vt_rs_count, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < p->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        s->len
    );

    // check remove strategy
    if(rs == rs_stable) {
        memmove((char**)(p->ptr2) + at * p->elsize, (char**)(p->ptr2) + (at + 1) * p->elsize, (p->len - at) * p->elsize);
    } else {
        vt_gswap((char**)(p->ptr2) + at * p->elsize, (char**)(p->ptr2) + (p->len - 1) * p->elsize, p->elsize);
    }

    // update length
    p->len--;

    return vt_ve_operation_success;
}

void vt_plist_apply(const vt_plist_t *const p, void (*func)(void*, size_t)) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(func != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // iterate
    const size_t len = vt_plist_len(p);
    for(size_t i = 0; i < len; i++) {
        func(((char*)p->ptr2)[i], i);
    }
}
