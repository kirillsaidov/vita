#ifndef VITA_FLOGGER_H
#define VITA_FLOGGER_H

/** VITA_FLOGGER MODULE (logging to stdout, stderr)
    - flogger_open
    - flogger_close
    - flogger_close_all
    - flogger_erase
    - flogger_erase_all

    - logger_info
    - logger_warn
    - logger_error
    - logger_fatal
*/

#include <stdio.h>
#include <stdlib.h>

#include "../string/str.h"

// file logger struct
typedef struct FLogger* flogger_t;

/** logger_info ==> opens a new file logger for appending, if file doesn't exist, creates a new one
    params:
        const str_t flname
        const char* msg
*/
extern const flogger_t flogger_open(const str_t flname);
extern void flogger_close(const flogger_t fl);
extern void flogger_close_all();
extern void flogger_erase(const flogger_t fl);
extern void flogger_erase_all();

/** logger_info ==> prints info msg to file
    params:
        const flogger_t fl
        const char* msg
*/
extern void flogger_info(const flogger_t fl, const str_t msg);

/** logger_warn ==> prints info msg and tag to file
    params:
        const flogger_t fl
        const str_t msg
        const str_t tag
*/
extern void flogger_warn(const flogger_t fl, const str_t msg, const str_t tag);

/** logger_error ==> prints info msg and tag to file
    params:
        const flogger_t fl
        const str_t msg
        const str_t tag
*/
extern void flogger_error(const flogger_t fl, const str_t msg, const str_t tag);

/** logger_fatal ==> prints info msg and tag to file and exits the program
    params:
        const flogger_t fl
        const str_t msg
        const str_t tag
*/
extern void flogger_fatal(const flogger_t fl, const str_t msg, const str_t tag);


#endif // VITA_FLOGGER_H


















