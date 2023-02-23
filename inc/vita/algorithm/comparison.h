#ifndef VITA_COMPARISON_H
#define VITA_COMPARISON_H

/** VITA_COMPARISON MODULE
 * Macros:
    - VT_CMP_MIN
    - VT_CMP_MAX
    - VT_CMP_CLAMP

 * Functions:
    - vt_cmp_minT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_cmp_maxT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
*/

#include "../core/core.h"

/** Compares 2 values and returns the minimum value
    @param x fist value
    @param y second value

    @returns the minimum value
*/
#define VT_CMP_MIN(x, y) _Generic((x),              \
                          int8_t: vt_cmp_mini8(x, y),    \
                         uint8_t: vt_cmp_minu8(x, y),    \
                         int16_t: vt_cmp_mini16(x, y),   \
                        uint16_t: vt_cmp_minu16(x, y),   \
                         int32_t: vt_cmp_mini32(x, y),   \
                        uint32_t: vt_cmp_minu32(x, y),   \
                         int64_t: vt_cmp_mini64(x, y),   \
                        uint64_t: vt_cmp_minu64(x, y),   \
                           float: vt_cmp_minf(x, y),     \
                          double: vt_cmp_mind(x, y),     \
                            real: vt_cmp_minr(x, y),     \
                         default: vt_cmp_mini64(x, y))

/** Compares 2 values and returns the maximum value
    @param x fist value
    @param y second value

    @returns the maximum value
*/
#define VT_CMP_MAX(x, y) _Generic((x),              \
                          int8_t: vt_cmp_maxi8(x, y),    \
                         uint8_t: vt_cmp_maxu8(x, y),    \
                         int16_t: vt_cmp_maxi16(x, y),   \
                        uint16_t: vt_cmp_maxu16(x, y),   \
                         int32_t: vt_cmp_maxi32(x, y),   \
                        uint32_t: vt_cmp_maxu32(x, y),   \
                         int64_t: vt_cmp_maxi64(x, y),   \
                        uint64_t: vt_cmp_maxu64(x, y),   \
                           float: vt_cmp_maxf(x, y),     \
                          double: vt_cmp_maxd(x, y),     \
                            real: vt_cmp_maxr(x, y),     \
                         default: vt_cmp_maxi64(x, y))

#define VT_CMP_CLAMP(x, l, h) (VT_CMP_MAX(l, VT_CMP_MIN(x, h)))

/** Compares 2 values and returns the minimum value
    @param x fist value
    @param y second value

    @returns the minimum value
*/
#define VT_PROTOTYPE_CMP_MIN(T, t) extern T vt_cmp_min##t(const T x, const T y)
VT_PROTOTYPE_CMP_MIN(int8_t, i8);
VT_PROTOTYPE_CMP_MIN(uint8_t, u8);
VT_PROTOTYPE_CMP_MIN(int16_t, i16);
VT_PROTOTYPE_CMP_MIN(uint16_t, u16);
VT_PROTOTYPE_CMP_MIN(int32_t, i32);
VT_PROTOTYPE_CMP_MIN(uint32_t, u32);
VT_PROTOTYPE_CMP_MIN(int64_t, i64);
VT_PROTOTYPE_CMP_MIN(uint64_t, u64);
VT_PROTOTYPE_CMP_MIN(float, f);
VT_PROTOTYPE_CMP_MIN(double, d);
VT_PROTOTYPE_CMP_MIN(real, r);
#undef VT_PROTOTYPE_CMP_MIN

/** Compares 2 values and returns the maximum value
    @param x fist value
    @param y second value

    @returns the maximum value
*/
#define VT_PROTOTYPE_CMP_MAX(T, t) extern T vt_cmp_max##t(const T x, const T y)
VT_PROTOTYPE_CMP_MAX(int8_t, i8);
VT_PROTOTYPE_CMP_MAX(uint8_t, u8);
VT_PROTOTYPE_CMP_MAX(int16_t, i16);
VT_PROTOTYPE_CMP_MAX(uint16_t, u16);
VT_PROTOTYPE_CMP_MAX(int32_t, i32);
VT_PROTOTYPE_CMP_MAX(uint32_t, u32);
VT_PROTOTYPE_CMP_MAX(int64_t, i64);
VT_PROTOTYPE_CMP_MAX(uint64_t, u64);
VT_PROTOTYPE_CMP_MAX(float, f);
VT_PROTOTYPE_CMP_MAX(double, d);
VT_PROTOTYPE_CMP_MAX(real, r);
#undef VT_PROTOTYPE_CMP_MAX

#endif // VITA_COMPARISON_H
