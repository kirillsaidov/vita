#ifndef VITA_PATH_H
#define VITA_PATH_H

/** VITA_PATH MODULE
	- path_build
	- path_build_n
	- path_getcwd
	- path_exists
	- path_fexists
	- path_listdir
*/

#include "../core/core.h"
#include "../container/str.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <direct.h>
	#define PATH_SEPARATOR "\\\\"
	#define getcwd _getcwd
#else
	#include <unistd.h>
	#include <dirent.h>
	#define PATH_SEPARATOR "/"
#endif

#include <sys/types.h>
#include <sys/stat.h>

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
extern str_t *path_build_n(const plist_t *const p);

/**
Get current working directory

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_getcwd();

/**
Checks if directory exists

Params:
	cs = path

Returns: `true` if directory exists
*/
extern bool path_exists(const char *const cs);

/**
Checks if file exists

Params:
	cs = path

Returns: `true` if file exists
*/
extern bool path_fexists(const char *const cs);

/**
Get all directory contents

Params:
	cs = path

Returns: `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir(const char *const cs);

#endif // VITA_PATH_H