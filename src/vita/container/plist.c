#include "vita/container/plist.h"


plist_t *plist_new(void) {
    plist_t *p = malloc(sizeof(plist_t));
    if(is_null(p)) {
        return NULL; 
    }

    // default-init
    *p = (plist_t) {};

    return p;
}

enum ContainerError plist_ctor(plist_t *p, const size_t n) {
    // check if p was allocated
    if(is_null(p)) {
        return ce_container_is_null;
    }

    // plist_t init
    *p = (plist_t) {
        .ptr2 = calloc(n, sizeof(void*)),
        .len = 0,
        .capacity = n,
        .elsize = sizeof(void*),
    };

    // checking if p->ptr2 was allocated
    if(is_null(p->ptr2)) {
        return ce_error_allocation;
    }

    return ce_operation_success;
}

void plist_dtor(plist_t *p) {
    // if NULL, exit
    if(is_null(p)) {
        return;
    }

    // free plist_t contents
    free(p->ptr2);

    // default-init
    *p = (plist_t) {};
}


void plist_free(plist_t *p) {
    // if NULL, exit
    if(is_null(p)) {
        return;
    }

    // free the plist_t itself
    free(p);

    // reset to NULL
    p = NULL;
}







plist_t *plist_create(const size_t n) {
    plist_t *p = plist_new();
    if(is_null(p)) {
        return NULL;
    }

    if(plist_ctor(p, n) != ce_operation_success) {
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
    return p->len;
}

size_t plist_capacity(const plist_t *const p) {
    return p->capacity;
}

size_t plist_has_space(const plist_t *const p) {
    return (p->capacity - p->len);
}







enum ContainerError plist_reserve(plist_t *const p, const size_t n) {
    if(is_null(p) || !n) {
        return ce_container_is_null;
    }

    // reserve memory for additional n elements
    void **newptr = realloc(p->ptr2, (p->capacity + n) * p->elsize);
    if(is_null(newptr)) {
        vita_warn("memory allocation failed!", __FUNCTION__);
        return ce_error_allocation;
    }

    // update values
    p->ptr2 = newptr;
    p->capacity += n;

    return ce_operation_success;
}

enum ContainerError plist_shrink(plist_t *const p) {
    if(is_null(p)) {
        return ce_container_is_null;
    }

    // if length and capacity are the same, exit the function
    if(p->len == p->capacity) {
        return ce_operation_success;
    }

    void **newptr = realloc(p->ptr2, p->len * p->elsize);
    if(is_null(newptr)) {
        vita_warn("memory allocation failed!", __FUNCTION__);
		return ce_error_allocation;
    }

    // update values
    p->ptr2 = newptr;
    p->capacity = p->len;

    return ce_operation_success;
}

enum ContainerError plist_set(plist_t *const p, const void *ptr, const size_t at) {
    if(is_null(p) || is_null(ptr)) {
        return ce_container_is_null;
    }

    if(!(at < p->len)) {
        return ce_error_out_of_bounds_access;
    }

    // add ptr to plist_t
    p->ptr2[at] = (void*)ptr;

    return ce_operation_success;
}

void *plist_get(const plist_t *const p, const size_t at) {
    if(is_null(p) || !(at < p->len)) {
        return NULL;
    }

    return p->ptr2[at];
}

enum ContainerError plist_push(plist_t *const p, const void *ptr) {
    if(is_null(p) || is_null(ptr)) {
        return ce_container_is_null;
    }

    // check if new memory needs to be allocated
	if(!plist_has_space(p) && plist_reserve(p, (size_t)(p->capacity * CONTAINER_GROWTH_RATE + 1)) != ce_operation_success) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return ce_error_allocation;
	}

    // add ptr to plist_t
    p->ptr2[p->len++] = (void*)ptr;

    return ce_operation_success;
}

enum ContainerError plist_pop(plist_t *const p) {
    if(is_null(p)) {
        return ce_container_is_null;
    }

    // pop the last element
    if(p->len > 0) {
        p->len--;
    }

    return ce_operation_success;
}

void *plist_pop_get(plist_t *const p) {
    if(is_null(p)) {
        return NULL;
    }

    // pop the last element
    return p->ptr2[--p->len];
}

enum ContainerError plist_remove(plist_t *const p, const size_t at, const enum RemoveStrategy rs) {
    if(is_null(p)) {
        return ce_container_is_null;
    }

    if(!(at < p->len)) {
        return ce_error_out_of_bounds_access;
    }

    // check remove strategy
    if(rs == rs_stable) {
        memmove(p->ptr2 + at * p->elsize, p->ptr2 + (at + 1) * p->elsize, (p->len - at) * p->elsize);
    } else {
        gswap(p->ptr2 + at * p->elsize, p->ptr2 + (p->len - 1) * p->elsize, p->elsize);
    }

    // update length
    p->len--;

    return ce_operation_success;
}

void plist_foreach(const plist_t *const p, void (*func)(void*, size_t)) {
    if(is_null(p) || is_null(func)) {
        return;
    }

    for(size_t i = 0; i < plist_len(p); i++) {
        func(p->ptr2[i], i);
    }
}















