#ifndef VITA_UTIL_DEBUG_H
#define VITA_UTIL_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - VT_DEBUG_ASSERT
    - VT_DEBUG_ASSERTC
    - VT_DEBUG_PRINTF
    - VT_CHECK
    - VT_ENFORCE
    - VT_NOTE
    - VT_REQUIRED
    - VT_TODO
    - VT_UNIMPLEMENTED

 * Functions
    - vt_debug_assert
    - vt_debug_printf
    - vt_debug_redirect_output
    - vt_debug_disable_output
*/

#include <math.h>
#include "../core/core.h"
#include "../time/datetime.h"

/**************** ERROR HANDLING ****************/
#if !defined(NDEBUG)
    #define VT_DEBUG_ASSERT(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), "DEBUG ASSERTION FAILURE", true, __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)
    #define VT_DEBUG_ASSERTC(expr, file, func, line, ...) vt_debug_assert(expr, VT_STRING_OF(expr), "DEBUG ASSERTION FAILURE", true, file, func, line, __VA_ARGS__)
    #define VT_DEBUG_PRINTF(...) vt_debug_printf("DEBUG INFO", __VA_ARGS__)
#else
    #define VT_DEBUG_ASSERT(expr, ...)
    #define VT_DEBUG_ASSERTC(expr, file, func, line, ...)
    #define VT_DEBUG_PRINTF(...)
#endif

// check for a condition (outputs a message in case of error);  isn't removed in release builds
#define VT_CHECK(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), "CHECK FAILURE", false, __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

// the same as assert, but isn't removed in release builds
#define VT_ENFORCE(expr, ...) vt_debug_assert(expr, VT_STRING_OF(expr), "ENFORCE FAILURE", true,  __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

// leave a note, move on
#define VT_NOTE(...) vt_debug_assert(false, "!", "NOTE", false, __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

// output important information, move on
#define VT_REQUIRED(...) vt_debug_assert(false, "!", "REQUIRED", false, __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

// output msg, then quit
#define VT_TODO(...) vt_debug_assert(false, "!", "TODO", true, __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

// output msg, then quit
#define VT_UNIMPLEMENTED(...) vt_debug_assert(false, "!", "UNIMPLEMENTED", true, __SOURCE_FILENAME__, __func__, __LINE__, __VA_ARGS__)

/** Asserts an expression and exits upon its evaluation to false
    @param expr expression to test
    @param zexpr string of expr condition
    @param subject message subject
    @param quit_on_failure quit program if `expr` condition failes
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param func function name (__func__)
    @param line source file line (__LINE__)
    @param zfmt formatting
    @param ... additional arguments
*/
extern void vt_debug_assert(
    const bool expr, 
    const char *const zexpr, 
    const char *const subject, 
    const bool quit_on_failure, 
    const char *const file, 
    const char *const func, 
    const size_t line, 
    const char *const zfmt, 
    ...
);

/** Debug info print
    @param subject message subject
    @param zfmt formatting
    @param ... additional arguments
*/
extern void vt_debug_printf(const char *const subject, const char *const zfmt, ...);

/** Redirects all output to the specified file
    @param zfilename file name
    @note if `zfilename` is NULL, stderr is used by default
*/
extern void vt_debug_redirect_output(const char *const zfilename);

/** Disables all debug output completely
    @param off true or false
*/
extern void vt_debug_disable_output(const bool off);

#endif // VITA_UTIL_DEBUG_H
