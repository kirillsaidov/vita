#ifndef VITA_GETOPT_H
#define VITA_GETOPT_H

/** VITA_GETOPT MODULE
	- getopt
*/

#include <stdarg.h>

#include "core/core.h"
#include "container/str.h"

// argument options
struct Options {
    str_t *optLong;
    str_t *optShort;
    str_t *optValue;
    str_t *optDescription;
};

// argument options result containing all argument data
typedef struct GetOptResult {
    struct Options options;

    bool helpWanted;
    str_t *helpManual;
} getopt_t;

/**
Allocates memory for vec_t

Returns: `vec_t*` upon success, `NULL` otherwise
*/
getopt_t *getopt(const int argc, const char **const argv, ...);

void getopt_free(getopt_t *g);

void getopt_print(const char *const cs, const struct Options *const options);

#endif // VITA_GETOPT_H