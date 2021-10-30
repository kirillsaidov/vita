#include <assert.h>

#include "../../include/vita/container/vec.h"

void func(void *ptr, size_t i) {
	*(double*)ptr = i/2.0;
}

int main(void) { 
	vec_t *v = vec_create(10, sizeof(double)); {
		assert(vec_len(v) == 0);
		assert(vec_capacity(v) == 10);
		assert(vec_has_space(v) == 10);
		assert(vec_is_empty(v));

		vec_resize(v, 10);
		assert(vec_len(v) == 10);
		assert(vec_capacity(v) == 10);
		assert(vec_has_space(v) == 0);
		assert(!vec_is_empty(v));

		vec_foreach(v, func);
		assert(vec_getd(v, 1) == 0.5);

		vec_pushd(v, 3.125);
		assert(vec_getd(v, vec_len(v)-1) == 3.125);
		assert(vec_len(v) == 11);
		assert(vec_capacity(v) == 14);
		assert(vec_has_space(v) == 3);

		vec_setd(v, 24.5, 5);
		assert(vec_getd(v, 5) == 24.5);

		vec_reserve(v, 32);
		assert(vec_len(v) == 11);
		assert(vec_capacity(v) == 46);
		assert(vec_has_space(v) == 35);

		double dt = 134.431;
		vec_insert(v, &dt, 5);
		assert(vec_getd(v, 5) == dt);
		assert(vec_len(v) == 12);
		assert(vec_capacity(v) == 46);
		assert(vec_has_space(v) == 34);
		assert(vec_contains(v, &dt) == 5);

		vec_t *vcopy = vec_dup(v); {
			assert(vec_getd(vcopy, 5) == dt);

			vec_remove(vcopy, vec_contains(v, &dt), rs_fast);
			assert(vec_getd(vcopy, 5) == 3.125);
		} vec_destroy(vcopy);

		vec_remove(v, vec_contains(v, &dt), rs_stable);
		assert(vec_getd(v, 5) == 24.5);

		vec_shrink(v);
		assert(vec_len(v) == 11);
		assert(vec_capacity(v) == 11);
		assert(vec_has_space(v) == 0);

		vec_clear(v);
		assert(vec_len(v) == 0);
		assert(vec_capacity(v) == 11);
		assert(vec_has_space(v) == 11);

		vec_resize(v, 5);
		assert(vec_len(v) == 5);
		assert(vec_capacity(v) == 5);
		assert(vec_has_space(v) == 0);

		vec_resize(v, 1);
		assert(vec_len(v) == 1);
		assert(vec_capacity(v) == 1);
		assert(vec_has_space(v) == 0);

		vec_pushd(v, 3.125);
		assert(vec_getd(v, vec_len(v)-1) == 3.125);
		assert(vec_len(v) == 2);
		assert(vec_capacity(v) == 2);
		assert(vec_has_space(v) == 0);

		vec_pushd(v, 4);
		assert(vec_getd(v, vec_len(v)-1) == 4);
		assert(vec_len(v) == 3);
		assert(vec_capacity(v) == 3);
		assert(vec_has_space(v) == 0);
	} vec_destroy(v);

	return 0;
}
































