#ifndef VITA_LOG_H
#define VITA_LOG_H

/** VITA_LOG MODULE
 * Macros - GLOBAL LOGGER
    - LOG_INFO
    - LOG_WARN
    - LOG_DEBUG
    - LOG_ERROR
    - LOG_FATAL
    - LOG_ASSERT

 * Macros - CUSTOM LOGGING TO FILE
    - LOGF_INFO
    - LOGF_WARN
    - LOGF_DEBUG
    - LOGF_ERROR
    - LOGF_FATAL
    - LOGF_ASSERT

 * Functions
    - log_set_level
    - log_set_level_default
    - log_get_level_str
    - log_log
*/

#include "../core/core.h"
#include "../system/fileio.h"

// Log levels
enum LogLevel {
    ll_info,    // write to file/stderr, move on
    ll_warn,    // write to file/stderr, move on
    ll_debug,   // write to file/stderr, move on
    ll_error,   // write to file/stderr, move on
    ll_fatal,   // write to file/stderr, exit program
    ll_assert,  // write to file/stderr upon expression being evaluated to false
    ll_count    // number of elements
};

/* ---------------- GLOBAL LOGGER BASED ON LOG LEVEL ---------------- */
#define LOG_INFO(...) log_log(NULL, ll_info, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) log_log(NULL, ll_warn, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) log_log(NULL, ll_debug, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) log_log(NULL, ll_error, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) log_log(NULL, ll_fatal, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ASSERT(expr, ...) log_log(NULL, ll_assert, expr, STRINGOF(expr), __FILE__, __LINE__, __VA_ARGS__)

/* ------------- CUSTOM FILE LOGGER BASED ON LOG LEVEL ------------- */
#define LOGF_INFO(zfilename, ...) log_log(zfilename, ll_info, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOGF_WARN(zfilename, ...) log_log(zfilename, ll_warn, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOGF_DEBUG(zfilename, ...) log_log(zfilename, ll_debug, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOGF_ERROR(zfilename, ...) log_log(zfilename, ll_error, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOGF_FATAL(zfilename, ...) log_log(zfilename, ll_fatal, false, NULL, __FILE__, __LINE__, __VA_ARGS__)
#define LOGF_ASSERT(zfilename, expr, ...) log_log(zfilename, ll_assert, expr, STRINGOF(expr), __FILE__, __LINE__, __VA_ARGS__)

/** Sets custom log level
    @param log_level enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    @param filename file name to log to
*/
extern void log_set_level(enum LogLevel log_level, const char *const filename);

/** Sets default log level for all levels
    @param log_level enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    @param filename file name to log to
*/
extern void log_set_level_default(const char *const filename);

/** Returns log level string
    @param log_level enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    @returns const char*
*/
extern const char *log_get_level_str(enum LogLevel log_level);

/** Global logger
    @param zfilename filename to log info to; if `NULL` is passed, uses `stderr`
    @param log_level enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    @param expr expression to test
    @param zexpr a string representation of exp
    @param file source file name (__FILE__) from where the logger has been called
    @param line source file line (__LINE__)
    @param zfmt formatting
    @param ... additional arguments
*/
extern void log_log(const char *const zfilename, enum LogLevel log_level, const bool expr, const char *const zexpr, const char *const file, const int32_t line, const char *const zfmt, ...);

/** Custom logger to file
    @param zfilename filename to log info to; if `NULL` is passed, uses `stderr`
    @param log_level enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    @param expr expression to test
    @param zexpr a string representation of exp
    @param file source file name (__FILE__) from where the logger has been called
    @param line source file line (__LINE__)
    @param zfmt formatting
    @param ... additional arguments
*/
// extern void log_logf(const char *const zfilename, enum LogLevel log_level, const bool expr, const char *const zexpr, const char *const file, const int32_t line, const char *const zfmt, ...);

#endif // VITA_LOG_H








