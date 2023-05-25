#ifndef VITA_MALLOCATOR_H
#define VITA_MALLOCATOR_H

/** VITA_MALLOCATOR MODULE 
    - 
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

// allocator cache
struct VitaAllocatorObject {
    void *ptr;
    size_t bytes;
};

// base allocator type for all allocator-like primitives
struct VitaBaseAllocatorType {
    // statistics
    struct VitaAllocatorStats stats;

    // obj cache list
    size_t obj_list_len;
    size_t obj_list_capacity;
    struct VitaAllocatorObject *obj_list;

    // functions
    void *(*alloc)(struct VitaBaseAllocatorType *const alloctr, const size_t);           // custom allocation function
    void *(*realloc)(struct VitaBaseAllocatorType *const alloctr, void*, const size_t);  // custom reallocation function
    void  (*free)(struct VitaBaseAllocatorType *const alloctr, void*);                   // custom free function
};

// mallocator
typedef struct VitaBaseAllocatorType vt_mallocator_t;

extern vt_mallocator_t *vt_mallocator_create(void);
extern void vt_mallocator_destroy(vt_mallocator_t *alloctr);

extern void *vt_mallocator_alloc(vt_mallocator_t *const alloctr, const size_t bytes);
extern void *vt_mallocator_realloc(vt_mallocator_t *const alloctr, void *ptr, const size_t bytes);
extern void vt_mallocator_free(vt_mallocator_t *const alloctr, void *ptr);

// /// prints mallocator stats
// extern void vt_mallocator_print_stats(void);

#endif // VITA_MALLOCATOR_H
