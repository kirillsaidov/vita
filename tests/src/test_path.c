#include <assert.h>

#include "../../inc/vita/system/path.h"

#define FILES_IN_DIR 15

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

    #if defined(_WIN32) || defined(_WIN64)
        //...
    #else
        str_t *cwd = path_getcwd(); {
            assert(str_equals(cstr(cwd), "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab@kirill.saidov/Vita/tests/src"));
        } str_free(cwd);

        assert(path_exists("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab@kirill.saidov/Vita/tests/src"));
        assert(path_is_dir("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab@kirill.saidov/Vita/tests/src"));
    	assert(path_is_file("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab@kirill.saidov/Vita/tests/src/test_path.c"));

	    plist_t *pdir = path_listdir(NULL, "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab@kirill.saidov/Vita/tests/src/", false); {
		    assert(plist_len(pdir) == FILES_IN_DIR);
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
    
    assert(path_get_file_size("src/test_str.c") == 4009);

    return 0;
}

void free_str(void *ptr, size_t i) {
	str_free(ptr);
}















