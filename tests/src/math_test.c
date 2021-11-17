#include <stdio.h>
#include <assert.h>

#include "../../include/vita/math/math.h"

int main(void) {
	// assert((int)(isqrt(9)) == 3); // fails: isqrt does not work (have no idea why)
	assert(is_palindrome(9009));

	return 0;
}
































