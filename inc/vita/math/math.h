#ifndef VITA_MATH_MATH_H
#define VITA_MATH_MATH_H

/** MATH MODULE
    - vt_math_is_palindrome
    - vt_math_map_to
    - vt_math_sqrt_carmack
    - vt_math_isqrt_carmack
    - vt_math_random_u64
    - vt_math_random_f32
    - vt_math_random_f32_uniform
*/

#include <math.h>
#include "../core/core.h"

/** Checks if a natural number is a palindrome
    @param num input number
    @returns `true` if num is palindrome
*/
extern bool vt_math_is_palindrome(const uint64_t num);

/** Linear mapping: x in [xMin, xMax] -> y in [outMin, outMax]
    @param x value
    @param xMin min bound
    @param xMax max bound
    @param outMin out min bound
    @param outMax out max bound

    @returns double value mapped between [outMin, outMax]
*/
extern double vt_math_map_to(const double x, const double xMin, const double xMax, const double outMin, const double outMax);

/** Carmack fast sqrt
    @param v value
    @returns random float value
*/
extern float vt_math_sqrt_carmack(float v);

/** Carmack fast inverse sqrt
    @param v value
    @returns random float value
*/
extern float vt_math_isqrt_carmack(float v);

/** Behaves like rand() library function producing random number in [0, uint64_t.max - 1) range
    @returns  uint64_t
*/
extern uint64_t vt_math_random_u64(void);

/** Generates a random float value in range [0; ubound)
    @param ubound upper bound
    @returns random float value
*/
extern float vt_math_random_f32(const uint32_t ubound);

/** Generates a random float value in range [lbound; ubound); approximately 'uniformely distributed' 
    @param lbound lower bound
    @param ubound upper bound

    @returns random float value
*/
extern float vt_math_random_f32_uniform(const float lbound, const float ubound);

/** Generates a random float value from normal distribution
    @param mu mean
    @param std sigma

    @returns random float value
*/
extern float vt_math_random_f32_normal(const float mu, const float std);

#endif // VITA_MATH_H
