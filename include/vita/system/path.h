#ifndef VITA_PATH_H
#define VITA_PATH_H

#include "../string/str.h"

/** VITA_PATH MODULE
	- path_build
*/

// path separator
const str_t pathSeparator;

/** path_build ==> builds platform specific path
*/
extern str_t path_build(const str_t first, const str_t second);

#endif // VITA_PATH_H












