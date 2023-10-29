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
    static time_t random_time = 0;
    if (!random_func_is_init) {
        random_time = time(NULL);
        random_func_is_init = 1;
    }

    random_time = (0x343FDULL * random_time + 0x269EC3ULL) % 0xFFFFFFFFULL;
    return (random_time & 0x3FFF8000) >> 15;
}

float vt_math_random_f32(const uint32_t bound) {
    const float randval = sin(vt_math_random_u64());
    return bound * fabs(randval);
}

float vt_math_random_f32_uniform(const float lbound, const float ubound) {
    const float randval = sin(vt_math_random_u64() * vt_math_random_u64());
    return lbound + (ubound - lbound) * fabs(randval);
}

float vt_math_random_f32_normal(const float mu, const float std) {
    float U1, U2, W, mult;
    static float X1, X2;
    static bool call = 0;

    if (call == 1) {
        call = !call;
        return (mu + std * X2);
    }

    do {
        U1 = -1 + (vt_math_random_f32(RAND_MAX) / RAND_MAX) * 2;
        U2 = -1 + (vt_math_random_f32(RAND_MAX) / RAND_MAX) * 2;
        W = pow(U1, 2) + pow(U2, 2);
    }
    while (W >= 1 || W == 0);

    mult = sqrt((-2 * log(W)) / W);
    X1 = U1 * mult;
    X2 = U2 * mult;

    call = !call;

    return (mu + std * X1);
}

bool vt_math_is_close(const float lhs, const float rhs, const float rtol) {
    return fabs(lhs - rhs) < fabs(rtol);
}

int64_t vt_math_gcd(const int64_t a, const int64_t b) {
    int64_t rem, low_val, high_val;
    low_val = a >= b ? b : a;
    high_val = a >= b ? a : b;

    // find gcd
    do {
        rem = high_val % low_val;
        high_val = low_val;
        low_val = rem;
    } while (rem != 0);

    return high_val;
}

int64_t vt_math_lcm(int64_t a, int64_t b) {
    return a / vt_math_gcd(a, b) * b;
}

