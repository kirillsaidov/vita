#include <assert.h>
#include "../../inc/vita/system/path.h"

#define FILES_IN_DIR 12

// helper functions
void free_str(void *ptr, size_t i);

// test functions
void test_path(void);
void test_expand_tilde(void);

int main(void) {
    DEBUG_DEFAULT_INIT();
    // debug_disable_output(true);

    // tests
    test_path();    
    test_expand_tilde();

    DEBUG_DEFAULT_QUIT();
    return 0;
}

void free_str(void *ptr, size_t i) {
    str_free(ptr);
}

void test_path(void) {
    plist_t *p = plist_create(3);
        plist_push(p, "hello");
        plist_push(p, "world");
        plist_push(p, "folder");

        str_t *sp = path_build_n(NULL, p); {
            assert(str_equals(cstr(sp), "hello/world/folder"));
        } str_free(sp);
    plist_destroy(p);

    assert(path_exists("/home/lala") == false); // must fail

    #if defined(_WIN32) || defined(_WIN64)
        str_t *s = path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(str_equals(cstr(s), "hello\\world\\media\\dev\\"));
        str_free(s);

        str_t *cwd = path_getcwd(); {
            assert(str_equals(cstr(cwd), "D:\\myfiles\\media\\dev\\repos\\gitlab.kirill.saidov\\vita\\tests"));
        } str_free(cwd);

        assert(path_exists("D:\\myfiles\\media\\dev\\repos\\gitlab.kirill.saidov\\vita\\tests"));
        assert(path_is_dir("D:\\myfiles\\media\\dev\\repos\\gitlab.kirill.saidov\\vita\\tests\\src"));
        assert(path_is_file("D:\\myfiles\\media\\dev\\repos\\gitlab.kirill.saidov\\vita\\tests\\src\\test_path.c"));

        plist_t *pdir = path_listdir(NULL, "D:\\myfiles\\media\\dev\\repos\\gitlab.kirill.saidov\\vita\\tests\\src", true); {
            assert(plist_len(pdir) == FILES_IN_DIR);
            plist_apply(pdir, free_str);
        } plist_destroy(pdir);

        str_t *sbasename = str("my\\test\\folder\\text.txt"); {
            assert(str_equals(cstr(path_basename(sbasename, cstr(sbasename))), "text.txt"));
        } str_free(sbasename);

        // make directories
        // path_mkdir("hello_test_dir"); // works
        // path_mkdir_parents("\\hello\\world\\of\\my\\"); // works
    #elif defined(__linux__)
        str_t *s = path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(str_equals(cstr(s), "hello/world/media/dev/"));
        str_free(s);

        str_t *cwd = path_getcwd(); {
            assert(str_equals(cstr(cwd), "/home/kirill/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        } str_free(cwd);

        assert(path_exists("/home/kirill/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(path_is_dir("/home/kirill/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(path_is_file("/home/kirill/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/test_path.c"));

        plist_t *pdir = path_listdir(NULL, "/home/kirill/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/", true); {
            assert(plist_len(pdir) == FILES_IN_DIR);
            plist_apply(pdir, free_str);
        } plist_destroy(pdir);

        str_t *sbasename = str("my/test/folder/text.txt"); {
            assert(str_equals(cstr(path_basename(sbasename, cstr(sbasename))), "text.txt"));
        } str_free(sbasename);

        // make directories
        // path_mkdir("hello_test_dir"); // works
        // path_mkdir_parents("/hello/world/of/my/"); // works
    #else
        str_t *s = path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(str_equals(cstr(s), "hello/world/media/dev/"));
        str_free(s);

        str_t *cwd = path_getcwd(); {
            assert(str_equals(cstr(cwd), "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        } str_free(cwd);

        assert(path_exists("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(path_is_dir("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(path_is_file("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/test_path.c"));

        plist_t *pdir = path_listdir(NULL, "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/", true); {
            assert(plist_len(pdir) == FILES_IN_DIR);
            plist_apply(pdir, free_str);
        } plist_destroy(pdir);

        str_t *sbasename = str("my/test/folder/text.txt"); {
            assert(str_equals(cstr(path_basename(sbasename, cstr(sbasename))), "text.txt"));
        } str_free(sbasename);

        // make directories
        // path_mkdir("hello_test_dir"); // works
        // path_mkdir_parents("/hello/world/of/my/"); // works
    #endif
    
    // rename file/dirs
    // path_rename("hello", "hello_renamed"); // works
    
    const size_t fs = path_get_file_size("src/test_str.c");
    // assert(fs == 7077);
}

void test_expand_tilde(void) {
    const char *z_path_tilde1 = "~/hello";
    const char *z_path_tilde2 = "./~";
    str_t *s_path_tilde1 = path_expand_tilde(z_path_tilde1);
    str_t *s_path_tilde2 = path_expand_tilde(z_path_tilde2);    
    {   
        #if defined(_WIN32) || defined(_WIN64)
            assert(str_equals(cstr(s_path_tilde1), "C:\\Users\\Kirill Saidov/hello"));
        #elif defined(__linux__)
            assert(str_equals(cstr(s_path_tilde1), "/home/kirill/hello"));
        #else
            assert(str_equals(cstr(s_path_tilde1), "/Users/KS/hello"));
        #endif

        assert(str_equals(cstr(s_path_tilde2), z_path_tilde2)); // since '~' does not start from [0] position, don't do anything
    }
    str_free(s_path_tilde1);
    str_free(s_path_tilde2);
}















