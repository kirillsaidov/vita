#include "vita/util/log.h"

// log level strings for printing
static const char *const log_level_strings[] = {
    "INFO", "WARN", "DEBUG", "ERROR", "FATAL"
};

// log to file
static const char *log_filenames[ll_count - 1] = {0};

void log_set_level(enum LogLevel log_level, const char *const cs_filename) {
    if(log_level < 0 || log_level >= ll_count) {
        log_level = ll_info;
    }
    
    // update log filenames
    log_filenames[log_level] = cs_filename;
}

void log_set_level_default(const char *const cs_filename) {
    for(size_t i = 0; i < ll_count; i++) {
        log_filenames[i] = cs_filename;
    }
}

const char *log_get_level_string(enum LogLevel log_level) {
    if(log_level < 0 || log_level >= ll_count) {
        log_level = ll_info;
    }

    return log_level_strings[log_level];
}

void log_log(enum LogLevel log_level, const char *const cs_file, const int line, const char *const cs_fmt, ...) {
    if(log_level < 0 || log_level >= ll_count) {
        log_level = ll_info;
    }

    //...
    printf("%s log_log test...\n", log_get_level_string(log_level));
}







