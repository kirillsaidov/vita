#include "vita/memory/mem.h"

void* mem_malloc(const size_t n, const size_t size) {
    // memory pointer
    void* ptr = malloc(n*size);
    
    // error checking
    if(is_null(ptr)) {
        logger_error(str("unable to allocate memory!"), str("mem_malloc"));
    }
    
    return ptr;
}

void* mem_calloc(const size_t n, const size_t size) {
    // memory pointer
    void* ptr = calloc(n, size);
    
    // error checking
    if(is_null(ptr)) {
        logger_error(str("unable to allocate memory!"), str("mem_calloc"));
    }
    
    return ptr;
}

bool mem_realloc(void** ptr, const size_t n, const size_t size) {
    // memory pointer
    void* ptrNew = realloc(*ptr, n*size);
    
    // error checking
    if(is_null(ptrNew)) {
        logger_error(str("unable to reallocate memory!"), str("mem_realloc"));
        return false;
    } else {
        *ptr = ptrNew;
    }
    
    return true;
}

void mem_free(void* ptr) {
    // check if ptr is NULL to avoid double frees
    if(is_null(ptr)) {
        logger_warn(str("ptr is NULL; skipping..."), str("mem_free"));
        return;
    }
    
    // free memory
    free(ptr);
    
    // reset to NULL
    ptr = NULL;
}









