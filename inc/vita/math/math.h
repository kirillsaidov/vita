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

#endif // VITA_MATH_H