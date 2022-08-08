#ifndef VITA_DEBUG_H
#define VITA_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - DEBUG_ASSERT

 * Functions
    - debug_assert
*/

#include "../core/core.h"
#include "../algorithm/search.h"

// Memory handler
typedef struct DebugMemoryHandler {
    // data
    struct BaseArrayType data;  // data container
    bool is_init;               // status

    // statistics
    size_t n_allocs;            // number of allocations
    size_t n_reallocs;          // number of reallocations
    size_t n_frees;             // number of frees
    size_t bytes_totally_alloced;       // memory size totally allocated throughout the program in bytes
    size_t bytes_freed;         // memory size freed throughout the program in bytes
} debug_mh_t;

// private memory handler that used by default
debug_mh_t *gi_mh;

// macros
#ifndef NDEBUG
    #define DEBUG_ASSERT(expr, ...) debug_assert(expr, __FILE__, __LINE__, __VA_ARGS__)
    #define DEBUG_ASSERT2(expr, file, line, ...) debug_assert(expr, file, line, __VA_ARGS__)
    #define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__);

    // memory management
    #define DEBUG_MALLOC(bytes) debug_mh_malloc(gi_mh, bytes, __FILE__, __LINE__)
    #define DEBUG_CALLOC(bytes) debug_mh_calloc(gi_mh, bytes, __FILE__, __LINE__)
    #define DEBUG_REALLOC(ptr, bytes) debug_mh_realloc(gi_mh, ptr, bytes, __FILE__, __LINE__)
    #define DEBUG_FREE(ptr) debug_mh_free(gi_mh, ptr, __FILE__, __LINE__)

    // stats
    #define DEBUG_NALLOCS debug_mh_get_nallocs(gi_mh)
    #define DEBUG_NREALLOCS debug_mh_get_nreallocs(gi_mh)
    #define DEBUG_NFREES debug_mh_get_nfrees(gi_mh)
    #define DEBUG_BYTES_TOTALLY_ALOCATED debug_mh_get_bytes_totally_alloced(gi_mh)
    #define DEBUG_BYTES_CURRENTLY_ALOCATED debug_mh_get_bytes_currently_alloced(gi_mh)
    #define DEBUG_BYTES_FREED debug_mh_get_bytes_freed(gi_mh)
#else
    #define DEBUG_ASSERT(expr, ...)
    #define DEBUG_ASSERT2(expr, file, line, ...)
    #define DEBUG_PRINT(fmt, ...)

    // memory management
    #define DEBUG_MALLOC(bytes) malloc(bytes)
    #define DEBUG_CALLOC(bytes) calloc(1, bytes)
    #define DEBUG_REALLOC(ptr, bytes) realloc(ptr, bytes)
    #define DEBUG_FREE(ptr) free(ptr)
#endif

/**
Asserts an expression and exits upon its evaluation to false

Params:
    expr = expression to test
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)
    fmt = formatting
    ... = additional arguments
*/
extern void debug_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...);

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

extern void *debug_mh_malloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line);
extern void *debug_mh_calloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line);
extern void *debug_mh_realloc(debug_mh_t *const mh, void *ptr, const size_t bytes, const char *const file, const int32_t line);
extern void debug_mh_free(debug_mh_t *const mh, void *ptr, const char *const file, const int32_t line);

extern void debug_mh_add(debug_mh_t *const mh, const void *const ptr, const size_t bytes);
extern int64_t debug_mh_remove(debug_mh_t *const mh, const void *const ptr);

extern void debug_mh_cleanup(void);

/* ---------------------- STATS FUNCTIONS ---------------------- */

extern size_t debug_mh_get_nallocs(const debug_mh_t *const mh);
extern size_t debug_mh_get_nreallocs(const debug_mh_t *const mh);
extern size_t debug_mh_get_nfrees(const debug_mh_t *const mh);
extern size_t debug_mh_get_bytes_totally_alloced(const debug_mh_t *const mh);
extern size_t debug_mh_get_bytes_currently_alloced(const debug_mh_t *const mh);
extern size_t debug_mh_get_bytes_freed(const debug_mh_t *const mh);

extern char *debug_mh_get_stats_str(const debug_mh_t *const mh, char *buffer);
extern void debug_mh_print_stats(const debug_mh_t *const mh);

/* ------------------- mh HANDLER FUNCTIONS -------------------- */

extern debug_mh_t *debug_mh_handler_create(void);
extern void debug_mh_handler_destroy(debug_mh_t *mh);
extern bool debug_mh_handler_is_init(const debug_mh_t *const mh);
extern bool debug_mh_handler_is_init_then(debug_mh_t *mh);

#endif // VITA_DEBUG_H
