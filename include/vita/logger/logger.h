#ifndef VITA_LOGGER_H
#define VITA_LOGGER_H

/** VITA_LOGGER MODULE (logging to stdout, stderr)
    - logger_info
    - logger_warn
    - logger_error
    - logger_fatal
*/

// std includes
#include <stdio.h>
#include <stdlib.h>

// vita imports
#include "../string/str.h"

/** logger_info ==> prints info msg to stdout
    params:
        const char* msg
*/
extern void logger_info(const str_t msg);

/** logger_warn ==> prints info msg and tag to stdout
    params:
        const str_t msg
        const str_t tag
*/
extern void logger_warn(const str_t msg, const str_t tag);

/** logger_error ==> prints info msg and tag to stderr
    params:
        const str_t msg
        const str_t tag
*/
extern void logger_error(const str_t msg, const str_t tag);

/** logger_fatal ==> prints info msg and tag to stderr and exits the program
    params:
        const str_t msg
        const str_t tag
*/
extern void logger_fatal(const str_t msg, const str_t tag);


#endif // VITA_LOGGER_H


















