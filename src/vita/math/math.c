#include "vita/math/math.h"

// DOES NOT WORK (HAVE NO IDEA WHY)
/*float isqrt(const float x) {
    float x2 = x * 0.5f;
    float y = x;
    long i = *(long*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y *= 1.5f - x2*y*y;
    y *= 1.5f - x2*y*y;
    y *= 1.5f - x2*y*y;

    return y;
}*/

bool is_palindrome(const uint64_t num) {
    uint64_t tempNum = num;
    uint64_t reversed = 0;

    while (tempNum > 0) {
        reversed = reversed * 10 + tempNum % 10;
        tempNum /= 10;
    }

    return (reversed == num);
}









