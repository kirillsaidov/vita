#include <assert.h>

#include "../../inc/vita/system/path.h"

void free_str(void *ptr, size_t i);

int main(void) {
	str_t *s = path_build(NULL, "hello", "world"); {
		assert(str_equals(cstr(s), "hello/world"));
	} str_free(s);

	plist_t *p = plist_create(3); {
		plist_push(p, "hello");
		plist_push(p, "world");
		plist_push(p, "folder");

		str_t *sp = path_build_n(NULL, p); {
			assert(str_equals(cstr(sp), "hello/world/folder"));
		} str_free(sp);
	} plist_free(p);

	assert(path_exists("/home/lala") == false); // must fail

    #ifdef __APPLE__
        str_t *cwd = path_getcwd(); {
            assert(str_equals(cstr(cwd), "/Users/KS/Desktop/myfiles/media/dev/repos/bitbucket/vita/tests/src"));
        } str_free(cwd);

        assert(path_exists("/Users/KS/Desktop/myfiles/media/dev/repos/bitbucket/vita/tests/src"));
        assert(path_is_dir("/Users/KS/Desktop/myfiles/media/dev/repos/bitbucket/vita/tests/src"));
    	assert(path_is_file("/Users/KS/Desktop/myfiles/media/dev/repos/bitbucket/vita/tests/src/test_path.c"));

	    plist_t *pdir = path_listdir("/Users/KS/Desktop/myfiles/media/dev/repos/bitbucket/vita/tests/src/"); {
		    assert(plist_len(pdir) == 9);
		    plist_foreach(pdir, free_str);
	    } plist_free(pdir);
    #elif defined(_WIN32) || defined(_WIN64)
        //...
    #else
        str_t *cwd = path_getcwd(); {
            assert(str_equals(cstr(cwd), "/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests"));
        } str_free(cwd);

        assert(path_exists("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests"));
        assert(path_is_dir("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests"));
    	assert(path_is_file("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests/src/test_path.c"));

	    plist_t *pdir = path_listdir("/home/kirill/myfiles/media/dev/repos/bitbucket/vita/tests/src/"); {
		    assert(plist_len(pdir) == 9);
		    plist_foreach(pdir, free_str);
	    } plist_free(pdir);
    #endif

	str_t *sbasename = str("my/test/folder/text.txt"); {
		assert(str_equals(cstr(path_basename(sbasename, cstr(sbasename))), "text.txt"));
	} str_free(sbasename);

    // make directories
    //path_mkdir("hello_test_dir"); // works
    //path_mkdir_parents("/hello/world/of/my/"); // works
    
    // rename file/dirs
    //path_rename("hello", "hello_renamed"); // works

    return 0;
}

void free_str(void *ptr, size_t i) {
	str_free(ptr);
}















