#ifndef VITA_UTIL_LOG_H
#define VITA_UTIL_LOG_H

/** VITA_LOG MODULE
 * Macros - GLOBAL LOGGER
    - VT_LOG_INFO
    - VT_LOG_WARN
    - VT_LOG_DEBUG
    - VT_LOG_ERROR
    - VT_LOG_FATAL
    - VT_LOG_ASSERT

 * Macros - CUSTOM LOGGING TO FILE
    - VT_LOGF_INFO
    - VT_LOGF_WARN
    - VT_LOGF_DEBUG
    - VT_LOGF_ERROR
    - VT_LOGF_FATAL
    - VT_LOGF_ASSERT

 * Functions
    - vt_log_redirect_level_output
    - vt_log_redirect_all_output
    - vt_log_get_level_str
    - vt_log
*/

#include "../core/core.h"
#include "../system/fileio.h"
#include "../time/datetime.h"

// Log levels
enum VitaLogLevel {
    vt_log_info,    // write to file/stderr, move on
    vt_log_warn,    // write to file/stderr, move on
    vt_log_debug,   // write to file/stderr, move on
    vt_log_error,   // write to file/stderr, move on
    vt_log_fatal,   // write to file/stderr, exit program
    vt_log_assert,  // write to file/stderr, exit program
    vt_log_count    // number of elements
};

/* ---------------- GLOBAL LOGGER BASED ON LOG LEVEL ---------------- */

#define VT_LOG_INFO(...) vt_log(NULL, vt_log_info, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_WARN(...) vt_log(NULL, vt_log_warn, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)

#if !defined(NDEBUG)
    #define VT_LOG_DEBUG(...) vt_log(NULL, vt_log_debug, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#endif

#define VT_LOG_ERROR(...) vt_log(NULL, vt_log_error, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_FATAL(...) vt_log(NULL, vt_log_fatal, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_ASSERT(expr, ...) vt_log(NULL, vt_log_assert, expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)

/* ------------- CUSTOM FILE LOGGER BASED ON LOG LEVEL ------------- */

#define VT_LOGF_INFO(filename, ...) vt_log(filename, vt_log_info, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_WARN(filename, ...) vt_log(filename, vt_log_warn, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)

#if !defined(NDEBUG)
    #define VT_LOGF_DEBUG(filename, ...) vt_log(filename, vt_log_debug, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#endif

#define VT_LOGF_ERROR(filename, ...) vt_log(filename, vt_log_error, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_FATAL(filename, ...) vt_log(filename, vt_log_fatal, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_ASSERT(filename, expr, ...) vt_log(filename, vt_log_assert, expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)

/** Redirect log level to the specified output
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @param filename file name to log to

    @note if `NULL` is specified, redirects to stderr
*/
extern void vt_log_redirect_level_output(enum VitaLogLevel vt_log_level, const char *const filename);

/** Redirect all log levels to the specified output
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @param filename file name to log to

    @note if `NULL` is specified, redirects to stderr
*/
extern void vt_log_redirect_all_output(const char *const filename);

/** Get log level string
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @returns const char*
*/
extern const char *vt_log_get_level_str(enum VitaLogLevel vt_log_level);

/** Global logger
    @param filename filename to log info to; if `NULL` is passed, uses `stderr`
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @param expr expression to test
    @param zexpr a string representation of exp
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param line source file line (__LINE__)
    @param zfmt formatting
    @param ... additional arguments

    @note if `NULL` is specified for filename, redirects to stdout
*/
extern void vt_log(const char *const filename, enum VitaLogLevel vt_log_level, const bool expr, const char *const zexpr, const char *const file, const size_t line, const char *const zfmt, ...);

#endif // VITA_UTIL_LOG_H



