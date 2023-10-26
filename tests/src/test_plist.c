#include <assert.h>

#include "../../inc/vita/container/str.h"
#include "../../inc/vita/container/plist.h"
#include "../../inc/vita/util/log.h"

int main(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    // elements
    char *h = "hello";
    char *w = "world";
    char *t = "temp";
    char *a = "Aaaa";
    char *b = "Bbbb";
    char *c = "Cccc";
    char *d = "Dddd";
    
    vt_plist_t *p = vt_plist_create(5, alloctr); {
        assert(vt_plist_len(p) == 0);
        assert(vt_plist_capacity(p) == 5);

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

        vt_plist_remove(p, 0, VT_REMOVE_STRATEGY_FAST);
        assert(vt_plist_len(p) == 0);
        assert(vt_plist_capacity(p) == 7);

        vt_plist_push(p, h);
        vt_plist_push(p, w);
        vt_plist_push(p, h);
        vt_plist_push(p, w);
        vt_plist_push(p, h);
        vt_plist_push(p, w);

        void *i = NULL;
        while ((i = vt_array_slide_front(p)) != NULL) {
            printf("%s\n", *((char**)(i)));
        }

        char *c = NULL;
        while ((c = vt_plist_slide_front(p)) != NULL) {
            printf("--- %s\n", c);
        }

        // copying 
        vt_plist_t *pcopy = vt_plist_dup(p, alloctr); {
            assert(vt_plist_len(pcopy) == vt_plist_len(p));
            assert(vt_plist_get(pcopy, 0) == vt_plist_get(p, 0));
            assert(vt_str_equals_z(vt_plist_get(pcopy, 0), "hello"));
        } vt_plist_destroy(pcopy);

        // resize
        vt_plist_resize(p, 3);
        assert(vt_plist_len(p) == 3);
    } vt_plist_destroy(p);

    // testing elements removal
    vt_plist_t *list = vt_plist_create(1, alloctr);
    {
        // push items
        vt_plist_push(list, a);
        vt_plist_push(list, b);
        vt_plist_push(list, c);
        vt_plist_push(list, d);
        vt_plist_push(list, h);
        vt_plist_push(list, w);
        vt_plist_push(list, t);
        assert(vt_plist_len(list) == 7);
        assert(vt_plist_get(list, 0) == a);

        vt_plist_remove(list, 0, VT_REMOVE_STRATEGY_STABLE);
        assert(vt_plist_get(list, 0) != NULL);
        assert(vt_plist_get(list, 0) == b);
        assert(vt_plist_len(list) == 6);

        vt_plist_remove(list, 0, VT_REMOVE_STRATEGY_FAST);
        assert(vt_plist_get(list, 0) != NULL);
        assert(vt_plist_get(list, 0) == t);
        assert(vt_plist_len(list) == 5);

        vt_plist_remove(list, 0, VT_REMOVE_STRATEGY_STABLE);
        assert(vt_plist_get(list, 0) != NULL);
        assert(vt_plist_get(list, 0) == c);
        assert(vt_plist_len(list) == 4);

        vt_plist_remove(list, 0, VT_REMOVE_STRATEGY_FAST);
        assert(vt_plist_get(list, 0) != NULL);
        assert(vt_plist_get(list, 0) == w);
        assert(vt_plist_get(list, 1) == d);
        assert(vt_plist_get(list, 2) == h);
        assert(vt_plist_len(list) == 3);

        vt_plist_shrink(list);
        assert(vt_plist_len(list) == 3);
        assert(vt_plist_capacity(list) == 3);

        vt_plist_remove(list, vt_plist_len(list) - 1, VT_REMOVE_STRATEGY_STABLE);
        assert(vt_plist_len(list) == 2);
        assert(vt_plist_capacity(list) == 3);
    }
    vt_plist_destroy(list);

    vt_mallocator_destroy(alloctr);
    return 0;
}
