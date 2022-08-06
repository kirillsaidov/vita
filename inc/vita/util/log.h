#ifndef VITA_LOG_H
#define VITA_LOG_H

/** VITA_LOG MODULE
 * Function-like macros
    - LOG_INFO
    - LOG_WARN
    - LOG_DEBUG
    - LOG_ERROR
    - LOG_FATAL

 * Functions
    - log_set_level
    - log_set_level_default
    - log_get_level_string
    - log_log
*/

#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include "../system/fileio.h"

// Log levels
enum LogLevel {
    ll_info,    // write to file/stderr, move on
    ll_warn,    // write to file/stderr, move on
    ll_debug,   // write to file/stderr, move on
    ll_error,   // write to file/stderr, move on
    ll_fatal,   // write to file/stderr, exit program
    ll_count    // number of elements
};

// macros
#define LOG_INFO(...) log_log(ll_info, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) log_log(ll_warn, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) log_log(ll_debug, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) log_log(ll_error, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) log_log(ll_fatal, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ASSERT(expr, ...) log_assert(expr, __FILE__, __LINE__, __VA_ARGS__)

/**
Sets custom log level

Params:
    log_level = enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    filename = file name to log to
*/
void log_set_level(enum LogLevel log_level, const char *const filename);

/**
Sets default log level for all levels

Params:
    log_level = enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    filename = file name to log to
*/
void log_set_level_default(const char *const filename);

/**
Returns log level string

Params:
    log_level = enum LogLevel (if invalid log_level is specified, LOG_INFO is used)

Returns: const char*
*/
const char *log_get_level_string(enum LogLevel log_level);

/**
Custom logger

Params:
    log_level = enum LogLevel (if invalid log_level is specified, LOG_INFO is used)
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)
    fmt = string format
    ... = format variables
*/
void log_log(enum LogLevel log_level, const char *const file, const int32_t line, const char *const fmt, ...);

/**
Stderr logger and assert

Params:
    expr = expression to test
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)
    fmt = string format
    ... = format variables
*/
void log_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...);

#endif // VITA_LOG_H








