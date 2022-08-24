#ifndef VITA_MATH_H
#define VITA_MATH_H

/** VITA_MATH MODULE
    - 
*/

#include "../core/core.h"

// extern float isqrt(const float x);

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
Behaves like rand() library function producing random number in [0, uint64_t.max] range

Returns: uint64_t value
*/
uint64_t get_random();

#endif // VITA_MATH_H
