#ifndef VITA_DEBUG_H
#define VITA_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - DEBUG_ASSERT
    - DEBUG_ASSERTC
    - DEBUG_PRINTF
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
    - debug_handler_create
    - debug_handler_destroy
    - debug_handler_default_init
    - debug_handler_default_quit
    - debug_handler_default_get_handler
    - debug_handler_malloc
    - debug_handler_calloc
    - debug_handler_realloc
    - debug_handler_free
    - debug_handler_add
    - debug_handler_remove
    - debug_handler_get_nallocs
    - debug_handler_get_nreallocs
    - debug_handler_get_nfrees
    - debug_handler_get_bytes_totally_alloced
    - debug_handler_get_bytes_currently_alloced
    - debug_handler_get_bytes_freed
    - debug_handler_get_stats_str
    - debug_handler_print_stats
*/

#include <math.h>
#include "../core/core.h"

// memory handler instance
typedef struct DebugMemoryHandler debug_handler_t;

#ifndef NDEBUG
    /**************** ERROR HANDLING ****************/
    #define DEBUG_ASSERT(expr, ...) debug_assert(expr, STRINGOF(expr), __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)
    #define DEBUG_ASSERTC(expr, file, func, line, ...) debug_assert(expr, STRINGOF(expr), file, func, line, __VA_ARGS__)
    #define DEBUG_PRINTF(...) debug_printf(__VA_ARGS__)

    /************ DEFAULT MEMORY HANDLER ************/
    #define DEBUG_DEFAULT_INIT() debug_handler_default_init()
    #define DEBUG_DEFAULT_QUIT() debug_handler_default_quit()
    #define DEBUG_MALLOC(bytes) debug_handler_malloc(debug_handler_default_get_handler(), bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define DEBUG_CALLOC(bytes) debug_handler_calloc(debug_handler_default_get_handler(), bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define DEBUG_REALLOC(ptr, bytes) debug_handler_realloc(debug_handler_default_get_handler(), ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define DEBUG_FREE(ptr) debug_handler_free(debug_handler_default_get_handler(), ptr, __SOURCE_FILENAME__, __func__, __LINE__)

    // stats
    #define DEBUG_NALLOCS debug_handler_get_nallocs(debug_handler_default_get_handler())
    #define DEBUG_NREALLOCS debug_handler_get_nreallocs(debug_handler_default_get_handler())
    #define DEBUG_NFREES debug_handler_get_nfrees(debug_handler_default_get_handler())
    #define DEBUG_BYTES_TOTALLY_ALOCATED debug_handler_get_bytes_totally_alloced(debug_handler_default_get_handler())
    #define DEBUG_BYTES_CURRENTLY_ALOCATED debug_handler_get_bytes_currently_alloced(debug_handler_default_get_handler())
    #define DEBUG_BYTES_FREED debug_handler_get_bytes_freed(debug_handler_default_get_handler())

    /************ CUSTOM MEMORY HANDLER ************/
    #define DEBUG_CREATE_MH() debug_handler_create()
    #define DEBUG_DESTROY_MH(mh) debug_handler_destroy(mh)
    #define DEBUG_MALLOC_MH(mh, bytes) debug_handler_malloc(mh, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define DEBUG_CALLOC_MH(mh, bytes) debug_handler_calloc(mh, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define DEBUG_REALLOC_MH(mh, ptr, bytes) debug_handler_realloc(mh, ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define DEBUG_FREE_MH(mh, ptr) debug_handler_free(mh, ptr, __SOURCE_FILENAME__, __func__, __LINE__)

    // stats
    #define DEBUG_NALLOCS_MH(mh) debug_handler_get_nallocs(mh)
    #define DEBUG_NREALLOCS_MH(mh) debug_handler_get_nreallocs(mh)
    #define DEBUG_NFREES_MH(mh) debug_handler_get_nfrees(mh)
    #define DEBUG_BYTES_TOTALLY_ALOCATED_MH(mh) debug_handler_get_bytes_totally_alloced(mh)
    #define DEBUG_BYTES_CURRENTLY_ALOCATED_MH(mh) debug_handler_get_bytes_currently_alloced(mh)
    #define DEBUG_BYTES_FREED_MH(mh) debug_handler_get_bytes_freed(mh)
#else
    // error handling
    #define DEBUG_ASSERT(expr, ...)
    #define DEBUG_ASSERT2(expr, file, func, line, ...)
    #define DEBUG_PRINTF(...)

    // memory management
    #define DEBUG_DEFAULT_INIT()
    #define DEBUG_DEFAULT_QUIT()
    #define DEBUG_MALLOC(bytes) malloc(bytes)
    #define DEBUG_CALLOC(bytes) calloc(1, bytes)
    #define DEBUG_REALLOC(ptr, bytes) realloc(ptr, bytes)
    #define DEBUG_FREE(ptr) free(ptr)

    // stats
    #define DEBUG_NALLOCS(mh) debug_handler_get_nallocs(mh)
    #define DEBUG_NREALLOCS(mh) debug_handler_get_nreallocs(mh)
    #define DEBUG_NFREES(mh) debug_handler_get_nfrees(mh)
    #define DEBUG_BYTES_TOTALLY_ALOCATED(mh) debug_handler_get_bytes_totally_alloced(mh)
    #define DEBUG_BYTES_CURRENTLY_ALOCATED(mh) debug_handler_get_bytes_currently_alloced(mh)
    #define DEBUG_BYTES_FREED(mh) debug_handler_get_bytes_freed(mh)
#endif

/** Asserts an expression and exits upon its evaluation to false
    @param zfilename filename to redirect the output to; if `NULL` is passed, `stderr` is used
    @param expr expression to test
    @param zexpr string of expr condition
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)
    @param zfmt formatting
    @param ... additional arguments
*/
extern void debug_assert(const bool expr, const char *const zexpr, const char *const file, const char *const func, const int32_t line, const char *const zfmt, ...);

/** Debug info print
    @param zfilename filename to redirect the output to; if `NULL` is passed, `stderr` is used
    @param zfmt formatting
    @param ... additional arguments
*/
extern void debug_printf(const char *const zfmt, ...);

/** Redirects all output to the specified file
    @param zfilename file name
    @note if `zfilename` is NULL, stderr is used by default
*/
extern void debug_redirect_output(const char *const zfilename);

/** Disables all debug output completely
    @param off true or false
*/
extern void debug_disable_output(const bool off);

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

/** Creates a mh handler
    @returns handler pointer, `NULL` upon failure
*/
extern debug_handler_t *debug_handler_create(void);

/** Frees mh handler
    @param mh memory handler instance
*/
extern void debug_handler_destroy(debug_handler_t *mh);

/** Checks if mh was initialized
    @param mh memory handler instance
    @returns true if it was initialized
*/
extern bool debug_handler_is_init(const debug_handler_t *const mh);

/** Creates the default internal mh handler
*/
extern bool debug_handler_default_init(void);

/** Frees the default internal mh handler
*/
extern void debug_handler_default_quit(void);

/** Returns the default internal mh handler instance
    @returns handler pointer
*/
extern debug_handler_t *debug_handler_default_get_handler(void);

// /** Redirects memory handler specific output to the specified file
//     @param zfilename file name
//     @note if `zfilename` is NULL, stderr is used by default
// */
// extern void debug_handler_redirect_output(const char *const zfilename);

// /** Disables memory handler specific debug output completely
//     @param on true or false
// */
// extern void debug_handler_disable_output(const bool on);

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

/** Allocates memory like malloc
    @param mh memory handler instance
    @param bytes memory size in bytes
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure
*/
extern void *debug_handler_malloc(debug_handler_t *const mh, const size_t bytes, const char *const func, const char *const file, const int32_t line);

/** Allocates memory like calloc
    @param mh memory handler instance
    @param bytes memory size in bytes
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure
*/
extern void *debug_handler_calloc(debug_handler_t *const mh, const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Rellocates memory like realloc
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param bytes memory size in bytes
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure

    @note
        It's meant to work with mh instance. 
        If ptr is not found the mh cache, then nothing happens.
*/
extern void *debug_handler_realloc(debug_handler_t *const mh, void *ptr, const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Frees memory like free
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)

    @note
        It's meant to work with mh instance. 
        If ptr is not found the mh cache, then nothing happens.
*/
extern void debug_handler_free(debug_handler_t *const mh, void *ptr, const char *const file, const char *const func, const int32_t line);

/** Adds allocated memory to memory handler cache
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param bytes memory size in bytes
*/
extern void debug_handler_add(debug_handler_t *const mh, const void *const ptr, const size_t bytes);

/** Removes allocated memory from memory handler cache
    @param mh memory handler instance
    @param ptr pointer to current memory
*/
extern size_t debug_handler_remove(debug_handler_t *const mh, const void *const ptr);

/* ---------------------- STATS FUNCTIONS ---------------------- */

/** Returns number of allocations 
    @param mh memory handler instance
    @returns number of allocations
*/
extern size_t debug_handler_get_nallocs(const debug_handler_t *const mh);

/** Returns number of reallocations 
    @param mh memory handler instance
    @returns number of reallocations
*/
extern size_t debug_handler_get_nreallocs(const debug_handler_t *const mh);

/** Returns number of frees 
    @param mh memory handler instance
    @returns number of frees
*/
extern size_t debug_handler_get_nfrees(const debug_handler_t *const mh);

/** Returns number of bytes totally allocated throughout the program 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t debug_handler_get_bytes_totally_alloced(const debug_handler_t *const mh);

/** Returns number of bytes currently allocated 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t debug_handler_get_bytes_currently_alloced(const debug_handler_t *const mh);

/** Returns number of bytes freed throughout the program 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t debug_handler_get_bytes_freed(const debug_handler_t *const mh);

/** Returns a formatted string with stats
    @param mh memory handler instance
    @param buffer buffer to store formatted stats; if `NULL` is specified, allocates
    @param buff_len buffer length (must be >= 256)

    @returns C string char*
*/
extern char *debug_handler_get_stats_str(const debug_handler_t *const mh, char *buffer, const size_t buff_len);

/** Prints a formatted string with stats
    @param mh memory handler instance
*/
extern void debug_handler_print_stats(const debug_handler_t *const mh);

#endif // VITA_DEBUG_H
