#ifndef VITA_ARGS_H
#define VITA_ARGS_H

/** VITA_ARGS MODULE
    - //
*/

#include <stdarg.h>

#include "../container/str.h"
#include "../container/plist.h"

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
    header = header msg
    optc = number of options
    optv = an array of argument options
*/
extern void argopt_print_help(const char* header, const size_t optc, const argopt_t *const optv);

/**
Prints user argument options and their values

Params:
    optc = number of options
    optv = an array of argument options
    padding = adds padding 
*/
extern void argopt_print_opts(const size_t optc, const argopt_t *const optv, int32_t padding);

#endif // VITA_ARGS_H
