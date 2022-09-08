#ifndef VITA_CONV_H
#define VITA_CONV_H

/** VITA_CONV MODULE
    - 
*/

#include "../core/core.h"
#include "../container/str.h"

#define MAX_PRECISION 512

/** Convert to int64_t
    @param z c string
    @returns int64_t (0 upon failure)
*/
extern int64_t conv_str2int64(const char *const z);

/** Convert to int64_t
    @param z c string
    @returns uint64_t (0 upon failure)
*/
extern uint64_t conv_str2uint64(const char *const z);

/** Convert to int64_t
    @param z c string
    @returns double (0 upon failure)
*/
extern double conv_str2double(const char *const z);

#endif // VITA_CONV_H
