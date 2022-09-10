#ifndef VITA_MATH_H
#define VITA_MATH_H

/** VITA_MATH MODULE
    - 
*/

#include <math.h>
#include "../core/core.h"

/** Checks if a natural number is a palindrome
    @param num input number
    @returns `true` if num is palindrome
*/
extern bool is_palindrome(const uint64_t num);

/** Linear mapping: x in [xMin, xMax] -> y in [outMin, outMax]
    @param x value
    @param xMin min bound
    @param xMax max bound
    @param outMin out min bound
    @param outMax out max bound

    @returns double value mapped between [outMin, outMax]
*/
extern double map_to(const double x, const double xMin, const double xMax, const double outMin, const double outMax);

/** Behaves like rand() library function producing random number in [0, uint64_t.max) range
    @returns  uint64_t
*/
extern uint64_t random_u64(void);

/** Carmack fast sqrt
    @param v value
    @returns random float value
*/
extern float sqrt_c(float v);

/** Carmack fast inverse sqrt
    @param v value
    @returns random float value
*/
extern float isqrt_c(float v);

/** Generates a random float value in range [0; ubound)
    @param ubound upper bound
    @returns random float value
*/
extern float random_f32(const uint32_t ubound);

/** Generates a random float value in range [lbound; ubound); approximately 'uniformely distributed' 
    @param lbound lower bound
    @param ubound upper bound

    @returns random float value
*/
extern float random_f32_uniform(float lbound, float ubound);

#endif // VITA_MATH_H
