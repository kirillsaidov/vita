#ifndef VITA_VT_LOG_H
#define VITA_VT_LOG_H

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
    - vt_log_set_level
    - vt_log_set_level_default
    - vt_log_get_level_str
    - vt_log
*/

#include "../core/core.h"
#include "../system/fileio.h"

#if defined(_WIN32) || defined(_WIN64)
    #define strncpy(d, s, n) strncpy_s(d, n, s, n)
#endif

// Log levels
enum VitaLogLevel {
    vt_ll_info,    // write to file/stderr, move on
    vt_ll_warn,    // write to file/stderr, move on
    vt_ll_debug,   // write to file/stderr, move on
    vt_ll_error,   // write to file/stderr, move on
    vt_ll_fatal,   // write to file/stderr, exit program
    vt_ll_assert,  // write to file/stderr upon expression being evaluated to false
    vt_ll_count    // number of elements
};

/* ---------------- GLOBAL LOGGER BASED ON LOG LEVEL ---------------- */
#define VT_LOG_INFO(...) vt_log(NULL, vt_ll_info, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_WARN(...) vt_log(NULL, vt_ll_warn, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_DEBUG(...) vt_log(NULL, vt_ll_debug, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_ERROR(...) vt_log(NULL, vt_ll_error, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_FATAL(...) vt_log(NULL, vt_ll_fatal, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOG_ASSERT(expr, ...) vt_log(NULL, vt_ll_assert, expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)

/* ------------- CUSTOM FILE LOGGER BASED ON LOG LEVEL ------------- */
#define VT_LOGF_INFO(zfilename, ...) vt_log(zfilename, vt_ll_info, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_WARN(zfilename, ...) vt_log(zfilename, vt_ll_warn, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_DEBUG(zfilename, ...) vt_log(zfilename, vt_ll_debug, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_ERROR(zfilename, ...) vt_log(zfilename, vt_ll_error, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_FATAL(zfilename, ...) vt_log(zfilename, vt_ll_fatal, false, NULL, __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)
#define VT_LOGF_ASSERT(zfilename, expr, ...) vt_log(zfilename, vt_ll_assert, expr, VT_STRING_OF(expr), __SOURCE_FILENAME__, __LINE__, __VA_ARGS__)

/** Sets custom log level
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @param filename file name to log to
*/
extern void vt_log_set_level(enum VitaLogLevel vt_log_level, const char *const filename);

/** Sets default log level for all levels
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @param filename file name to log to
*/
extern void vt_log_set_level_default(const char *const filename);

/** Returns log level string
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @returns const char*
*/
extern const char *vt_log_get_level_str(enum VitaLogLevel vt_log_level);

/** Global logger
    @param zfilename filename to log info to; if `NULL` is passed, uses `stderr`
    @param vt_log_level enum VitaLogLevel (if invalid vt_log_level is specified, VT_LOG_INFO is used)
    @param expr expression to test
    @param zexpr a string representation of exp
    @param file source file name (__SOURCE_FILENAME__) from where the logger has been called
    @param line source file line (__LINE__)
    @param zfmt formatting
    @param ... additional arguments
*/
extern void vt_log(const char *const zfilename, enum VitaLogLevel vt_log_level, const bool expr, const char *const zexpr, const char *const file, const int32_t line, const char *const zfmt, ...);

#endif // VITA_VT_LOG_H



