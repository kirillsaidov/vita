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
    
    return 0;
}
