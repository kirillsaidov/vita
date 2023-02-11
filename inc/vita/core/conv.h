#ifndef VITA_CONV_H
#define VITA_CONV_H

/** VITA_CONV MODULE
    - 
*/

#include "../core/core.h"
#include "../container/str.h"

#define VT_MAX_PRECISION 64

/** Convert to int64_t
    @param z C string
    @returns int64_t (0 upon failure)
*/
extern int64_t vt_conv_str_to_int64(const char *const z);

/** Convert to uint64_t
    @param z C string
    @returns uint64_t (0 upon failure)
*/
extern uint64_t vt_conv_str_to_uint64(const char *const z);

/** Convert to double
    @param z C string
    @returns double (0 upon failure)
*/
extern double vt_conv_str_to_double(const char *const z);

/** Convert to real
    @param z C string
    @returns real (0 upon failure)
*/
extern real vt_conv_str_to_real(const char *const z);


#endif // VITA_CONV_H
