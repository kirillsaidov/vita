#ifndef VITA_ARGOPT_H
#define VITA_ARGOPT_H

/** VITA_ARGOPT MODULE
    - argopt_parse
    - argopt_print_help
*/

#include <stdarg.h>

#include "../core/conv.h"
#include "../container/str.h"
#include "../container/plist.h"
#include "../algorithm/comparison.h"
#include "../util/debug.h"

#define ARGOPT_PARSE_SUCCESS 0
#define ARGOPT_PARSE_HELP_WANTED 1
#define ARGOPT_PARSE_ERROR -1
#define ARGOPT(v) ((void**)&v)

// Argument options data structure
typedef struct ArgOpt {
    // options info
    const char *const optionLong;
    const char *const optionShort;
    const char *const optionDesc;
    
    // value
    void **optionValue;
    const enum DataType optionType;

    // help
    bool help_wanted;
} argopt_t;

/** Parses command line arguments
    @param argc number of arguments
    @param argv command line arguments
    @param optc number of options
    @param optv an array of argument options

    @returns `1` if help wanted, `0` upon sucess and `-1` upon error

    @note
        Strings must be heap-allocated be it char* or str_t!
*/
extern int8_t argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv);

/** Prints the help manual
    @param header msg before the usage  manual
    @param footer msg after the usage manual
    @param optc number of options
    @param optv an array of argument options
*/
extern void argopt_print_help(const char *header, const char *footer, const size_t optc, const argopt_t *const optv);

#endif // VITA_ARGOPT_H
