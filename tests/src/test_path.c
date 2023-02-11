#include <assert.h>
#include "../../inc/vita/system/path.h"

#define FILES_IN_DIR 12

// helper functions
void free_str(void *ptr, size_t i);

// test functions
void test_path(void);
void test_expand_tilda(void);
void test_selfpath(void);

int main(void) {
    VT_DEBUG_DEFAULT_INIT();
    // debug_disable_output(true);

    // tests
    test_path();    
    // test_expand_tilda();
    // test_selfpath();

    VT_DEBUG_DEFAULT_QUIT();
    return 0;
}

void free_str(void *ptr, size_t i) {
    free(ptr);
}

void test_path(void) {
    vt_plist_t *p = vt_plist_create(3);
        vt_plist_push(p, "hello");
        vt_plist_push(p, "world");
        vt_plist_push(p, "folder");

        vt_str_t *sp = vt_path_build(NULL, p); {
            assert(vt_str_equals(vt_cstr(sp), "hello/world/folder"));
        } vt_str_free(sp);
    vt_plist_destroy(p);

    assert(vt_path_exists("/home/lala") == false); // must fail

    #if defined(_WIN32) || defined(_WIN64)
        vt_str_t *s = vt_path_build_n(NULL, 4, "hello", "world", "media", "dev");
        assert(vt_str_equals(vt_cstr(s), "hello\\world\\media\\dev"));
        vt_str_free(s);

        vt_str_t *cwd = vt_path_getcwd(); {
            assert(vt_str_equals(vt_cstr(cwd), "D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests"));
        } vt_str_free(cwd);

        assert(vt_path_exists("D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests"));
        assert(vt_path_is_dir("D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests\\src"));
        assert(vt_path_is_file("D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests\\src\\test_path.c"));

        vt_plist_t *pdir = vt_path_listdir(NULL, "D:\\myfiles\\media\\devrepos\\gitlab.kirill.saidov\\vita\\tests\\src", true); {
            assert(vt_plist_len(pdir) == FILES_IN_DIR);
            vt_plist_apply(pdir, free_str);
        } vt_plist_destroy(pdir);

        vt_str_t *sbasename = vt_str("my\\test\\folder\\text.txt"); {
            assert(vt_str_equals(vt_cstr(vt_path_basename(sbasename, vt_cstr(sbasename))), "text.txt"));
        } vt_str_free(sbasename);

        // make directories
        // vt_path_mkdir("hello_test_dir"); // works
        // vt_path_mkdir_parents("\\hello\\world\\of\\my\\"); // works
    #elif defined(__linux__)
        vt_str_t *s = vt_path_build_n(NULL, 4, "hello", "world", "media", "dev");
        assert(vt_str_equals(vt_cstr(s), "hello/world/media/dev/"));
        vt_str_free(s);

        vt_str_t *cwd = vt_path_getcwd(); {
            assert(vt_str_equals(vt_cstr(cwd), "/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src"));
        } vt_str_free(cwd);

        assert(vt_path_exists("/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src"));
        assert(vt_path_is_dir("/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src"));
        assert(vt_path_is_file("/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src/test_path.c"));

        vt_plist_t *pdir = vt_path_listdir(NULL, "/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/gitlab.kirill.saidov/Vita/tests/src/", true); {
            assert(vt_plist_len(pdir) == FILES_IN_DIR);
            vt_plist_apply(pdir, free_str);
        } vt_plist_destroy(pdir);

        vt_str_t *sbasename = vt_str("my/test/folder/text.txt"); {
            assert(vt_str_equals(vt_cstr(vt_path_basename(sbasename, vt_cstr(sbasename))), "text.txt"));
        } vt_str_free(sbasename);

        // make directories
        // vt_path_mkdir("hello_test_dir"); // works
        // vt_path_mkdir_parents("/hello/world/of/my/"); // works
    #else
        vt_str_t *s = vt_path_build(NULL, 4, "hello", "world", "media", "dev");
        assert(vt_str_equals(vt_cstr(s), "hello/world/media/dev/"));
        vt_str_free(s);

        vt_str_t *cwd = vt_path_getcwd(); {
            assert(vt_str_equals(vt_cstr(cwd), "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        } vt_str_free(cwd);

        assert(vt_path_exists("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(vt_path_is_dir("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src"));
        assert(vt_path_is_file("/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/test_path.c"));

        vt_plist_t *pdir = vt_path_listdir(NULL, "/Users/KS/Desktop/myfiles/media/dev/repos/gitlab.kirill.saidov/vita/tests/src/", true); {
            assert(vt_plist_len(pdir) == FILES_IN_DIR);
            vt_plist_apply(pdir, free_str);
        } vt_plist_destroy(pdir);

        vt_str_t *sbasename = vt_str("my/test/folder/text.txt"); {
            assert(vt_str_equals(vt_cstr(vt_path_basename(sbasename, vt_cstr(sbasename))), "text.txt"));
        } vt_str_free(sbasename);

        // make directories
        // vt_path_mkdir("hello_test_dir"); // works
        // vt_path_mkdir_parents("/hello/world/of/my/"); // works
    #endif
    
    // rename file/dirs
    // vt_path_rename("hello", "hello_renamed"); // works
    
    const size_t fs = vt_path_get_file_size("src/test_str.c");
    // assert(fs == 7077);
}

void test_expand_tilda(void) {
    const char *z_vt_path_tilda1 = "~/hello";
    const char *z_vt_path_tilda2 = "./~";
    vt_str_t *s_vt_path_tilda1 = vt_path_expand_tilda(z_vt_path_tilda1);
    vt_str_t *s_vt_path_tilda2 = vt_path_expand_tilda(z_vt_path_tilda2);    
    {   
        #if defined(_WIN32) || defined(_WIN64)
            assert(vt_str_equals(vt_cstr(s_vt_path_tilda1), "C:\\Users\\Kirill Saidov/hello"));
        #elif defined(__linux__)
            assert(vt_str_equals(vt_cstr(s_vt_path_tilda1), "/home/kiril/hello"));
        #else
            assert(vt_str_equals(vt_cstr(s_vt_path_tilda1), "/Users/KS/hello"));
        #endif

        assert(vt_str_equals(vt_cstr(s_vt_path_tilda2), z_vt_path_tilda2)); // since '~' does not start from [0] position, don't do anything
    }
    vt_str_free(s_vt_path_tilda1);
    vt_str_free(s_vt_path_tilda2);
}

void test_selfpath(void) {
    vt_str_t *selfpath = vt_path_get_this_exe_location();
    VT_DEBUG_ASSERT(selfpath != NULL, "selfpath is NULL");
    VT_DEBUG_PRINTF("this exe path: %s\n", vt_cstr(selfpath));
    vt_str_free(selfpath);
}















