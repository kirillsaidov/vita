#ifndef VITA_ARGS_H
#define VITA_ARGS_H

/** VITA_ARGS MODULE
    - //
*/

#include <stdarg.h>

#include "../container/str.h"
#include "../container/plist.h"

typedef struct ArgOpt {
    const char *const optionLong;
    const char *const optionShort;
    const char *const optionDesc;
    
    void *optionValue;
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
extern bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, const argopt_t *const optv);

/**
Prints user argument options

Params:
    optc = number of options
    optv = an array of argument options
*/
extern void argopt_print(const size_t optc, const argopt_t *const optv);

#endif // VITA_ARGS_H
