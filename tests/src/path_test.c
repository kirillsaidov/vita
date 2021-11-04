#include <assert.h>

#include "../../include/vita/system/path.h"

void free_str(void *ptr, size_t i);

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

	str_t *cwd = path_getcwd(); {
		assert(str_equals(cstr(cwd), "/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests"));
	} str_free(cwd);

	// assert(path_exists("/home/lala")); // must fail
	assert(path_exists("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests"));
	assert(path_fexists("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests/src/path_test.c"));

	plist_t *pdir = path_listdir("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests/src/"); {
		assert(plist_len(pdir) == 7);
		plist_foreach(pdir, free_str);
	} plist_free(pdir);

	return 0;
}

void free_str(void *ptr, size_t i) {
	str_free(ptr);
}
































