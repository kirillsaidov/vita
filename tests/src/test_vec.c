#include <assert.h>

#include "../../inc/vita/container/vec.h"

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

        vec_apply(v, func);
        assert(vec_getd(v, 1) == 0.5);

        vec_pushd(v, 3.125);
        assert(vec_getd(v, vec_len(v)-1) == 3.125);
        assert(vec_len(v) == 11);
        assert(vec_capacity(v) == 30);
        assert(vec_has_space(v) == 19);

        vec_setd(v, 24.5, 5);
        assert(vec_getd(v, 5) == 24.5);

        vec_reserve(v, 32);
        assert(vec_len(v) == 11);
        assert(vec_capacity(v) == 62);
        assert(vec_has_space(v) == 51);

        double dt = 134.431;
        vec_insert(v, &dt, 5);
        assert(vec_getd(v, 5) == dt);
        assert(vec_len(v) == 12);
        assert(vec_capacity(v) == 62);
        assert(vec_has_space(v) == 50);
        assert(vec_contains(v, &dt) == 5);

        vec_t *vcopy = vec_dup(v); {
            assert(vec_getd(vcopy, 5) == dt);

            vec_remove(vcopy, vec_contains(v, &dt), rs_fast);
            assert(vec_getd(vcopy, 5) == 3.125);
        } vec_destroy(vcopy);

        vec_remove(v, vec_contains(v, &dt), vt_rs_stable);
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
        assert(vec_capacity(v) == 3);
        assert(vec_has_space(v) == 1);

        vec_pushd(v, 4);
        assert(vec_getd(v, vec_len(v)-1) == 4);
        assert(vec_len(v) == 3);
        assert(vec_capacity(v) == 3);
        assert(vec_has_space(v) == 0);

        double value = *(double*)vec_pop_get(v);
        assert(value == 4);
        assert(vec_len(v) == 2);
        assert(vec_capacity(v) == 3);
        assert(vec_has_space(v) == 1);
    } vec_destroy(v);
    
    size_t w = 5, h = 5;
    vec_t *vecmat = vec_create(w*h, sizeof(int32_t)); {
        // right now it acts as a list, we need to set its length = w*h
        assert(vec_len(vecmat) == 0);
        assert(vec_capacity(vecmat) == w*h);

        // set length to w*h
        vec_resize(vecmat, w*h);
        assert(vec_len(vecmat) == w*h);
        assert(vec_capacity(vecmat) == w*h);
        assert(vec_has_space(vecmat) == 0);

        vec_seti32(vecmat, 1, index_2d_to_1d(0, 0, w));
        vec_seti32(vecmat, 1, index_2d_to_1d(1, 1, w));
        vec_seti32(vecmat, 1, index_2d_to_1d(2, 2, w));
        vec_seti32(vecmat, 1, index_2d_to_1d(3, 3, w));
        vec_seti32(vecmat, 1, index_2d_to_1d(4, 4, w));
        
        assert(vec_geti32(vecmat, 0) == 1);
        assert(vec_geti32(vecmat, 6) == 1);
        assert(vec_geti32(vecmat, 12) == 1);
        assert(vec_geti32(vecmat, 18) == 1);
        assert(vec_geti32(vecmat, 24) == 1);
    } vec_destroy(vecmat);

    DEBUG_DEFAULT_QUIT();
    return 0;
}
