#ifndef VITA_ARGS_H
#define VITA_ARGS_H

/** VITA_GETOPT MODULE
	- getopt
*/

#include <stdarg.h>

#include "container/str.h"
#include "container/plist.h"

typedef struct ArgOpt {
	plist_t *cmdLong;
	plist_t *cmdShort;
	plist_t *cmdDesc;

	str_t *helpManual;
	bool helpWanted;
} argopt_t;

/**
Parses command line arguments

Params:

Returns: 
*/
extern argopt_t *args_parse(const int argc, const char **const argv, const size_t numOptions, ...);
extern void args_free(argopt_t *a);

#endif // VITA_ARGS_H