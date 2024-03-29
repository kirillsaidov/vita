#include <assert.h>

#include "vita/container/vec.h"

void func(void *ptr, size_t i) {
    *(double*)ptr = i/2.0;
}

int main(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    vt_vec_t *v = vt_vec_create(10, sizeof(double), alloctr); {
        assert(vt_vec_len(v) == 0);
        assert(vt_vec_capacity(v) == 10);
        assert(vt_vec_has_space(v) == 10);
        assert(vt_vec_is_empty(v));

        vt_vec_resize(v, 10);
        assert(vt_vec_len(v) == 10);
        assert(vt_vec_capacity(v) == 10);
        assert(vt_vec_has_space(v) == 0);
        assert(!vt_vec_is_empty(v));

        vt_vec_apply(v, func);
        assert(vt_vec_getd(v, 1) == 0.5);

        vt_vec_push_backd(v, 3.125);
        assert(vt_vec_getd(v, vt_vec_len(v)-1) == 3.125);
        assert(vt_vec_len(v) == 11);
        assert(vt_vec_capacity(v) == 30);
        assert(vt_vec_has_space(v) == 19);

        vt_vec_setd(v, 24.5, 5);
        assert(vt_vec_getd(v, 5) == 24.5);

        vt_vec_reserve(v, 32);
        assert(vt_vec_len(v) == 11);
        assert(vt_vec_capacity(v) == 62);
        assert(vt_vec_has_space(v) == 51);

        double dt = 134.431;
        vt_vec_insert(v, &dt, 5);
        assert(vt_vec_getd(v, 5) == dt);
        assert(vt_vec_len(v) == 12);
        assert(vt_vec_capacity(v) == 62);
        assert(vt_vec_has_space(v) == 50);
        assert(vt_vec_can_find(v, &dt) == 5);

        vt_vec_t *vcopy = vt_vec_dup(v, alloctr); {
            assert(vt_vec_getd(vcopy, 5) == dt);
            vt_vec_remove(vcopy, vt_vec_can_find(v, &dt), VT_REMOVE_STRATEGY_FAST);            
            assert(vt_vec_getd(vcopy, 5) == 3.125);
        } vt_vec_destroy(vcopy);

        vt_vec_remove(v, vt_vec_can_find(v, &dt), VT_REMOVE_STRATEGY_STABLE);
        assert(vt_vec_getd(v, 5) == 24.5);

        vt_vec_shrink(v);
        assert(vt_vec_len(v) == 11);
        assert(vt_vec_capacity(v) == 11);
        assert(vt_vec_has_space(v) == 0);

        vt_vec_clear(v);
        assert(vt_vec_len(v) == 0);
        assert(vt_vec_capacity(v) == 11);
        assert(vt_vec_has_space(v) == 11);

        vt_vec_resize(v, 5);
        assert(vt_vec_len(v) == 5);
        assert(vt_vec_capacity(v) == 5);
        assert(vt_vec_has_space(v) == 0);

        vt_vec_resize(v, 1);
        assert(vt_vec_len(v) == 1);
        assert(vt_vec_capacity(v) == 1);
        assert(vt_vec_has_space(v) == 0);

        vt_vec_push_backd(v, 3.125);
        assert(vt_vec_getd(v, vt_vec_len(v)-1) == 3.125);
        assert(vt_vec_len(v) == 2);
        assert(vt_vec_capacity(v) == 3);
        assert(vt_vec_has_space(v) == 1);

        vt_vec_push_backd(v, 4);
        assert(vt_vec_getd(v, vt_vec_len(v)-1) == 4);
        assert(vt_vec_len(v) == 3);
        assert(vt_vec_capacity(v) == 3);
        assert(vt_vec_has_space(v) == 0);

        double value = *(double*)vt_vec_pop_get(v);
        assert(value == 4);
        assert(vt_vec_len(v) == 2);
        assert(vt_vec_capacity(v) == 3);
        assert(vt_vec_has_space(v) == 1);
    } vt_vec_destroy(v);
    
    size_t w = 5, h = 5;
    vt_vec_t *vecmat = vt_vec_create(w*h, sizeof(int32_t), alloctr); {
        // right now it acts as a list, we need to set its length = w*h
        assert(vt_vec_len(vecmat) == 0);
        assert(vt_vec_capacity(vecmat) == w*h);

        // set length to w*h
        vt_vec_resize(vecmat, w*h);
        assert(vt_vec_len(vecmat) == w*h);
        assert(vt_vec_capacity(vecmat) == w*h);
        assert(vt_vec_has_space(vecmat) == 0);

        vt_vec_seti32(vecmat, 1, vt_index_2d_to_1d(0, 0, w));
        vt_vec_seti32(vecmat, 1, vt_index_2d_to_1d(1, 1, w));
        vt_vec_seti32(vecmat, 1, vt_index_2d_to_1d(2, 2, w));
        vt_vec_seti32(vecmat, 1, vt_index_2d_to_1d(3, 3, w));
        vt_vec_seti32(vecmat, 1, vt_index_2d_to_1d(4, 4, w));
        
        assert(vt_vec_geti32(vecmat, 0) == 1);
        assert(vt_vec_geti32(vecmat, 6) == 1);
        assert(vt_vec_geti32(vecmat, 12) == 1);
        assert(vt_vec_geti32(vecmat, 18) == 1);
        assert(vt_vec_geti32(vecmat, 24) == 1);

        void *i = NULL;
        while ((i = vt_array_slide_front(vecmat)) != NULL) {
            // printf("%d\n", *VT_AS(int32_t*, i));
        }
    } vt_vec_destroy(vecmat);

    // testing element removal
    vt_vec_t *myvec = vt_vec_create(7, sizeof(int32_t), alloctr);
    {
        vt_vec_push_backi32(myvec, 1);
        vt_vec_push_backi32(myvec, 2);
        vt_vec_push_backi32(myvec, 3);
        vt_vec_push_backi32(myvec, 4);
        vt_vec_push_backi32(myvec, 5);
        vt_vec_push_backi32(myvec, 6);
        vt_vec_push_backi32(myvec, 7);
        assert(vt_vec_len(myvec) == 7);
        assert(vt_vec_geti32(myvec, 0) == 1);

        vt_vec_remove(myvec, 0, VT_REMOVE_STRATEGY_STABLE);
        assert(vt_vec_geti32(myvec, 0) == 2);
        assert(vt_vec_len(myvec) == 6);

        vt_vec_remove(myvec, 0, VT_REMOVE_STRATEGY_FAST);
        assert(vt_vec_geti32(myvec, 0) == 7);
        assert(vt_vec_len(myvec) == 5);

        vt_vec_remove(myvec, vt_vec_len(myvec) - 1, VT_REMOVE_STRATEGY_FAST);
        assert(vt_vec_geti32(myvec, vt_vec_len(myvec) - 1) == 5);
        assert(vt_vec_len(myvec) == 4);

        vt_vec_remove(myvec, vt_vec_len(myvec) - 1, VT_REMOVE_STRATEGY_STABLE);
        assert(vt_vec_geti32(myvec, vt_vec_len(myvec) - 1) == 4);
        assert(vt_vec_len(myvec) == 3);

        vt_vec_resize(myvec, 1);
        assert(vt_vec_geti32(myvec, 0) == 7);
        assert(vt_vec_len(myvec) == 1);

        vt_vec_remove(myvec, 0, VT_REMOVE_STRATEGY_STABLE);
        assert(vt_vec_len(myvec) == 0);
    }
    vt_vec_destroy(myvec);

    vt_mallocator_destroy(alloctr);
    return 0;
}
