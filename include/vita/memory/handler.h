#ifndef VITA_MEMORY_HANDLER_H
#define VITA_MEMORY_HANDLER_H

/* VITA_MEMORY_HANDLER MODULE
    - memory_handler_create
    - memory_handler_internal_create
    - memory_handler_internal
    - memory_handler_add
    - memory_handler_add_n
    - memory_handler_remove
    - memory_handler_update
    - memory_handler_malloc
    - memory_handler_malloc_2d
    - memory_handler_calloc
    - memory_handler_calloc_2d
    - memory_handler_realloc
    - memory_handler_realloc_2d
    - memory_handler_free
    - memory_handler_free_2d
    - memory_handler_destroy
*/

// std includes
#include <stdio.h>
#include <stdbool.h>

// vita includes
#include "../logger.h"
#include "../memory/memory.h"
#include "../algorithm/search.h"

/* MemoryHandler ==> structure for handling memory
    notes:
        it is a custom container of pointers, may be used to free all added memory at once
 */
typedef struct MemoryHandler MemoryHandler;

/* memory_handler_create
    returns:
         MemoryHandler*
*/
extern MemoryHandler* memory_handler_create(void);

/* memory_handler_internal_create ==> creates an internal memory handler
*/
extern void memory_handler_internal_create(void);

/* memory_handler_internal ==> returns an internal memory handler
    returns:
         MemoryHandler*
*/
extern MemoryHandler* memory_handler_internal(void);

/* memory_handler_add ==> adds allocated memory pointer to memory handler
    params:
        MemoryHandler* handler
        const void* ptr
*/
extern void memory_handler_add(MemoryHandler* handler, const void* ptr);

/* memory_handler_add_n ==> adds allocated memory pointers to memory handler
    params:
        MemoryHandler* handler
        const size_t n
        const void* ptrs[]
    returns:
        size_t id
*/
extern void memory_handler_add_n(MemoryHandler* handler, const size_t n, const void* ptrs[]);

/* memory_handler_remove ==> removes the ptr from memory handler by setting it to NULL
    params:
        MemoryHandler* handler
        const void* ptr
    notes:
        IMPORTANT: does not free the allocated memory
*/
extern void memory_handler_remove(MemoryHandler* handler, const void* ptr);

/* memory_handler_update ==> updates a ptr from memory handler by new ptr
    params:
        MemoryHandler* handler
        const void* oldPtr
        const void* newPtr
    notes:
        IMPORTANT: does not free the allocated memory
*/
extern void memory_handler_update(MemoryHandler* handler, const void* oldPtr, const void* newPtr);

/* memory_handler_malloc ==> allocates and adds the ptr to memory handler automatically
    params:
        MemoryHandler* handler
        const size_t n
        const size_t size
    retuns:
        void*
*/
extern void* memory_handler_malloc(MemoryHandler* handler, const size_t n, const size_t size);

/* memory_handler_malloc_2d ==> creates 2d array, updates the memory handler automatically
    params:
        MemoryHandler* handler
        const size_t rows
        const size_t cols
        const size_t size
    retuns:
        void**
*/
extern void** memory_handler_malloc_2d(MemoryHandler* handler, const size_t rows, const size_t cols, const size_t size);

/* memory_handler_calloc ==> allocates and adds the ptr to memory handler automatically
    params:
        MemoryHandler* handler
        const size_t n
        const size_t size
    retuns:
        void*
*/
extern void* memory_handler_calloc(MemoryHandler* handler, const size_t n, const size_t size);

/* memory_handler_calloc_2d ==> creates 2d array, updates the memory handler automatically
    params:
        MemoryHandler* handler
        const size_t rows
        const size_t cols
        const size_t size
    retuns:
        void**
*/
extern void** memory_handler_calloc_2d(MemoryHandler* handler, const size_t rows, const size_t cols, const size_t size);

/* memory_handler_realloc ==> same as realloc, but works through memory handler by updating the ptr automatically
    params:
        MemoryHandler* handler
        void** ptr
        const size_t n
        const size_t size
    returns:
        bool
    notes:
         true - on success
        false - on failure
*/
extern bool memory_handler_realloc(MemoryHandler* handler, void** ptr, const size_t n, const size_t size);

/* memory_handler_realloc_2d ==> resizes 2d array; same as realloc, but works through memory handler by updating the ptr automatically
    params:
        MemoryHandler* handler
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
extern bool memory_handler_realloc_2d(MemoryHandler* handler, void*** ptr, const size_t rows, const size_t cols, const size_t size);

/* memory_handler_free ==> same as memory_free, but updates memory handler automatically
    params:
        MemoryHandler* handler
        void* ptr
*/
extern void memory_handler_free(MemoryHandler* handler, void* ptr);

/* memory_handler_free_2d ==> frees 2d array; same as memory_free, but updates memory handler automatically
    params:
        MemoryHandler* handler
        void** ptr
*/
extern void memory_handler_free_2d(MemoryHandler* handler, void** ptr);

/* memory_handler_destroy ==> frees all pointers that point to allocated memory including the handler itself
    params:
        MemoryHandler* handler
    notes:
        if a pointer is NULL, safely skips avoiding double frees,
        if a memory handler is NULL, safely exits
*/
extern void memory_handler_destroy(MemoryHandler* handler);

#endif // VITA_MEMORY_HANDLER_H









