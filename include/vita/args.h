#ifndef VITA_ARGS_H
#define VITA_ARGS_H

/** VITA_GETOPT MODULE
	- getopt
*/

#include <stdarg.h>

#include "core/core.h"

/**
Parses command line arguments

Params:

Returns: 
*/
void args_parse(const int argc, const char **const argv, ...);

#endif // VITA_ARGS_H