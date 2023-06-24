#ifndef VITA_ALGORITHM_COMPARISON_H
#define VITA_ALGORITHM_COMPARISON_H

/** COMPARISON MODULE
    - vt_cmp_minT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_cmp_maxT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_cmp_clampT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
*/

#include "../core/core.h"

/** Compares 2 values and returns the minimum value
    @param x fist value
    @param y second value

    @returns the minimum value
*/
#define VT_PROTOTYPE_CMP_MIN(T, t) extern T vt_cmp_min##t(const T x, const T y);
VT_PROTOTYPE_CMP_MIN(int8_t, i8)
VT_PROTOTYPE_CMP_MIN(uint8_t, u8)
VT_PROTOTYPE_CMP_MIN(int16_t, i16)
VT_PROTOTYPE_CMP_MIN(uint16_t, u16)
VT_PROTOTYPE_CMP_MIN(int32_t, i32)
VT_PROTOTYPE_CMP_MIN(uint32_t, u32)
VT_PROTOTYPE_CMP_MIN(int64_t, i64)
VT_PROTOTYPE_CMP_MIN(uint64_t, u64)
VT_PROTOTYPE_CMP_MIN(float, f)
VT_PROTOTYPE_CMP_MIN(double, d)
VT_PROTOTYPE_CMP_MIN(real, r)
#undef VT_PROTOTYPE_CMP_MIN

/** Compares 2 values and returns the maximum value
    @param x fist value
    @param y second value

    @returns the maximum value
*/
#define VT_PROTOTYPE_CMP_MAX(T, t) extern T vt_cmp_max##t(const T x, const T y);
VT_PROTOTYPE_CMP_MAX(int8_t, i8)
VT_PROTOTYPE_CMP_MAX(uint8_t, u8)
VT_PROTOTYPE_CMP_MAX(int16_t, i16)
VT_PROTOTYPE_CMP_MAX(uint16_t, u16)
VT_PROTOTYPE_CMP_MAX(int32_t, i32)
VT_PROTOTYPE_CMP_MAX(uint32_t, u32)
VT_PROTOTYPE_CMP_MAX(int64_t, i64)
VT_PROTOTYPE_CMP_MAX(uint64_t, u64)
VT_PROTOTYPE_CMP_MAX(float, f)
VT_PROTOTYPE_CMP_MAX(double, d)
VT_PROTOTYPE_CMP_MAX(real, r)
#undef VT_PROTOTYPE_CMP_MAX

/** Compares 2 values and returns the minimum value
    @param x value to clamp
    @param l low boundary
    @param h high boundary

    @returns If `x < l`, returns `l`; otherwise if `h < x`, returns h; otherwise returns `x`
*/
#define VT_PROTOTYPE_CMP_CLAMP(T, t) extern T vt_cmp_clamp##t(const T x, const T l, const T h);
VT_PROTOTYPE_CMP_CLAMP(int8_t, i8)
VT_PROTOTYPE_CMP_CLAMP(uint8_t, u8)
VT_PROTOTYPE_CMP_CLAMP(int16_t, i16)
VT_PROTOTYPE_CMP_CLAMP(uint16_t, u16)
VT_PROTOTYPE_CMP_CLAMP(int32_t, i32)
VT_PROTOTYPE_CMP_CLAMP(uint32_t, u32)
VT_PROTOTYPE_CMP_CLAMP(int64_t, i64)
VT_PROTOTYPE_CMP_CLAMP(uint64_t, u64)
VT_PROTOTYPE_CMP_CLAMP(float, f)
VT_PROTOTYPE_CMP_CLAMP(double, d)
VT_PROTOTYPE_CMP_CLAMP(real, r)
#undef VT_PROTOTYPE_CMP_CLAMP

#endif // VITA_ALGORITHM_COMPARISON_H
