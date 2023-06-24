#include "vita/util/log.h"

// log level strings for printing
static const char *const vt_log_level_strings[] = {
    "INFO", "WARN", "DEBUG", "ERROR", "FATAL", "ASSERT FAILURE"
};

/* ---------------- GLOBAL LOGGER BASED ON LOG LEVEL ---------------- */

// log to file
static char vt_log_filenames[vt_log_count][PATH_MAX]; 

void vt_log_set_level(enum VitaLogLevel vt_log_level, const char *const zfilename) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_log_level < vt_log_count, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    // update log filenames
    strncpy(vt_log_filenames[vt_log_level], zfilename, PATH_MAX-1);
}

void vt_log_set_level_all(const char *const zfilename) {
    for(size_t i = 0; i < vt_log_count; i++) {
        strncpy(vt_log_filenames[i], zfilename, PATH_MAX-1);
    }
}

const char *vt_log_get_level_str(enum VitaLogLevel vt_log_level) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_log_level < vt_log_count, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return vt_log_level_strings[vt_log_level];
}

void vt_log(const char *const zfilename, enum VitaLogLevel vt_log_level, const bool expr, const char *const zexpr, const char *const file, const size_t line, const char *const zfmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_log_level < vt_log_count, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    if(!expr) {
        // get time
        char tbuf[VT_DATETIME_BUFFER_SIZE] = {0};
        vt_datetime_get_now_as_text(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
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
                fprintf(fp, "%s %5s %s:%zu: ", tbuf, vt_log_get_level_str(vt_log_level), file, line);
            } else {
                fprintf(fp, "%s %5s [%s] %s:%zu: ", tbuf, vt_log_get_level_str(vt_log_level), zexpr, file, line);
            }

            // output the rest
            vfprintf(fp, zfmt, args);
            fprintf(fp, "\n");

            // close file
            if(fp != stderr) { fclose(fp); }
        }
        va_end(args);

        // if log level = fatal or assert, exit
        if(vt_log_level == vt_log_fatal || vt_log_level == vt_log_assert) {
            exit(EXIT_FAILURE);
        }
    }
}


