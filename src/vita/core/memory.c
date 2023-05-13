#include "vita/core/memory.h"

void *vt_memory_malloc(const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr = malloc(bytes);
    if(ptr == NULL) {
        fprintf(stderr, "%s %s:%s:%d: %s\n", "MEMORY ALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *vt_memory_calloc(const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr = calloc(1, bytes);
    if(ptr == NULL) {
        fprintf(stderr, "%s %s:%s:%d: %s\n", "MEMORY ALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *vt_memory_realloc(void *ptr, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    assert(bytes > 0);

    // allocate and error checking
    void *ptr_new = realloc(ptr, bytes);
    if(ptr_new == NULL) {
        fprintf(stderr, "%s %s:%s:%d: %s\n", "MEMORY REALLOCATION FAILURE", file, func, line, "Aborting...");
        exit(EXIT_FAILURE);
    }

    return ptr_new;
}

void vt_memory_free(void *ptr) {
    if(ptr == NULL) {
        return;
    }

    free(ptr);
}


