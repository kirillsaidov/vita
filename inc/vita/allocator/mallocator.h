#ifndef VITA_MALLOCATOR_H
#define VITA_MALLOCATOR_H

/** VITA_MALLOCATOR MODULE 
    - 
*/

#include "../util/debug.h"

// macros
#define VT_MALLOCATOR_ALLOC(alloctr, bytes) vt_mallocator_alloc(alloctr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_MALLOCATOR_REALLOC(alloctr, ptr, bytes) vt_mallocator_realloc(alloctr, ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_MALLOCATOR_FREE(alloctr, bytes) vt_mallocator_free(alloctr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)

// allocator statistics
struct VitaAllocatorStats {
    size_t count_allocs;                    // number of allocations made
    size_t count_reallocs;                  // number of reallocations made
    size_t count_frees;                     // number of frees made
    size_t count_bytes_allocated;           // number of bytes currently allocated
    size_t count_bytes_freed;               // number of bytes freed
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
    struct VitaAllocatorObject *obj_list;
    size_t obj_list_len;
    size_t obj_list_capacity;

    // functions
    void *(*alloc)(struct VitaBaseAllocatorType *const alloctr, const size_t, const char *const, const char *const, const size_t);           // custom allocation function
    void *(*realloc)(struct VitaBaseAllocatorType *const alloctr, void*, const size_t, const char *const, const char *const, const size_t);  // custom reallocation function
    void  (*free)(struct VitaBaseAllocatorType *const alloctr, void*, const char *const, const char *const, const size_t);                   // custom free function
};

// mallocator
typedef struct VitaBaseAllocatorType vt_mallocator_t;

extern vt_mallocator_t *vt_mallocator_create(void);
extern void vt_mallocator_destroy(vt_mallocator_t *alloctr);

extern void *vt_mallocator_alloc(vt_mallocator_t *const alloctr, const size_t bytes, const char *const file, const char *const func, const size_t line);
extern void *vt_mallocator_realloc(vt_mallocator_t *const alloctr, void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line);
extern void vt_mallocator_free(vt_mallocator_t *const alloctr, void *ptr, const char *const file, const char *const func, const size_t line);

extern void vt_mallocator_stats_print(const struct VitaAllocatorStats stats);

#endif // VITA_MALLOCATOR_H
