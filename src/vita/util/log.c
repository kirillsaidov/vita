#include "vita/util/log.h"

// log level strings for printing
static const char *const vt_log_level_strings[] = {
    "INFO", "WARN", "DEBUG", "ERROR", "FATAL", "ASSERT FAILURE"
};

/* ---------------- GLOBAL LOGGER BASED ON LOG LEVEL ---------------- */

// log to file
static char vt_log_filenames[vt_ll_count][PATH_MAX]; 

void vt_log_get_level(enum VitaLogLevel vt_log_level, const char *const zfilename) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_log_level < vt_ll_count, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    
    // update log filenames
    strncpy(vt_log_filenames[vt_log_level], zfilename, PATH_MAX-1);
}

void vt_log_set_level_default(const char *const zfilename) {
    for(size_t i = 0; i < vt_ll_count; i++) {
        strncpy(vt_log_filenames[i], zfilename, PATH_MAX-1);
    }
}

const char *vt_log_get_level_str(enum VitaLogLevel vt_log_level) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_log_level < vt_ll_count, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    return vt_log_level_strings[vt_log_level];
}

void vt_log(const char *const zfilename, enum VitaLogLevel vt_log_level, const bool expr, const char *const zexpr, const char *const file, const int32_t line, const char *const zfmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_log_level < vt_ll_count, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    if(!expr) {
        // get time
        char tbuf[VT_TIME_STAMP_BUFFER_SIZE] = {0};
        vt_time_get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        const char *const logger_filename = (zfilename == NULL) ? vt_log_filenames[vt_log_level] : zfilename;
        va_list args; va_start(args, zfmt); 
        {
            // if `NULL` log to `stderr`, otherwise to a file
            FILE *fp = (logger_filename[0] == '\0') ? stderr : fopen(logger_filename, "a");
            if(fp == NULL) {
                VT_DEBUG_PRINTF("%s:%i: Failed to open a file <%s>!\n", file, line, logger_filename);
                return;
            }

            // logging data
            if(zexpr == NULL) {
                fprintf(fp, "%s %5s %s:%d: ", tbuf, vt_log_get_level_str(vt_log_level), file, line);
            } else {
                fprintf(fp, "%s %5s [%s] %s:%d: ", tbuf, vt_log_get_level_str(vt_log_level), zexpr, file, line);
            }

            // output the rest
            vfprintf(fp, zfmt, args);
            fprintf(fp, "\n");

            // close file
            if(fp != stderr) { fclose(fp); }
        }
        va_end(args);

        // if log level = fatal or assert, exit
        if(vt_log_level == vt_ll_fatal || vt_log_level == vt_ll_assert) {
            exit(EXIT_FAILURE);
        }
    }
}


