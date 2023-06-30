# Logger
Read more below or check out [`util/log.h`](../../inc/vita/util/log.h).

## Contents
* [Logger setup](page7.md#logger-setup)
* [Logging to `stderr`](page7.md#logging-to-stderr)
* [Logging to file](page7.md#logging-to-file)
* [Custom logger](page7.md#custom-logger)

### Logger setup
Vita provides 5 log levels: 
```c
// Log levels
enum VitaLogLevel {
    vt_log_info,    // write to file/stderr, move on
    vt_log_warn,    // write to file/stderr, move on
    vt_log_debug,   // write to file/stderr if NDEBUG macro is not defined, move on
    vt_log_error,   // write to file/stderr, move on
    vt_log_fatal,   // write to file/stderr, exit program
    vt_log_assert,  // write to file/stderr if assertion fails, move on
    vt_log_count    // number of elements
};
```

Vita logger is easy configure. By default Vita logs data to `stderr`, unless specified otherwise.
```c
#include "vita/util/log.h"

// redirect output to "my_info_logger_file.log"
vt_log_set_level(vt_log_info, "my_info_logger_file.log"); 

// redirect output to stderr
vt_log_set_level(vt_log_info, NULL); 
vt_log_set_level(vt_log_info, stderr); // ditto

// redirect all log levels to a file
vt_log_set_level_all("my_log_output.log");
vt_log_set_level_all(NULL);            // reset
```

You can also get log level string:
```c
const char *log_level_str = vt_log_get_level_str(vt_log_info);
assert(strcmp("INFO", log_level_str));
```

### Logging to `stderr`
```c
VT_LOG_INFO("[s] This is an info log entry.");
VT_LOG_WARN("[s] WRONG input specified: %s", "LOG");
VT_LOG_DEBUG("[s] Log only if NDEBUG is not defined. Testing: = %s, %s", "debug msg", "debug2");
VT_LOG_ERROR("[s] Error: %s", "testing...");
VT_LOG_FATAL("[s] Error: %s", "fatal error encountered. exit...");
VT_LOG_ASSERT(val == 2, "Log if assertion fails: %s", "assertion failed");
```

### Logging to file
```c
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
    const char *const zfilename, 
    enum VitaLogLevel vt_log_level, 
    const bool expr, 
    const char *const zexpr, 
    const char *const file, 
    const size_t line, 
    const char *const zfmt, 
    ...
);
```

**[ [Back](page6.md) | [Next](page8.md) ]**
