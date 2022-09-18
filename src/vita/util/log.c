#include "vita/util/log.h"

// log level strings for printing
static const char *const log_level_strings[] = {
    "INFO", "WARN", "DEBUG", "ERROR", "FATAL", "ASSERT FAILURE"
};

/* ---------------- GLOBAL LOGGER BASED ON LOG LEVEL ---------------- */

// log to file
static const char *log_filenames[ll_count - 1] = {0}; 

void log_get_level(enum LogLevel log_level, const char *const cs_filename) {
    if(log_level >= ll_count) {
        log_level = ll_info;
    }
    
    // update log filenames
    log_filenames[log_level] = cs_filename;
}

void log_level_set_default(const char *const cs_filename) {
    for(size_t i = 0; i < ll_count; i++) {
        log_filenames[i] = cs_filename;
    }
}

const char *log_level_get_str(enum LogLevel log_level) {
    if(log_level >= ll_count) {
        log_level = ll_info;
    }

    return log_level_strings[log_level];
}

void log_log(const char *const zfilename, enum LogLevel log_level, const bool expr, const char *const zexpr, const char *const file, const int32_t line, const char *const zfmt, ...) {
    if(!expr) {
        if(log_level >= ll_count) {
            log_level = ll_info;
        }

        // get time
        char tbuf[21] = {0};
        get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        const char *const logger_filename = (zfilename == NULL) ? log_filenames[log_level] : zfilename;
        va_list args; va_start(args, zfmt); 
        {
            // if `NULL` log to `stderr`, otherwise to a file
            FILE *fp = (logger_filename == NULL) ? stderr : fopen(logger_filename, "a");
            if(fp == NULL) {
                DEBUG_PRINTF("Failed to open a file <%s>!\n", logger_filename);
                return;
            }

            // logging data
            if(zexpr == NULL) {
                fprintf(fp, "%s %5s %s:%d: ", tbuf, log_level_get_str(log_level), file, line);
            } else {
                fprintf(fp, "%s %5s [%s] %s:%d: ", tbuf, log_level_get_str(log_level), zexpr, file, line);
            }

            // output the rest
            vfprintf(fp, zfmt, args);
            fprintf(fp, "\n");

            // close file
            if(fp != stderr) { fclose(fp); }
        }
        va_end(args);

        // if log level = fatal or assert, exit
        if(log_level == ll_fatal || log_level == ll_assert) {
            exit(EXIT_FAILURE);
        }
    }
}


