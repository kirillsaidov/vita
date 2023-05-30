#include "vita/util/debug.h"

// private
static bool vt_debug_output_on = true;
static const char *vt_debug_output_filename = NULL;

void vt_debug_assert(
    const bool expr, 
    const char *const zexpr, 
    const char *const subject, 
    const bool quit_on_failure, 
    const char *const file, 
    const char *const func, 
    const size_t line, 
    const char *const zfmt, 
    ...
) {
    if(!expr) {
        // get time
        char tbuf[VT_DATETIME_BUFFER_SIZE] = {0};
        vt_datetime_get_now_as_text(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, zfmt); 
        {
            if(vt_debug_output_on) {
                // create a file
                FILE *fp = (vt_debug_output_filename == NULL) ? stderr : fopen(vt_debug_output_filename, "a");
                if(fp == NULL) { fp = stderr; }

                // logging to fp
                fprintf(fp, "%s %s [%s] %s:%s:%zu: ", tbuf, subject, zexpr, file, func, line);
                vfprintf(fp, zfmt, args);
                fprintf(fp, "\n");

                // close the file, if needed
                if(vt_debug_output_filename != NULL) { fclose(fp); }
            }
        }
        va_end(args);

        // exit upon failure
        if(quit_on_failure) { exit(EXIT_FAILURE); }
    }
}

void vt_debug_printf(const char *const subject, const char *const zfmt, ...) {
    // check for invalid input
    assert(zfmt != NULL);

    // get time
    char tbuf[VT_DATETIME_BUFFER_SIZE] = {0};
    vt_datetime_get_now_as_text(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
    
    // getting arguments
    va_list args; va_start(args, zfmt); 
    {
        if(vt_debug_output_on) {
            // create a file
            FILE *fp = (vt_debug_output_filename == NULL) ? stderr : fopen(vt_debug_output_filename, "a");
            if(fp == NULL) { fp = stderr; }

            // logging to stderr
            fprintf(fp, "%s %s ", tbuf, subject);
            vfprintf(fp, zfmt, args);

            // close the file, if needed
            if(vt_debug_output_filename != NULL) { fclose(fp); }
        }
    }
    va_end(args);
}

void vt_debug_redirect_output(const char *const zfilename) {
    vt_debug_output_filename = zfilename;
}

void vt_debug_disable_output(const bool off) {
    vt_debug_output_on = !off;
}




