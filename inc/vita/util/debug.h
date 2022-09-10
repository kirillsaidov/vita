#ifndef VITA_DEBUG_H
#define VITA_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - DEBUG_ASSERT
    - DEBUG_ASSERT2
    - DEBUG_PRINT
    - DEBUG_MALLOC
    - DEBUG_CALLOC
    - DEBUG_REALLOC
    - DEBUG_NALLOCS
    - DEBUG_NREALLOCS
    - DEBUG_NFREES
    - DEBUG_BYTES_TOTALLY_ALOCATED
    - DEBUG_BYTES_CURRENTLY_ALOCATED
    - DEBUG_BYTES_FREED

 * Functions
    - debug_assert
    - debug_printf
    - debug_mh_handler_new
    - debug_mh_handler_free
    - debug_mh_handler_default_init
    - debug_mh_handler_default_quit
    - debug_mh_handler_default_get_handler
    - debug_mh_malloc
    - debug_mh_calloc
    - debug_mh_realloc
    - debug_mh_free
    - debug_mh_add
    - debug_mh_remove
    - debug_mh_get_nallocs
    - debug_mh_get_nreallocs
    - debug_mh_get_nfrees
    - debug_mh_get_bytes_totally_alloced
    - debug_mh_get_bytes_currently_alloced
    - debug_mh_get_bytes_freed
    - debug_mh_get_stats_str
    - debug_mh_print_stats
*/

#include <math.h>
#include "../core/core.h"

// memory handler instance
typedef struct DebugMemoryHandler debug_mh_t;

#ifndef NDEBUG
    // error handling
    #define DEBUG_ASSERT(expr, ...) debug_assert(expr, STRINGOF(expr), __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define DEBUG_ASSERT2(expr, file, func, line, ...) debug_assert(expr, STRINGOF(expr), file, func, line, __VA_ARGS__)
    #define DEBUG_PRINT(fmt, ...) debug_printf(fmt, __VA_ARGS__)

    // memory management
    #define DEBUG_MALLOC(bytes) debug_mh_malloc(debug_mh_handler_default_get_handler(), bytes, __FILENAME__, __FUNCTION__, __LINE__)
    #define DEBUG_CALLOC(bytes) debug_mh_calloc(debug_mh_handler_default_get_handler(), bytes, __FILENAME__, __FUNCTION__, __LINE__)
    #define DEBUG_REALLOC(ptr, bytes) debug_mh_realloc(debug_mh_handler_default_get_handler(), ptr, bytes, __FILENAME__, __FUNCTION__, __LINE__)
    #define DEBUG_FREE(ptr) debug_mh_free(debug_mh_handler_default_get_handler(), ptr, __FILENAME__, __FUNCTION__, __LINE__)

    // stats
    #define DEBUG_NALLOCS debug_mh_get_nallocs(debug_mh_handler_default_get_handler())
    #define DEBUG_NREALLOCS debug_mh_get_nreallocs(debug_mh_handler_default_get_handler())
    #define DEBUG_NFREES debug_mh_get_nfrees(debug_mh_handler_default_get_handler())
    #define DEBUG_BYTES_TOTALLY_ALOCATED debug_mh_get_bytes_totally_alloced(debug_mh_handler_default_get_handler())
    #define DEBUG_BYTES_CURRENTLY_ALOCATED debug_mh_get_bytes_currently_alloced(debug_mh_handler_default_get_handler())
    #define DEBUG_BYTES_FREED debug_mh_get_bytes_freed(debug_mh_handler_default_get_handler())
#else
    // error handling
    #define DEBUG_ASSERT(expr, ...)
    #define DEBUG_ASSERT2(expr, file, line, ...)
    #define DEBUG_PRINT(fmt, ...)

    // memory management
    #define DEBUG_MALLOC(bytes) malloc(bytes)
    #define DEBUG_CALLOC(bytes) calloc(1, bytes)
    #define DEBUG_REALLOC(ptr, bytes) realloc(ptr, bytes)
    #define DEBUG_FREE(ptr) free(ptr)

    // stats
    #define DEBUG_NALLOCS(mh) debug_mh_get_nallocs(mh)
    #define DEBUG_NREALLOCS(mh) debug_mh_get_nreallocs(mh)
    #define DEBUG_NFREES(mh) debug_mh_get_nfrees(mh)
    #define DEBUG_BYTES_TOTALLY_ALOCATED(mh) debug_mh_get_bytes_totally_alloced(mh)
    #define DEBUG_BYTES_CURRENTLY_ALOCATED(mh) debug_mh_get_bytes_currently_alloced(mh)
    #define DEBUG_BYTES_FREED(mh) debug_mh_get_bytes_freed(mh)
#endif

// default memory handler
#define DEBUG_DEFAULT_INIT debug_mh_handler_default_init()
#define DEBUG_DEFAULT_QUIT debug_mh_handler_default_quit()

/** Asserts an expression and exits upon its evaluation to false
    @param expr expression to test
    @param z_expr string of expr condition
    @param file source file name (__FILENAME__) from where the logger has been called
    @param func function name (__FUNCTION__)
    @param line source file line (__LINE__)
    @param fmt formatting
    @param ... additional arguments
*/
extern void debug_assert(const bool expr, const char *const z_expr, const char *const file, const char *const func, const int32_t line, const char *const fmt, ...);

/** Debug info print
    @param fmt formatting
    @param ... additional arguments
*/
extern void debug_printf(const char *const fmt, ...);

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

/** Creates a mh handler
    @returns handler pointer, `NULL` upon failure
*/
extern debug_mh_t *debug_mh_handler_new(void);

/** Frees mh handler
    @param mh memory handler instance
*/
extern void debug_mh_handler_free(debug_mh_t *mh);

/** Checks if mh was initialized
    @param mh memory handler instance
    @returns true if it was initialized
*/
extern bool debug_mh_handler_is_init(const debug_mh_t *const mh);

/** Creates the default internal mh handler
*/
extern bool debug_mh_handler_default_init(void);

/** Frees the default internal mh handler
*/
extern void debug_mh_handler_default_quit(void);

/** Returns the default internal mh handler instance
    @returns handler pointer
*/
extern debug_mh_t *debug_mh_handler_default_get_handler(void);

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

/** Allocates memory like malloc
    @param mh memory handler instance
    @param bytes memory size in bytes
    @param file source file name (__FILENAME__) from where the logger has been called
    @param func function name (__FUNCTION__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure
*/
extern void *debug_mh_malloc(debug_mh_t *const mh, const size_t bytes, const char *const func, const char *const file, const int32_t line);

/** Allocates memory like calloc
    @param mh memory handler instance
    @param bytes memory size in bytes
    @param file source file name (__FILENAME__) from where the logger has been called
    @param func function name (__FUNCTION__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure
*/
extern void *debug_mh_calloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Rellocates memory like realloc
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param bytes memory size in bytes
    @param file source file name (__FILENAME__) from where the logger has been called
    @param func function name (__FUNCTION__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure

    @note
        It's meant to work with mh instance. 
        If ptr is not found the mh cache, then nothing happens.
*/
extern void *debug_mh_realloc(debug_mh_t *const mh, void *ptr, const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Frees memory like free
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param file source file name (__FILENAME__) from where the logger has been called
    @param func function name (__FUNCTION__)
    @param line source file line (__LINE__)

    @note
        It's meant to work with mh instance. 
        If ptr is not found the mh cache, then nothing happens.
*/
extern void debug_mh_free(debug_mh_t *const mh, void *ptr, const char *const file, const char *const func, const int32_t line);

/** Adds allocated memory to memory handler cache
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param bytes memory size in bytes
*/
extern void debug_mh_add(debug_mh_t *const mh, const void *const ptr, const size_t bytes);

/** Removes allocated memory from memory handler cache
    @param mh memory handler instance
    @param ptr pointer to current memory
*/
extern size_t debug_mh_remove(debug_mh_t *const mh, const void *const ptr);

/* ---------------------- STATS FUNCTIONS ---------------------- */

/** Returns number of allocations 
    @param mh memory handler instance
    @returns number of allocations
*/
extern size_t debug_mh_get_nallocs(const debug_mh_t *const mh);

/** Returns number of reallocations 
    @param mh memory handler instance
    @returns number of reallocations
*/
extern size_t debug_mh_get_nreallocs(const debug_mh_t *const mh);

/** Returns number of frees 
    @param mh memory handler instance
    @returns number of frees
*/
extern size_t debug_mh_get_nfrees(const debug_mh_t *const mh);

/** Returns number of bytes totally allocated throughout the program 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t debug_mh_get_bytes_totally_alloced(const debug_mh_t *const mh);

/** Returns number of bytes currently allocated 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t debug_mh_get_bytes_currently_alloced(const debug_mh_t *const mh);

/** Returns number of bytes freed throughout the program 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t debug_mh_get_bytes_freed(const debug_mh_t *const mh);

/** Returns a formatted string with stats
    @param mh memory handler instance
    @param buffer buffer to store formatted stats; if `NULL` is specified, allocates
    @param buff_len buffer length (must be >= 256)

    @returns C string char*
*/
extern char *debug_mh_get_stats_str(const debug_mh_t *const mh, char *buffer, const size_t buff_len);

/** Prints a formatted string with stats
    @param mh memory handler instance
*/
extern void debug_mh_print_stats(const debug_mh_t *const mh);

#endif // VITA_DEBUG_H
