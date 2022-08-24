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

double map_to(const double x, const double xMin, const double xMax, const double outMin, const double outMax) {
    return (x - xMin) * (outMax - outMin) / (xMax - xMin) + outMin;
}

uint64_t get_random() {
    static int8_t random_func_is_init = 0;
    static uint64_t random_func_x = 0;
    if (!random_func_is_init) {
        random_func_x = time(NULL);
        random_func_is_init = 1;
    }

    random_func_x = (0x343FDULL * random_func_x + 0x269EC3ULL) % 0xFFFFFFFFULL;
    return (random_func_x & 0x3FFF8000) >> 15;
}
