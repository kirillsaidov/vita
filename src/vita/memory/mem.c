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







void** mem_malloc_2d(const size_t rows, const size_t cols, const size_t size) {
    // allocate rows and do error checking
    void** ptr = mem_malloc(rows, sizeof(void*));
    if(is_null(ptr)) {
        logger_error(str("unable to allocate memory!"), str("mem_malloc_2d"));
        return NULL;
    }
    
    // allocate cols and do error checking
    void* temp = mem_malloc(rows*cols, size);
    if(is_null(temp)) {
        logger_error(str("unable to allocate memory!"), str("mem_malloc_2d"));
        
        // free rows
        mem_free(ptr);
        
        return NULL;
    }
    
    // relocate columns
    for(size_t i = 0; i < rows; i++, temp += cols*size) {
        ptr[i] = temp;
    }
    
    return ptr;
}

void** mem_calloc_2d(const size_t rows, const size_t cols, const size_t size) {
    // allocate rows and do error checking
    void** ptr = mem_calloc(rows, sizeof(void*));
    if(is_null(ptr)) {
        logger_error(str("unable to allocate memory!"), str("mem_calloc_2d"));
        return NULL;
    }
    
    // allocate cols and do error checking
    void* temp = mem_calloc(rows*cols, size);
    if(is_null(temp)) {
        logger_error(str("unable to allocate memory!"), str("mem_calloc_2d"));
        
        // free rows
        mem_free(ptr);
        
        return NULL;
    }
    
    // relocate columns
    for(size_t i = 0; i < rows; i++, temp += cols*size) {
        ptr[i] = temp;
    }
    
    return ptr;
}

bool mem_realloc_2d(void*** ptr, const size_t rows, const size_t cols, const size_t size) {
    // reallocate rows
    if(!mem_realloc(*ptr, rows, sizeof(void*))) {
        logger_error(str("unable to reallocate memory! failed to reallocate rows..."), str("mem_realloc_2d"));
        return false;
    }
    
    // reallocate columns
    if(!mem_realloc(&(**ptr), cols*rows, size)) {
        logger_error(str("unable to reallocate memory! failed to reallocate columns..."), str("mem_realloc_2d"));
        return false;
    }
    
    // re-organize columns
    void* temp = **ptr;
    for(size_t i = 0; i < rows; i++, temp += cols*size) {
        (*ptr)[i] = temp;
    }
    
    return true;
}

void mem_free_2d(void** ptr) {
    // check if ptr is NULL to avoid double frees
    if(is_null(ptr)) {
        logger_warn(str("ptr is NULL; skipping..."), str("mem_free_2d"));
        return;
    }
    
    // free columns
    mem_free(*ptr);
    
    // free rows
    mem_free(ptr);
}






