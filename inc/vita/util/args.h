#ifndef VITA_ARGS_H
#define VITA_ARGS_H

/** VITA_ARGS MODULE
    - getopt
*/

#include <stdarg.h>

#include "../container/str.h"
#include "../container/plist.h"

typedef struct ArgOpt {
    plist_t *optionLong;
    plist_t *optionShort;
    plist_t *optionDesc;

    str_t *helpInfo;
    bool helpWanted;
} argopt_t;

/**
Parses command line arguments

Params:
    argc = number of arguments
    argv = arguments
    numOptions = number of command line options
    ... = option, description, variable where to save value

Returns: `argopt_t*` upon sucess, `NULL` otherwise
*/
extern argopt_t *args_parse(const int argc, const char **const argv, const size_t numOptions, ...);
extern void args_free(argopt_t *a);

#endif // VITA_ARGS_H
