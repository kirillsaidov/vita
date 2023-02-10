#include <assert.h>
#include "../../inc/vita/system/path.h"

#define FILES_IN_DIR 11

// helper functions
void free_str(void *ptr, size_t i);

// test functions
void test_path(void);
void test_expand_tilda(void);
void test_selfpath(void);

int main(void) {
    DEBUG_DEFAULT_INIT();
    // debug_disable_output(true);

    // tests
    test_path();    
    test_expand_tilda();
    test_selfpath();

    DEBUG_DEFAULT_QUIT();
    return 0;
}

void free_str(void *ptr, size_t i) {
    str_free(ptr);
}

void test_path(void) {
    vt_plist_t *p = plist_create(3);
        plist_push(p, "hello");
        plist_push(p, "world");
        plist_push(p, "folder");

        vt_str_t *sp = path_build_n(NULL, p); {
            assert(str_equals(vt_cstr(sp), "hello/world/folder"));
        } str_free(sp);
    plist_destroy(p);

    assert(path_exists("/home/lala") == false); // must fail

    #if defined(_WIN32) || defined(_WIN64)
        vt_str_t *s = path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(str_equals(vt_cstr(s), "hello\\world\\media\\dev"));
        str_free(s);

        vt_str_t *cwd = path_getcwd(); {
            assert(str_equals(vt_cstr(cwd), "D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests"));
        } str_free(cwd);

        assert(path_exists("D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests"));
        assert(path_is_dir("D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests\\src"));
        assert(path_is_file("D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests\\src\\test_path.c"));

        vt_plist_t *pdir = path_listdir(NULL, "D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests\\src", true); {
            assert(plist_len(pdir) == FILES_IN_DIR);
            plist_apply(pdir, free_str);
        } plist_destroy(pdir);

        vt_str_t *sbasename = str("my\\test\\folder\\text.txt"); {
            assert(str_equals(vt_cstr(path_basename(sbasename, vt_cstr(sbasename))), "text.txt"));
        } str_free(sbasename);

        // make directories
        // path_mkdir("hello_test_dir"); // works
        // path_mkdir_parents("\\hello\\world\\of\\my\\"); // works
    #elif defined(__linux__)
        vt_str_t *s = path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(str_equals(vt_cstr(s), "hello/world/media/dev/"));
        str_free(s);

        vt_str_t *cwd = path_getcwd(); {
            assert(str_equals(vt_cstr(cwd), "/mnt/c/Users/kiril/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src"));
        } str_free(cwd);

        assert(path_exists("/mnt/c/Users/kiril/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src"));
        assert(path_is_dir("/mnt/c/Users/kiril/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src"));
        assert(path_is_file("/mnt/c/Users/kiril/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src/test_path.c"));

        vt_plist_t *pdir = path_listdir(NULL, "/mnt/c/Users/kiril/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src/", true); {
            assert(plist_len(pdir) == FILES_IN_DIR);
            plist_apply(pdir, free_str);
        } plist_destroy(pdir);

        vt_str_t *sbasename = str("my/test/folder/text.txt"); {
            assert(str_equals(vt_cstr(path_basename(sbasename, vt_cstr(sbasename))), "text.txt"));
        } str_free(sbasename);

        // make directories
        // path_mkdir("hello_test_dir"); // works
        // path_mkdir_parents("/hello/world/of/my/"); // works
    #else
        vt_str_t *s = path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(str_equals(vt_cstr(s), "hello/world/media/dev/"));
        str_free(s);

        vt_str_t *cwd = path_getcwd(); {
            assert(str_equals(vt_cstr(cwd), "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        } str_free(cwd);

        assert(path_exists("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(path_is_dir("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(path_is_file("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/test_path.c"));

        vt_plist_t *pdir = path_listdir(NULL, "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/", true); {
            assert(plist_len(pdir) == FILES_IN_DIR);
            plist_apply(pdir, free_str);
        } plist_destroy(pdir);

        vt_str_t *sbasename = str("my/test/folder/text.txt"); {
            assert(str_equals(vt_cstr(path_basename(sbasename, vt_cstr(sbasename))), "text.txt"));
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

void test_expand_tilda(void) {
    const char *z_path_tilda1 = "~/hello";
    const char *z_path_tilda2 = "./~";
    vt_str_t *s_path_tilda1 = path_expand_tilda(z_path_tilda1);
    vt_str_t *s_path_tilda2 = path_expand_tilda(z_path_tilda2);    
    {   
        #if defined(_WIN32) || defined(_WIN64)
            assert(str_equals(vt_cstr(s_path_tilda1), "C:\\Users\\Kirill Saidov/hello"));
        #elif defined(__linux__)
            assert(str_equals(vt_cstr(s_path_tilda1), "/home/kiril/hello"));
        #else
            assert(str_equals(vt_cstr(s_path_tilda1), "/Users/KS/hello"));
        #endif

        assert(str_equals(vt_cstr(s_path_tilda2), z_path_tilda2)); // since '~' does not start from [0] position, don't do anything
    }
    str_free(s_path_tilda1);
    str_free(s_path_tilda2);
}

void test_selfpath(void) {
    vt_str_t *selfpath = path_get_this_exe_location();
    DEBUG_ASSERT(selfpath != NULL, "selfpath is NULL");
    VT_DEBUG_PRINTF("this exe path: %s\n", vt_cstr(selfpath));
    str_free(selfpath);
}















