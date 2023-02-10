#include "vita/math/math.h"

bool vt_math_is_palindrome(const uint64_t num) {
    uint64_t tempNum = num;
    uint64_t reversed = 0;

    while (tempNum > 0) {
        reversed = reversed * 10 + tempNum % 10;
        tempNum /= 10;
    }

    return (reversed == num);
}

double vt_math_map_to(const double x, const double xMin, const double xMax, const double outMin, const double outMax) {
    return (x - xMin) * (outMax - outMin) / (xMax - xMin) + outMin;
}

float vt_math_sqrt_carmack(float v) {
    union {
        float f;
        uint32_t i;
    } conv = { .f = v };

    conv.i = 0x5f375a86 - (conv.i >> 1); 
    conv.f *= 1.5F - (v * 0.5F * conv.f * conv.f); 
    conv.f *= 1.5F - (v * 0.5F * conv.f * conv.f);   
    conv.f *= 1.5F - (v * 0.5F * conv.f * conv.f); 
    
    return v*conv.f;
}

float vt_math_isqrt_carmack(float v) {
    union {
        float f;
        uint32_t i;
    } conv = { .f = v };
    
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (v * 0.5F * conv.f * conv.f);
    conv.f *= 1.5F - (v * 0.5F * conv.f * conv.f);
    conv.f *= 1.5F - (v * 0.5F * conv.f * conv.f);
    
    return conv.f;
}

uint64_t vt_math_random_u64(void) {
    static int8_t random_func_is_init = 0;
    static uint64_t random_func_x = 0;
    if (!random_func_is_init) {
        random_func_x = time(NULL);
        random_func_is_init = 1;
    }

    random_func_x = (0x343FDULL * random_func_x + 0x269EC3ULL) % 0xFFFFFFFFULL;
    return (random_func_x & 0x3FFF8000) >> 15;
}

float vt_math_random_f32(const uint32_t bound) {
    return ((float)vt_math_random_u64()/(float)UINT64_MAX) * bound;
}

float vt_math_random_f32_uniform(float lbound, float ubound) {
    float randval = sin(vt_math_random_u64() * vt_math_random_u64());
    return lbound + (ubound - lbound) * fabs(randval);
}
