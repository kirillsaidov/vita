#ifndef VITA_VT_DEBUG_H
#define VITA_VT_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - VT_DEBUG_ASSERT
    - VT_DEBUG_ASSERTC
    - VT_DEBUG_PRINTF
    - VT_DEBUG_MALLOC
    - VT_DEBUG_CALLOC
    - VT_DEBUG_REALLOC
    - VT_DEBUG_NALLOCS
    - VT_DEBUG_NREALLOCS
    - VT_DEBUG_NFREES
    - VT_DEBUG_BYTES_TOTALLY_ALOCATED
    - VT_DEBUG_BYTES_CURRENTLY_ALOCATED
    - VT_DEBUG_BYTES_FREED

 * Functions
    - vt_debug_assert
    - vt_debug_printf
    - vt_debug_handler_create
    - vt_debug_handler_destroy
    - vt_debug_handler_default_init
    - vt_debug_handler_default_quit
    - vt_debug_handler_default_get_handler
    - vt_debug_handler_malloc
    - vt_debug_handler_calloc
    - vt_debug_handler_realloc
    - vt_debug_handler_free
    - vt_debug_handler_add
    - vt_debug_handler_remove
    - vt_debug_handler_get_nallocs
    - vt_debug_handler_get_nreallocs
    - vt_debug_handler_get_nfrees
    - vt_debug_handler_get_bytes_totally_alloced
    - vt_debug_handler_get_bytes_currently_alloced
    - vt_debug_handler_get_bytes_freed
    - vt_debug_handler_get_stats_str
    - vt_debug_handler_print_stats
*/

#include <math.h>
#include "../core/core.h"
#include "../time/datetime.h"

// memory handler instance
typedef struct VitaDebugMemoryHandler vt_debug_handler_t;

#if !defined(NDEBUG)
    /**************** ERROR HANDLING ****************/
    #define VT_DEBUG_ASSERT(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)
    #define VT_DEBUG_ASSERTC(expr, file, func, line, ...) vt_debug_assert(expr, VT_STRING_OF(expr), file, func, line, __VA_ARGS__)
    #define VT_DEBUG_PRINTF(...) vt_debug_printf(__VA_ARGS__)

    /************ DEFAULT MEMORY HANDLER ************/
    #define VT_DEBUG_DEFAULT_INIT() vt_debug_handler_default_init()
    #define VT_DEBUG_DEFAULT_QUIT() vt_debug_handler_default_quit()
    #define VT_DEBUG_MALLOC(bytes) vt_debug_handler_malloc(vt_debug_handler_default_get_handler(), bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define VT_DEBUG_CALLOC(bytes) vt_debug_handler_calloc(vt_debug_handler_default_get_handler(), bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define VT_DEBUG_REALLOC(ptr, bytes) vt_debug_handler_realloc(vt_debug_handler_default_get_handler(), ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define VT_DEBUG_FREE(ptr) vt_debug_handler_free(vt_debug_handler_default_get_handler(), ptr, __SOURCE_FILENAME__, __func__, __LINE__)

    // stats
    #define VT_DEBUG_NALLOCS vt_debug_handler_get_nallocs(vt_debug_handler_default_get_handler())
    #define VT_DEBUG_NREALLOCS vt_debug_handler_get_nreallocs(vt_debug_handler_default_get_handler())
    #define VT_DEBUG_NFREES vt_debug_handler_get_nfrees(vt_debug_handler_default_get_handler())
    #define VT_DEBUG_BYTES_TOTALLY_ALOCATED vt_debug_handler_get_bytes_totally_alloced(vt_debug_handler_default_get_handler())
    #define VT_DEBUG_BYTES_CURRENTLY_ALOCATED vt_debug_handler_get_bytes_currently_alloced(vt_debug_handler_default_get_handler())
    #define VT_DEBUG_BYTES_FREED vt_debug_handler_get_bytes_freed(vt_debug_handler_default_get_handler())

    /************ CUSTOM MEMORY HANDLER ************/
    #define VT_DEBUG_CREATE_MH() vt_debug_handler_create()
    #define VT_DEBUG_DESTROY_MH(mh) vt_debug_handler_destroy(mh)
    #define VT_DEBUG_MALLOC_MH(mh, bytes) vt_debug_handler_malloc(mh, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define VT_DEBUG_CALLOC_MH(mh, bytes) vt_debug_handler_calloc(mh, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define VT_DEBUG_REALLOC_MH(mh, ptr, bytes) vt_debug_handler_realloc(mh, ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
    #define VT_DEBUG_FREE_MH(mh, ptr) vt_debug_handler_free(mh, ptr, __SOURCE_FILENAME__, __func__, __LINE__)

    // stats
    #define VT_DEBUG_NALLOCS_MH(mh) vt_debug_handler_get_nallocs(mh)
    #define VT_DEBUG_NREALLOCS_MH(mh) vt_debug_handler_get_nreallocs(mh)
    #define VT_DEBUG_NFREES_MH(mh) vt_debug_handler_get_nfrees(mh)
    #define VT_DEBUG_BYTES_TOTALLY_ALOCATED_MH(mh) vt_debug_handler_get_bytes_totally_alloced(mh)
    #define VT_DEBUG_BYTES_CURRENTLY_ALOCATED_MH(mh) vt_debug_handler_get_bytes_currently_alloced(mh)
    #define VT_DEBUG_BYTES_FREED_MH(mh) vt_debug_handler_get_bytes_freed(mh)
#else
    // error handling
    #define VT_DEBUG_ASSERT(expr, ...)
    #define VT_DEBUG_ASSERTC(expr, file, func, line, ...)
    #define VT_DEBUG_PRINTF(...)

    // memory management
    #define VT_DEBUG_DEFAULT_INIT()
    #define VT_DEBUG_DEFAULT_QUIT()
    #define VT_DEBUG_MALLOC(bytes) malloc(bytes)
    #define VT_DEBUG_CALLOC(bytes) calloc(1, bytes)
    #define VT_DEBUG_REALLOC(ptr, bytes) realloc(ptr, bytes)
    #define VT_DEBUG_FREE(ptr) free(ptr)

    // stats
    #define VT_DEBUG_NALLOCS(mh) vt_debug_handler_get_nallocs(mh)
    #define VT_DEBUG_NREALLOCS(mh) vt_debug_handler_get_nreallocs(mh)
    #define VT_DEBUG_NFREES(mh) vt_debug_handler_get_nfrees(mh)
    #define VT_DEBUG_BYTES_TOTALLY_ALOCATED(mh) vt_debug_handler_get_bytes_totally_alloced(mh)
    #define VT_DEBUG_BYTES_CURRENTLY_ALOCATED(mh) vt_debug_handler_get_bytes_currently_alloced(mh)
    #define VT_DEBUG_BYTES_FREED(mh) vt_debug_handler_get_bytes_freed(mh)
#endif

// the same as assert, but isn't removed in release builds
#define VT_ENFORCE(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

// required stats buffer size
#define VT_DEBUG_STATS_BUFFER_SIZE 256

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
extern void vt_debug_assert(const bool expr, const char *const zexpr, const char *const file, const char *const func, const size_t line, const char *const zfmt, ...);

/** Debug info print
    @param zfilename filename to redirect the output to; if `NULL` is passed, `stderr` is used
    @param zfmt formatting
    @param ... additional arguments
*/
extern void vt_debug_printf(const char *const zfmt, ...);

/** Redirects all output to the specified file
    @param zfilename file name
    @note if `zfilename` is NULL, stderr is used by default
*/
extern void vt_debug_redirect_output(const char *const zfilename);

/** Disables all debug output completely
    @param off true or false
*/
extern void vt_debug_disable_output(const bool off);

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

/** Creates a mh handler
    @returns handler pointer, `NULL` upon failure
*/
extern vt_debug_handler_t *vt_debug_handler_create(void);

/** Frees mh handler
    @param mh memory handler instance
*/
extern void vt_debug_handler_destroy(vt_debug_handler_t *mh);

/** Creates the default internal mh handler
*/
extern bool vt_debug_handler_default_init(void);

/** Frees the default internal mh handler
*/
extern void vt_debug_handler_default_quit(void);

/** Returns the default internal mh handler instance
    @returns handler pointer
*/
extern vt_debug_handler_t *vt_debug_handler_default_get_handler(void);

// /** Redirects memory handler specific output to the specified file
//     @param zfilename file name
//     @note if `zfilename` is NULL, stderr is used by default
// */
// extern void vt_debug_handler_redirect_output(const char *const zfilename);

// /** Disables memory handler specific debug output completely
//     @param on true or false
// */
// extern void vt_debug_handler_disable_output(const bool on);

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

/** Allocates memory like malloc
    @param mh memory handler instance
    @param bytes memory size in bytes
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure
*/
extern void *vt_debug_handler_malloc(vt_debug_handler_t *const mh, const size_t bytes, const char *const func, const char *const file, const size_t line);

/** Allocates memory like calloc
    @param mh memory handler instance
    @param bytes memory size in bytes
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)

    @returns pointer to allocated block of memory, `NULL` upon failure
*/
extern void *vt_debug_handler_calloc(vt_debug_handler_t *const mh, const size_t bytes, const char *const file, const char *const func, const size_t line);

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
extern void *vt_debug_handler_realloc(vt_debug_handler_t *const mh, void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line);

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
extern void vt_debug_handler_free(vt_debug_handler_t *const mh, void *ptr, const char *const file, const char *const func, const size_t line);

/** Adds allocated memory to memory handler cache
    @param mh memory handler instance
    @param ptr pointer to current memory
    @param bytes memory size in bytes
*/
extern void vt_debug_handler_add(vt_debug_handler_t *const mh, const void *const ptr, const size_t bytes);

/** Removes allocated memory from memory handler cache
    @param mh memory handler instance
    @param ptr pointer to current memory
*/
extern size_t vt_debug_handler_remove(vt_debug_handler_t *const mh, const void *const ptr);

/* ---------------------- STATS FUNCTIONS ---------------------- */

/** Returns number of allocations 
    @param mh memory handler instance
    @returns number of allocations
*/
extern size_t vt_debug_handler_get_nallocs(const vt_debug_handler_t *const mh);

/** Returns number of reallocations 
    @param mh memory handler instance
    @returns number of reallocations
*/
extern size_t vt_debug_handler_get_nreallocs(const vt_debug_handler_t *const mh);

/** Returns number of frees 
    @param mh memory handler instance
    @returns number of frees
*/
extern size_t vt_debug_handler_get_nfrees(const vt_debug_handler_t *const mh);

/** Returns number of bytes totally allocated throughout the program 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t vt_debug_handler_get_bytes_totally_alloced(const vt_debug_handler_t *const mh);

/** Returns number of bytes currently allocated 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t vt_debug_handler_get_bytes_currently_alloced(const vt_debug_handler_t *const mh);

/** Returns number of bytes freed throughout the program 
    @param mh memory handler instance
    @returns number of bytes
*/
extern size_t vt_debug_handler_get_bytes_freed(const vt_debug_handler_t *const mh);

/** Returns a formatted string with stats
    @param mh memory handler instance
    @param buffer buffer to store formatted stats; if `NULL` is specified, allocates
    @param buff_len buffer length (must be >= 256)

    @returns C string char*
*/
extern char *vt_debug_handler_get_stats_str(const vt_debug_handler_t *const mh, char *buffer, const size_t buff_len);

/** Prints a formatted string with stats
    @param mh memory handler instance
*/
extern void vt_debug_handler_print_stats(const vt_debug_handler_t *const mh);

#endif // VITA_VT_DEBUG_H
