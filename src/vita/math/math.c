#include "vita/math/math.h"

float isqrt(const float x) {
    float x2 = x * 0.5f;
    float y = x;
    long i = *(long*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y *= 1.5f - x2*y*y;
    y *= 1.5f - x2*y*y;
    y *= 1.5f - x2*y*y;

    return y;
}









