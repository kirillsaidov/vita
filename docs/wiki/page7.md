# Logger
Read more below or check out [`util/log.h`](../../inc/vita/util/log.h).

## Contents
* [Logger setup](page7.md#logger-setup)
* [Logging to `stderr`](page7.md#logging-to-stderr)
* [Logging to file](page7.md#logging-to-file)
* [Custom logger](page7.md#custom-logger)

### Logger setup
Vita provides 6 log levels: 

```c
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
```

Vita logger is easy configure. By default Vita logs data to `stderr`, unless specified otherwise.

```c
#include "vita/util/log.h"

// log file
const char *log_file = "my_log_output.log";

// redirect output to "my_info_logger_file.log"
vt_log_redirect_level_output(vt_log_info, log_file); 

// redirect output to stderr
vt_log_redirect_level_output(vt_log_info, NULL); 
vt_log_redirect_level_output(vt_log_info, stderr); // ditto

// redirect all log levels to a file
vt_log_redirect_all_output(log_file);
vt_log_redirect_all_output(NULL);                  // reset to stderr
```

You can also get log level string:

```c
const char *log_level_str = vt_log_get_level_str(vt_log_info);
assert(vt_str_equals_z("INFO", log_level_str));
```

### Logging to `stderr`

```c
// log to stderr unless you specified otherwise above
VT_LOG_INFO("[s] This is an info log entry.");
VT_LOG_WARN("[s] WRONG input specified: %s", "LOG");
VT_LOG_DEBUG("[s] Log only if NDEBUG is not defined. Testing: = %s, %s", "debug msg", "debug2");
VT_LOG_ERROR("[s] Error: %s", "exit...");
VT_LOG_FATAL("[s] Error: %s", "fatal error encountered. exit...");
VT_LOG_ASSERT(val == 2, "Log if assertion fails: %s", "assertion failed");
```

### Logging to file

```c
// log to a specified file
VT_LOGF_INFO(logger_filename, "This is a test. %s", "Trying out new functionality.");
VT_LOGF_WARN(logger_filename, "ditto");
VT_LOGF_DEBUG(logger_filename, "ditto");
VT_LOGF_ERROR(logger_filename, "ditto");
VT_LOGF_FATAL(logger_filename, "ditto");
VT_LOGF_ASSERT(logger_filename, val == 2, "ditto");
```

### Custom logger

```c
// customize
void vt_log(
    const char *const zfilename,    // if NULL, checks if output was redirected by the user, otherwise use stderr
    enum VitaLogLevel log_level,    // log level
    const bool expr,                // expression to test
    const char *const zexpr,        // VT_STRING_OF(expr)
    const char *const file,         // __SOURCE_FILENAME__
    const size_t line,              // __LINE__
    const char *const zfmt,         // format
    ...                             // arguments
);
```

**[ [Back](page6.md) | [Next](page8.md) ]**

