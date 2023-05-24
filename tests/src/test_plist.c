#include <assert.h>

#include "../../inc/vita/container/plist.h"

int main(void) {
    VT_DEBUG_DEFAULT_INIT();
    
    vt_plist_t *p = vt_plist_create(5); {
        assert(vt_plist_len(p) == 0);
        assert(vt_plist_capacity(p) == 5);

        char *h = "hello";
        char *w = "world";
        vt_plist_push(p, h);
        vt_plist_push(p, w);
        assert(vt_plist_len(p) == 2);
        assert(vt_plist_capacity(p) == 5);
        assert(vt_plist_has_space(p) == 3);

        vt_plist_shrink(p);
        assert(vt_plist_capacity(p) == 2);

        vt_plist_reserve(p, 5);
        assert(vt_plist_capacity(p) == 7);

        assert(strncmp(vt_plist_pop_get(p), w, strlen(w)) == 0);
        assert(vt_plist_len(p) == 1);
        assert(vt_plist_capacity(p) == 7);

        vt_plist_remove(p, 0, vt_rs_fast);
        assert(vt_plist_len(p) == 0);
        assert(vt_plist_capacity(p) == 7);

        vt_plist_push(p, h);
        vt_plist_push(p, w);
        vt_plist_push(p, h);
        vt_plist_push(p, w);
        vt_plist_push(p, h);
        vt_plist_push(p, w);

        void *i = NULL;
        while((i = vt_bat_slide_front(p)) != NULL) {
            // printf("%s\n", *((char**)(i)));
        }
    } vt_plist_destroy(p);

    VT_DEBUG_DEFAULT_QUIT();
    return 0;
}
