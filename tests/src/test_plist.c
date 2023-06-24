#include <assert.h>

#include "../../inc/vita/container/str.h"
#include "../../inc/vita/container/plist.h"

int main(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    vt_plist_t *p = vt_plist_create(5, alloctr); {
        assert(vt_plist_len(p) == 0);
        assert(vt_plist_capacity(p) == 5);

        char *h = "hello";
        char *w = "world";
        char *t = "temp";
        vt_plist_push(p, h);
        vt_plist_push(p, w);
        assert(vt_plist_len(p) == 2);
        assert(vt_plist_capacity(p) == 5);
        assert(vt_plist_has_space(p) == 3);
        assert(vt_plist_can_find(p, w) == 1);
        assert(vt_plist_can_find(p, t) == -1);

        vt_plist_shrink(p);
        assert(vt_plist_capacity(p) == 2);

        vt_plist_reserve(p, 5);
        assert(vt_plist_capacity(p) == 7);

        assert(strncmp(vt_plist_pop_get(p), w, strlen(w)) == 0);
        assert(vt_plist_len(p) == 1);
        assert(vt_plist_capacity(p) == 7);

        vt_plist_remove(p, 0, vt_remove_stategy_fast);
        assert(vt_plist_len(p) == 0);
        assert(vt_plist_capacity(p) == 7);

        vt_plist_push(p, h);
        vt_plist_push(p, w);
        vt_plist_push(p, h);
        vt_plist_push(p, w);
        vt_plist_push(p, h);
        vt_plist_push(p, w);

        void *i = NULL;
        while((i = vt_array_slide_front(p)) != NULL) {
            // printf("%s\n", *((char**)(i)));
        }

        // copying 
        vt_plist_t *pcopy = vt_plist_dup(p, alloctr); {
            assert(vt_plist_len(pcopy) == vt_plist_len(p));
            assert(vt_plist_get(pcopy, 0) == vt_plist_get(p, 0));
            assert(vt_str_equals(vt_plist_get(pcopy, 0), "hello"));
        } vt_plist_destroy(pcopy);

        // resize
        vt_plist_resize(p, 3);
        assert(vt_plist_len(p) == 3);
    } vt_plist_destroy(p);

    vt_mallocator_destroy(alloctr);
    return 0;
}
