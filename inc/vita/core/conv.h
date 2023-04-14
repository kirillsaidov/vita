#ifndef VITA_CONV_H
#define VITA_CONV_H

/** VITA_CONV MODULE
    - vt_conv_str_to_int64
    - vt_conv_str_to_uint64
    - vt_conv_str_to_double
    - vt_conv_str_to_real
    - vt_conv_int64_to_str
    - vt_conv_uint64_to_str
    - vt_conv_double_to_str
    - vt_conv_real_to_str
*/

#include "../core/core.h"
#include "../container/str.h"

#define VT_MAX_PRECISION 48

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

/** Convert to str
    @param buf C string
    @param buf_len buffer length
    @param val value to convert
*/
#define VT_PROTOTYPE_CONV_TO_STR(T, t) extern void vt_conv_##t##_to_str(char *const buf, const size_t buf_len, const T val)
VT_PROTOTYPE_CONV_TO_STR(int64_t, int64);
VT_PROTOTYPE_CONV_TO_STR(uint64_t, uint64);
VT_PROTOTYPE_CONV_TO_STR(double, double);
VT_PROTOTYPE_CONV_TO_STR(real, real);
#undef VT_PROTOTYPE_CONV_TO_STR

#endif // VITA_CONV_H
