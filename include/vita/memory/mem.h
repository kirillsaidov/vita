#ifndef VITA_MEM_H
#define VITA_MEM_H

/** VITA_MEM MODULE (memory management)
    - mem_malloc
    - mem_calloc
    - mem_realloc
    - mem_free
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../core/core.h"
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
        1. if ptr is NULL, safely exits
        2. sets ptr to NULL after deallocation
*/
extern void mem_free(void* ptr);

#endif // VITA_MEM_H
