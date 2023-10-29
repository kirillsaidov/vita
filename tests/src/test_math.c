#include <stdio.h>
#include <assert.h>

#include "../../inc/vita/math/math.h"

#define N_GEN 5

int main(void) {
    assert(vt_math_is_palindrome(9009));

    printf("Random dist:\n");
    VT_FOREACH(i, 0, N_GEN) {
        printf("%f\n", vt_math_random_f32(1));
        // printf("%llu\n", vt_math_random_u64());
    }

    printf("Normal dist:\n");
    VT_FOREACH(i, 0, N_GEN) {
        printf("%f\n", vt_math_random_f32_normal(0, 1));
        // printf("%llu\n", vt_math_random_u64());
    }

    printf("Uniform dist:\n");
    VT_FOREACH(i, 0, N_GEN) {
        printf("%f\n", vt_math_random_f32_uniform(0, 1));
        // printf("%llu\n", vt_math_random_u64());
    }

    printf("INT in range %% 255:\n");
    VT_FOREACH(i, 0, N_GEN) {
        printf("%zu\n", vt_math_random_u64() % 255 + 1);
        // printf("%llu\n", vt_math_random_u64());
    }

    // aprox. equal
    assert(vt_math_is_close(10, 9.9, 0.11));
    assert(vt_math_is_close(0.5, 0.49, 0.01));
    
    return 0;
}
