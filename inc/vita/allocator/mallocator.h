#ifndef VITA_ALLOCATOR_MALLOCATOR_H
#define VITA_ALLOCATOR_MALLOCATOR_H

/** MALLOCATOR MODULE 
 * This allocator utilizes plain calloc/free functions for memory management. 
 * However, it also keeps the allocation statistics for further examination.

 * Macros
    - VT_ALLOCATOR_ALLOC
    - VT_ALLOCATOR_REALLOC
    - VT_ALLOCATOR_FREE

 * Functions
    - vt_mallocator_create
    - vt_mallocator_destroy
    - vt_mallocator_alloc
    - vt_mallocator_realloc
    - vt_mallocator_free
    - vt_mallocator_print_stats
*/

#include "../core/core.h"
#include "../util/debug.h"

// macros
#define VT_ALLOCATOR_ALLOC(alloctr, bytes) alloctr->alloc(alloctr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_ALLOCATOR_REALLOC(alloctr, ptr, bytes) alloctr->realloc(alloctr, ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_ALLOCATOR_FREE(alloctr, ptr) alloctr->free(alloctr, ptr, __SOURCE_FILENAME__, __func__, __LINE__)

// allocator statistics
struct VitaAllocatorStats {
    size_t count_allocs;                    // number of allocations made
    size_t count_reallocs;                  // number of reallocations made
    size_t count_frees;                     // number of frees made
    size_t count_bytes_allocated;           // number of bytes currently allocated
    size_t count_bytes_freed;               // number of bytes freed
};

// allocator cache
struct VitaAllocatedObject {
    void *ptr;
    size_t bytes;
};

// base allocator type for all allocator-like primitives
struct VitaBaseAllocatorType {
    // statistics
    struct VitaAllocatorStats stats;

    // obj cache list
    struct VitaAllocatedObject *obj_list;
    size_t obj_list_len;
    size_t obj_list_capacity;

    // functions
    void *(*alloc)(struct VitaBaseAllocatorType *const, const size_t, const char *const, const char *const, const size_t);           // custom allocation function
    void *(*realloc)(struct VitaBaseAllocatorType *const, void*, const size_t, const char *const, const char *const, const size_t);  // custom reallocation function
    void  (*free)(struct VitaBaseAllocatorType *const, void*, const char *const, const char *const, const size_t);                   // custom free function
};

// mallocator
typedef struct VitaBaseAllocatorType vt_mallocator_t;

/** Creates a mallocator object
    @returns vt_mallocator_t*
*/
extern vt_mallocator_t *vt_mallocator_create(void);

/** Frees all allocated memory and destroys the mallocator object
    @param alloctr vt_mallocator_t object
*/
extern void vt_mallocator_destroy(vt_mallocator_t *alloctr);

/** Allocates memory using the mallocator object
    @param alloctr vt_mallocator_t object
    @param bytes number of bytes to allocate
    @param file __SOURCE_FILENAME__
    @param func __func__
    @param line __LINE__

    @returns pointer to allocated block of memory
*/
extern void *vt_mallocator_alloc(vt_mallocator_t *const alloctr, const size_t bytes, const char *const file, const char *const func, const size_t line);

/** Reallocates memory using the mallocator object
    @param alloctr vt_mallocator_t object
    @param ptr pointer to the previously allocated block of memory
    @param bytes number of bytes to allocate
    @param file __SOURCE_FILENAME__
    @param func __func__
    @param line __LINE__

    @returns pointer to reallocated block of memory
*/
extern void *vt_mallocator_realloc(vt_mallocator_t *const alloctr, void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line);

/** Frees memory allocated with the mallocator object
    @param alloctr vt_mallocator_t object
    @param ptr pointer to the previously allocated block of memory
    @param file __SOURCE_FILENAME__
    @param func __func__
    @param line __LINE__
*/
extern void vt_mallocator_free(vt_mallocator_t *const alloctr, void *ptr, const char *const file, const char *const func, const size_t line);

/** Prints stats report on memory allocations
    @param stats VitaAllocatorStats struct
*/
extern void vt_mallocator_print_stats(const struct VitaAllocatorStats stats);

#endif // VITA_ALLOCATOR_MALLOCATOR_H
