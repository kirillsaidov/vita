#ifndef VITA_COMPARISON_H
#define VITA_COMPARISON_H

/** VITA_COMPARISON MODULE
 * Macros:
    - VT_CMP_MIN
    - VT_CMP_MAX
    - VT_CMP_CLAMP

 * Functions:
    - vt_cmp_minX (X = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_cmp_maxX (X = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
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
extern int8_t vt_cmp_mini8(const int8_t x, const int8_t y);
extern uint8_t vt_cmp_minu8(const uint8_t x, const uint8_t y);
extern int16_t vt_cmp_mini16(const int16_t x, const int16_t y);
extern uint16_t vt_cmp_minu16(const uint16_t x, const uint16_t y);
extern int32_t vt_cmp_mini32(const int32_t x, const int32_t y);
extern uint32_t vt_cmp_minu32(const uint32_t x, const uint32_t y);
extern int64_t vt_cmp_mini64(const int64_t x, const int64_t y);
extern uint64_t vt_cmp_minu64(const uint64_t x, const uint64_t y);
extern float vt_cmp_minf(const float x, const float y);
extern double vt_cmp_mind(const double x, const double y);
extern real vt_cmp_minr(const real x, const real y);

/** Compares 2 values and returns the maximum value
    @param x fist value
    @param y second value

    @returns the maximum value
*/
extern int8_t vt_cmp_maxi8(const int8_t x, const int8_t y);
extern uint8_t vt_cmp_maxu8(const uint8_t x, const uint8_t y);
extern int16_t vt_cmp_maxi16(const int16_t x, const int16_t y);
extern uint16_t vt_cmp_maxu16(const uint16_t x, const uint16_t y);
extern int32_t vt_cmp_maxi32(const int32_t x, const int32_t y);
extern uint32_t vt_cmp_maxu32(const uint32_t x, const uint32_t y);
extern int64_t vt_cmp_maxi64(const int64_t x, const int64_t y);
extern uint64_t vt_cmp_maxu64(const uint64_t x, const uint64_t y);
extern float vt_cmp_maxf(const float x, const float y);
extern double vt_cmp_maxd(const double x, const double y);
extern real vt_cmp_maxr(const real x, const real y);

#endif // VITA_COMPARISON_H
