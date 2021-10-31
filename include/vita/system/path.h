#ifndef VITA_PATH_H
#define VITA_PATH_H

/** VITA_PATH MODULE
	- 
*/

#include "../core/core.h"
#include "../container/str.h"

#if defined(_WIN32) || defined(_WIN64)
	#define PATH_SEPARATOR "\\\\"
#else
	#define PATH_SEPARATOR "/"
#endif

/**
Builds path from raw C strings

Params:
	cs1 = raw c string
	cs2 = raw c string

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build(const char *const cs1, const char *const cs2);

/**
Builds path from raw C strings

Params:
	p = array of raw C strings

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build_n(const plist_t *p);

#endif // VITA_PATH_H