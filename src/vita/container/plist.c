#include "vita/container/plist.h"

plist_t *plist_new(void) {
    plist_t *p = DEBUG_CALLOC(sizeof(plist_t));
    if(p == NULL) {
        return NULL;
    }

    return p;
}

enum VitaError plist_ctor(plist_t *p, const size_t n) {
    // check if p was allocated
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not allocated!");
        return ve_error_is_null;
    }

    // plist_t init
    *p = (plist_t) {
        .ptr2 = DEBUG_CALLOC(n * sizeof(void*)),
        .len = 0,
        .capacity = n,
        .elsize = sizeof(void*),
    };

    // checking if p->ptr2 was allocated
    if(p->ptr2 == NULL) {
        DEBUG_ASSERT(p->ptr2 != NULL, "Unable to construct plist_t instance!");
        return ve_error_allocation;
    }

    return ve_operation_success;
}

void plist_dtor(plist_t *p) {
    // if NULL, exit
    if(p == NULL) {
        return;
    }

    // DEBUG_FREE plist_t contents
    DEBUG_FREE(p->ptr2);

    // default-init
    *p = (plist_t) {0};
}

void plist_free(plist_t *p) {
    // if NULL, exit
    if(p == NULL) {
        return;
    }

    // DEBUG_FREE the plist_t itself
    DEBUG_FREE(p);

    // reset to NULL
    p = NULL;
}

plist_t *plist_create(const size_t n) {
    plist_t *p = plist_new();
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "Unable to allocate plist_t instance!");
        return NULL;
    }

    if(plist_ctor(p, n) != ve_operation_success) {
        plist_free(p);
        return NULL;
    }

    return p;
}

void plist_destroy(plist_t *p) {
    plist_dtor(p);
    plist_free(p);
}

size_t plist_len(const plist_t *const p) {
    DEBUG_ASSERT(p != NULL, "plist_t was not initialized!");
    return (p == NULL) ? 0 : p->len;
}

size_t plist_capacity(const plist_t *const p) {
    DEBUG_ASSERT(p != NULL, "plist_t was not initialized!");
    return (p == NULL) ? 0 : p->capacity;
}

size_t plist_has_space(const plist_t *const p) {
    DEBUG_ASSERT(p != NULL, "plist_t was not initialized!");
    return (p == NULL) ? 0 : (p->capacity - p->len);
}

enum VitaError plist_reserve(plist_t *const p, const size_t n) {
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        return ve_error_is_null;
    }

    // if n = 0, do nothing
    if(!n) {
        return ve_operation_success;
    }

    // reserve memory for additional n elements
    void **newptr = DEBUG_REALLOC(p->ptr2, (p->capacity + n) * p->elsize);
    if(newptr == NULL) {
        return ve_error_allocation;
    }

    // update values
    p->ptr2 = newptr;
    p->capacity += n;

    return ve_operation_success;
}

enum VitaError plist_shrink(plist_t *const p) {
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        return ve_error_is_null;
    }

    // if length and capacity are the same, exit the function
    if(p->len == p->capacity) {
        return ve_operation_success;
    }

    void **newptr = DEBUG_REALLOC(p->ptr2, p->len * p->elsize);
    if(newptr == NULL) {
        DEBUG_ASSERT(newptr != NULL, "Failed to allocate memory!");
        return ve_error_allocation;
    }

    // update values
    p->ptr2 = newptr;
    p->capacity = p->len;

    return ve_operation_success;
}

enum VitaError plist_clear(plist_t *const p) {
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        return ve_error_is_null;
    }

    // update length
    p->len = 0;

    return ve_operation_success;
}

enum VitaError plist_set(plist_t *const p, const void *ptr, const size_t at) {
    if(p == NULL || ptr == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        DEBUG_ASSERT(ptr != NULL, "ptr value supplied is NULL!");
        return ve_error_is_null;
    }

    if(!(at < p->len)) {
        DEBUG_ASSERT(at < p->len, "Out of bounds access at %zu, but length is %zu!", at, p->len);
        return ve_error_out_of_bounds_access;
    }

    // add ptr to plist_t
    p->ptr2[at] = (void*)ptr;

    return ve_operation_success;
}

void *plist_get(const plist_t *const p, const size_t at) {
    if(p == NULL || !(at < p->len)) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        DEBUG_ASSERT(at < p->len, "Out of bounds access at %zu, but length is %zu!", at, p->len);
        return NULL;
    }

    return p->ptr2[at];
}

enum VitaError plist_push(plist_t *const p, const void *ptr) {
    if(p == NULL || ptr == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        DEBUG_ASSERT(ptr != NULL, "ptr value supplied is NULL!");
        return ve_error_is_null;
    }

    // check if new memory needs to be allocated
    if(!plist_has_space(p) && plist_reserve(p, p->capacity * CONTAINER_GROWTH_RATE) != ve_operation_success) {
        DEBUG_ASSERT(0, "Failed to reserve memory before pushing a new value!");
        return ve_error_allocation;
    }

    // add ptr to plist_t
    p->ptr2[p->len++] = (void*)ptr;

    return ve_operation_success;
}

enum VitaError plist_pop(plist_t *const p) {
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        return ve_error_is_null;
    }

    // pop the last element
    if(p->len > 0) {
        p->len--;
    }

    return ve_operation_success;
}

void *plist_pop_get(plist_t *const p) {
    if(p == NULL || !p->len) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        return NULL;
    }

    // pop the last element
    return p->ptr2[--p->len];
}

enum VitaError plist_remove(plist_t *const p, const size_t at, const enum RemoveStrategy rs) {
    if(p == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        return ve_error_is_null;
    }

    if(!(at < p->len)) {
        DEBUG_ASSERT(at < p->len, "Out of bounds access at %zu, but length is %zu!", at, p->len);
        return ve_error_out_of_bounds_access;
    }

    // check remove strategy
    if(rs == rs_stable) {
        memmove(p->ptr2 + at * p->elsize, p->ptr2 + (at + 1) * p->elsize, (p->len - at) * p->elsize);
    } else {
        gswap(p->ptr2 + at * p->elsize, p->ptr2 + (p->len - 1) * p->elsize, p->elsize);
    }

    // update length
    p->len--;

    return ve_operation_success;
}

void plist_apply(const plist_t *const p, void (*func)(void*, size_t)) {
    if(p == NULL || func == NULL) {
        DEBUG_ASSERT(p != NULL, "plist_t instance was not initialized!");
        DEBUG_ASSERT(func != NULL, "func supplied is NULL!");
        return;
    }

    const size_t len = plist_len(p);
    for(size_t i = 0; i < len; i++) {
        func(p->ptr2[i], i);
    }
}
