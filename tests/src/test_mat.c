#include <assert.h>

#include "../../inc/vita/container/mat.h"

void func(void *ptr, size_t i, size_t j) {
	*(double*)ptr = i + j;
}

void print(void *ptr, size_t i, size_t j) {
	printf("%.2f\n", *(double*)ptr);
}

int main(void) { 
	mat_t *m = mat_create(15, 15, sizeof(double)); {
		assert(mat_rows(m) == 15 && mat_cols(m) == 15);

		mat_resize(m, 10, 10);
		assert(mat_rows(m) == 10 && mat_cols(m) == 10);

		mat_foreach(m, func);
		assert(mat_getd(m, 1, 1) == 2);

		mat_setd(m, 24.5, 5, 5);
		assert(mat_getd(m, 5, 5) == 24.5);

		mat_t *mcopy = mat_dup(m); {
			assert(mat_getd(mcopy, 5, 5) == 24.5);
		} mat_destroy(mcopy);

		mat_clear(m);
		assert(mat_rows(m) == 10 && mat_cols(m) == 10);
		assert(mat_getd(m, 5, 5) == 0);

		// mat_resize(m, 0, 0); // false: cannot do it
		mat_resize(m, 3, 3);
		mat_foreach(m, func);
		assert(mat_getd(m, 2, 2) == 4);
		// mat_foreach(m, print);
	} mat_destroy(m);

	return 0;
}
































