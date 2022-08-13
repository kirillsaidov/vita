#ifndef VITA_ARGOPT_H
#define VITA_ARGOPT_H

/** VITA_ARGOPT MODULE
    - argopt_parse
    - argopt_print_help
*/

#include <stdarg.h>

#include "../container/str.h"
#include "../container/plist.h"
#include "../algorithm/comparison.h"

#define OPT(v) ((void**)&v)

typedef struct ArgOpt {
    const char *const optionLong;
    const char *const optionShort;
    const char *const optionDesc;
    
    void **optionValue;
    const enum DataType optionType;
} argopt_t;

/**
Parses command line arguments

Params:
    argc = number of arguments
    argv = command line arguments
    optc = number of options
    optv = an array of argument options

Returns: `true` upon sucess, `false` otherwise
*/
extern bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv);

/**
Prints help message (the usage manual)

Params:
    header = msg before the usage  manual
    footer = msg after the usage manual
    optc = number of options
    optv = an array of argument options
*/
extern void argopt_print_help(const char *header, const char *footer, const size_t optc, const argopt_t *const optv);

#endif // VITA_ARGOPT_H
