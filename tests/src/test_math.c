#include <stdio.h>
#include <assert.h>

#include "../../inc/vita/math/math.h"

int main(void) {
    assert(vt_math_is_palindrome(9009));
    VT_FOREACH(i, 0, 10) {
        printf("%f\n", vt_math_random_f32(1));
        // printf("%llu\n", vt_math_random_u64());
    }
    
    return 0;
}
