#include "vita/util/log.h"

// log level strings for printing
static const char *const log_level_strings[] = {
    "INFO", "WARN", "DEBUG", "ERROR", "FATAL", "ASSERT FAILURE"
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

void log_log(enum LogLevel log_level, const bool expr, const char *const file, const int32_t line, const char *const fmt, ...) {
    if(!expr) {
        if(log_level < 0 || log_level >= ll_count) {
            log_level = ll_info;
        }

        // get time
        char tbuf[21];
        get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, fmt); 
        {
            // logging to stderr, else to file
            if(log_filenames[log_level] == NULL) {
                fprintf(stderr, "%s %5s %s:%d: ", tbuf, log_get_level_string(log_level), file, line);
                vfprintf(stderr, fmt, args);
                fprintf(stderr, "\n");
            } else {
                // open file
                FILE *fp = fopen(log_filenames[log_level], "a");
                if(fp == NULL) {
                    return;
                }
                
                // write to file
                fprintf(fp, "%s %5s %s:%d: ", tbuf, log_get_level_string(log_level), file, line);
                vfprintf(fp, fmt, args);
                fprintf(fp, "\n");
                
                // close file
                fclose(fp);
            }
        }
        va_end(args);

        // if log level = fatal or assert, exit
        if(log_level == ll_fatal || log_level == ll_assert) {
            exit(EXIT_FAILURE);
        }
    }
}


