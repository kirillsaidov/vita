#ifndef VITA_VT_DEBUG_H
#define VITA_VT_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - VT_DEBUG_ASSERT
    - VT_DEBUG_ASSERTC
    - VT_DEBUG_PRINTF
    - VT_ENFORCE

 * Functions
    - vt_debug_assert
    - vt_debug_printf
*/

#include <math.h>
#include "../core/core.h"
#include "../time/datetime.h"

// memory handler instance
typedef struct VitaDebugMemoryHandler vt_debug_handler_t;

/**************** ERROR HANDLING ****************/
#if !defined(NDEBUG)
    #define VT_DEBUG_ASSERT(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)
    #define VT_DEBUG_ASSERTC(expr, file, func, line, ...) vt_debug_assert(expr, VT_STRING_OF(expr), file, func, line, __VA_ARGS__)
    #define VT_DEBUG_PRINTF(...) vt_debug_printf(__VA_ARGS__)
#else
    #define VT_DEBUG_ASSERT(expr, ...)
    #define VT_DEBUG_ASSERTC(expr, file, func, line, ...)
    #define VT_DEBUG_PRINTF(...)
#endif

// the same as assert, but isn't removed in release builds
#define VT_ENFORCE(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

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

#endif // VITA_VT_DEBUG_H
