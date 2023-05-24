#ifndef VITA_MALLOCATOR_H
#define VITA_MALLOCATOR_H

/** VITA_MALLOCATOR MODULE 
    - vt_mallocator_alloc
    - vt_mallocator_relloc
    - vt_mallocator_free
    - vt_mallocator_print_stats
*/

#include "../core/core.h"

// allocator statistics
struct VitaAllocatorStats {
    size_t count_allocs;                    // number of allocations made
    size_t count_reallocs;                  // number of reallocations made
    size_t count_frees;                     // number of frees made
    size_t count_bytes_allocated;           // number of bytes currently allocated
    size_t count_bytes_totally_allocated;   // number of bytes totally allocated
};

/// Same as vt_calloc
extern void *vt_mallocator_alloc(const size_t bytes);

/// Same as vt_realloc
extern void *vt_mallocator_realloc(void *ptr, const size_t bytes);

/// Same as vt_free
extern void vt_mallocator_free(void *ptr);

/// prints mallocator stats
extern void vt_mallocator_print_stats(void);

// mallocator instance
static struct {
    struct VitaAllocatorStats stats;
    
    // functions
    void *(*alloc)(const size_t);           // custom allocation function
    void *(*realloc)(void*, const size_t);  // custom reallocation function
    void  (*free)(void*);                   // custom free function
} vt_mallocator = {
    .alloc = vt_mallocator_alloc,
    .realloc = vt_mallocator_realloc,
    .free = vt_mallocator_free,
};

#endif // VITA_MALLOCATOR_H
