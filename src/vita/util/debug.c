#include "vita/util/debug.h"

void debug_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...) {
    if(!expr) {
        // get time
        char tbuf[21];
        get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, fmt); 
        {
            // logging to stderr
            fprintf(stderr, "%s %5s %s:%d: ", tbuf, "DEBUG ASSERT", file, line);
            vfprintf(stderr, fmt, args);
            fprintf(stderr, "\n");
        }
        va_end(args);

        // if log level = fatal or assert, exit
        exit(0);
    }
}

