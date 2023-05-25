#ifndef VITA_ARGOPT_H
#define VITA_ARGOPT_H

/** VITA_ARGOPT MODULE
    - vt_argopt_parse
    - vt_argopt_print_help
*/

#include <stdarg.h>

#include "../core/conv.h"
#include "../container/str.h"
#include "../container/plist.h"
#include "../algorithm/comparison.h"
#include "../util/debug.h"

#define VT_ARGOPT_PARSE_SUCCESS 0
#define VT_ARGOPT_PARSE_HELP_WANTED 1
#define VT_ARGOPT_PARSE_ERROR -1
#define VT_ARGOPT(v) ((void**)&v)

// Argument options data structure
typedef struct VitaArgOpt {
    // options info
    const char *const optionLong;
    const char *const optionShort;
    const char *const optionDesc;
    
    // value
    void **optionValue;
    const enum VitaTypeInfo optionType;
} vt_argopt_t;

/** Parses command line arguments
    @param argc number of arguments
    @param argv command line arguments
    @param optc number of options
    @param optv an array of argument options

    @returns `1` if help wanted, `0` upon sucess and `-1` upon error

    @note
        Strings must be heap-allocated be it char* or vt_str_t!
*/
extern int8_t vt_argopt_parse(const size_t argc, const char **const argv, const size_t optc, vt_argopt_t *const optv);

/** Prints the help manual
    @param header msg before the usage  manual
    @param footer msg after the usage manual
    @param optc number of options
    @param optv an array of argument options
*/
extern void vt_argopt_print_help(const char *header, const char *footer, const size_t optc, const vt_argopt_t *const optv);

#endif // VITA_ARGOPT_H
