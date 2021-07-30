#ifndef VITA_MEM_H
#define VITA_MEM_H

/* VITA_MEM MODULE (memory management)
    - mem_malloc
    - mem_calloc
    - mem_realloc
    - mem_free
 
    - mem_malloc_2d
    - mem_calloc_2d
    - mem_realloc_2d
    - mem_free_2d
*/

// std includes
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// vita includes
#include "../logger/logger.h"

/** mem_malloc ==> same as malloc, but tracks total number of allocations
    params:
        const size_t n
        const size_t size
    returns:
        void* (ptr to allocated memory) - on success
        NULL - on failure
*/
extern void* mem_malloc(const size_t n, const size_t size);

/** mem_calloc ==> same as calloc, but tracks total number of allocations
    params:
        const size_t n
        const size_t size
    returns:
        void* (ptr to allocated memory) - on success
        NULL - on failure
*/
extern void* mem_calloc(const size_t n, const size_t size);

/** mem_realloc ==> same as realloc, but tracks total number of allocations
    params:
        void** ptr
        const size_t n
        const size_t size
    returns:
        bool
    notes:
        true - on success
        false - on failure
*/
extern bool mem_realloc(void** ptr, const size_t n, const size_t size);

/** mem_free ==> same as free, but tracks total number of allocations and frees
    params:
        void* ptr
    notes:
        if ptr is NULL, safely exits
*/
extern void mem_free(void* ptr);







/** mem_malloc_2d ==> same as malloc, but tracks total number of allocations, allocates 2d array
    params:
        const size_t rows
        const size_t cols
        const size_t size
    returns:
        void** (ptr to allocated memory) - on success
        NULL - on failure
*/
extern void** mem_malloc_2d(const size_t rows, const size_t cols, const size_t size);

/** mem_calloc_2d ==> same as calloc, but tracks total number of allocations, allocates 2d array
    params:
        const size_t rows
        const size_t cols
        const size_t size
    returns:
        void** (ptr to allocated memory) - on success
        NULL - on failure
*/
extern void** mem_calloc_2d(const size_t rows, const size_t cols, const size_t size);

/** mem_realloc_2d ==> same as realloc, but tracks total number of allocations, reallocates 2d array
    params:
        void*** ptr
        const size_t rows
        const size_t cols
        const size_t size
    returns:
        bool
    notes:
        true - on success
        false - on failure
*/
extern bool mem_realloc_2d(void*** ptr, const size_t rows, const size_t cols, const size_t size);

/** mem_free_2d ==> same as free, but tracks total number of allocations and frees, frees 2d array
    params:
        void** ptr
    notes:
        if ptr is NULL, safely exits
*/
extern void mem_free_2d(void** ptr);

#endif // VITA_MEM_H
