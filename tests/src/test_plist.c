#include <assert.h>

#include "../../inc/vita/container/plist.h"

int main(void) { 
	plist_t *p = plist_create(5); {
		assert(plist_len(p) == 0);
		assert(plist_capacity(p) == 5);

		char *h = "hello";
		char *w = "world";
		plist_push(p, h);
		plist_push(p, w);
		assert(plist_len(p) == 2);
		assert(plist_capacity(p) == 5);
		assert(plist_has_space(p) == 3);

		plist_shrink(p);
		assert(plist_capacity(p) == 2);

		plist_reserve(p, 5);
		assert(plist_capacity(p) == 7);

		assert(strncmp(plist_pop_get(p), w, strlen(w)) == 0);
		assert(plist_len(p) == 1);
		assert(plist_capacity(p) == 7);

		plist_remove(p, 0, rs_fast);
		assert(plist_len(p) == 0);
		assert(plist_capacity(p) == 7);
	} plist_destroy(p);
	
	return 0;
}
































