#ifndef VITA_MATH_H
#define VITA_MATH_H

/** VITA_MATH MODULE
    - 
*/

#include <math.h>
#include "../core/core.h"

/**
Checks if a natural number is a palindrome

Params:
    num = input number

Returns: `true` if num is palindrome
*/
extern bool is_palindrome(const uint64_t num);

/**
Linear mapping: x in [xMin, xMax] -> y in [outMin, outMax]

Params:
    x = value
    xMin = min bound
    xMax = max bound
    outMin = out min bound
    outMax = out max bound

Returns: double value mapped between [outMin, outMax]
*/
extern double map_to(const double x, const double xMin, const double xMax, const double outMin, const double outMax);

/**
Behaves like rand() library function producing random number in [0, uint64_t.max) range

Returns: uint64_t
*/
extern uint64_t random_u64(void);

/**
Carmack fast sqrt

Params:
    v = value

Returns: float
*/
extern float sqrt_c(float v);

/**
Carmack fast inverse sqrt

Params:
    v = value

Returns: float
*/
extern float isqrt_c(float v);

/**
Generates a random float value in range [0; ubound)

Params:
    ubound = upper bound

Returns: float
*/
extern float random_f32(const uint32_t ubound);

/**
Generates a random float value in range [lbound; ubound); approximately 'uniformely distributed' 

Params:
    lbound = lower bound
    ubound = upper bound

Returns: float
*/
extern float random_f32_uniform(float lbound, float ubound);

#endif // VITA_MATH_H
