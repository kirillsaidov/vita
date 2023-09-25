#ifndef VITA_CORE_CONV_H
#define VITA_CORE_CONV_H

/** CONV MODULE
    - vt_conv_str_to_int8
    - vt_conv_str_to_uint8
    - vt_conv_str_to_int16
    - vt_conv_str_to_uint16
    - vt_conv_str_to_int32
    - vt_conv_str_to_uint32
    - vt_conv_str_to_int64
    - vt_conv_str_to_uint64
    - vt_conv_str_to_double
    - vt_conv_str_to_real
    - vt_conv_int8_to_str
    - vt_conv_uint8_to_str
    - vt_conv_int16_to_str
    - vt_conv_uint16_to_str
    - vt_conv_int32_to_str
    - vt_conv_uint32_to_str
    - vt_conv_int64_to_str
    - vt_conv_uint64_to_str
    - vt_conv_double_to_str
    - vt_conv_real_to_str
*/

#include <inttypes.h>
#include "../core/core.h"
#include "../container/str.h"

#define VT_MAX_PRECISION 48

// /** Convert to int64_t
//     @param z C string
//     @returns int64_t (0 upon failure)
// */
// extern int64_t vt_conv_str_to_int64(const char *const z);

// /** Convert to uint64_t
//     @param z C string
//     @returns uint64_t (0 upon failure)
// */
// extern uint64_t vt_conv_str_to_uint64(const char *const z);

// /** Convert to double
//     @param z C string
//     @returns double (0 upon failure)
// */
// extern double vt_conv_str_to_double(const char *const z);

// /** Convert to real
//     @param z C string
//     @returns real (0 upon failure)
// */
// extern real vt_conv_str_to_real(const char *const z);

/** Convert from string
    @param z C string
    @returns numerical value (or 0 upon failure)
*/
#define VT_PROTOTYPE_CONV_FROM_STR(T, t) extern T vt_conv_str_to_##t(const char *const z);
VT_PROTOTYPE_CONV_FROM_STR(int8_t, i8)
VT_PROTOTYPE_CONV_FROM_STR(uint8_t, u8)
VT_PROTOTYPE_CONV_FROM_STR(int16_t, i16)
VT_PROTOTYPE_CONV_FROM_STR(uint16_t, u16)
VT_PROTOTYPE_CONV_FROM_STR(int32_t, i32)
VT_PROTOTYPE_CONV_FROM_STR(uint32_t, u32)
VT_PROTOTYPE_CONV_FROM_STR(int64_t, i64)
VT_PROTOTYPE_CONV_FROM_STR(uint64_t, u64)
VT_PROTOTYPE_CONV_FROM_STR(float, f)
VT_PROTOTYPE_CONV_FROM_STR(double, d)
VT_PROTOTYPE_CONV_FROM_STR(real, r)
#undef VT_PROTOTYPE_CONV_FROM_STR 

/** Convert to string
    @param buf C string
    @param buf_len buffer length
    @param val value to convert
*/
#define VT_PROTOTYPE_CONV_TO_STR(T, t) extern void vt_conv_##t##_to_str(char *const buf, const size_t buf_len, const T val);
VT_PROTOTYPE_CONV_TO_STR(int8_t, i8)
VT_PROTOTYPE_CONV_TO_STR(uint8_t, u8)
VT_PROTOTYPE_CONV_TO_STR(int16_t, i16)
VT_PROTOTYPE_CONV_TO_STR(uint16_t, u16)
VT_PROTOTYPE_CONV_TO_STR(int32_t, i32)
VT_PROTOTYPE_CONV_TO_STR(uint32_t, u32)
VT_PROTOTYPE_CONV_TO_STR(int64_t, i64)
VT_PROTOTYPE_CONV_TO_STR(uint64_t, u64)
VT_PROTOTYPE_CONV_TO_STR(float, f)
VT_PROTOTYPE_CONV_TO_STR(double, d)
VT_PROTOTYPE_CONV_TO_STR(real, r)
#undef VT_PROTOTYPE_CONV_TO_STR

#endif // VITA_CORE_CONV_H
