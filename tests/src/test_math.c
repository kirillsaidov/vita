#include <stdio.h>
#include <assert.h>

#include "../../inc/vita/math/math.h"

int main(void) {
	assert((int)(vt_math_isqrt_carmack(9)) == 3); // fails: isqrt does not work (have no idea why)
	assert(vt_math_is_palindrome(9009));

	return 0;
}
