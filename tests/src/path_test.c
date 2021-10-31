#include <assert.h>

#include "../../include/vita/system/path.h"

int main(void) { 
	str_t *s = path_build("hello", "world"); {
		assert(str_equals(cstr(s), "hello/world"));
	} str_free(s);

	plist_t *p = plist_create(3); {
		plist_push(p, "hello");
		plist_push(p, "world");
		plist_push(p, "folder");

		str_t *sp = path_build_n(p); {
			assert(str_equals(cstr(sp), "hello/world/folder"));
		} str_free(sp);
	} plist_free(p);

	return 0;
}
































