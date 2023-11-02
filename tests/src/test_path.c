#include <assert.h>
#include "vita/system/path.h"

#define FILES_IN_DIR 18

// helper functions
void free_str(void *ptr, size_t i);

// test functions
void test_path(void);
void test_expand_tilda(void);
void test_selfpath(void);
void test_path_pop(void);

static vt_mallocator_t *alloctr;

int main(void) {
    alloctr = vt_mallocator_create();

    // debug_disable_output(true);

    // tests
    test_path();    
    test_expand_tilda();
    test_selfpath();
    test_path_pop();

    vt_mallocator_destroy(alloctr);
    return 0;
}

void free_str(void *ptr, size_t i) {
    // printf("* (%zu) %s\n", i, vt_str_z((vt_str_t*)ptr));
    vt_str_destroy((vt_str_t*)ptr);
}

void test_path(void) {
    assert(vt_path_exists("/home/lala") == false); // must fail

    #if defined(_WIN32) || defined(_WIN64)
        vt_str_t *s = vt_path_build_n(NULL, 4, "hello", "world", "media", "dev");
        assert(vt_str_equals_z(vt_str_z(s), "hello\\world\\media\\dev"));
        vt_str_destroy(s);

        vt_str_t *cwd = vt_path_get_cwd(alloctr); {
            assert(vt_str_equals_z(vt_str_z(cwd), "C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests"));
        } vt_str_destroy(cwd);

        assert(vt_path_exists("C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests"));
        assert(vt_path_exists("C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests\\src\\test_path.c"));
        assert(vt_path_is_dir("C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests\\src"));
        assert(vt_path_is_file("C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests\\src\\test_path.c"));

        vt_plist_t *pdir = vt_path_listdir(NULL, "C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests\\src", true); {
            assert(vt_plist_len(pdir) == FILES_IN_DIR);
            vt_plist_apply(pdir, free_str);
        } vt_plist_destroy(pdir);

        vt_str_t *sbasename = vt_str_create("my\\test\\folder\\text.txt", alloctr); {
            assert(vt_str_equals_z(vt_str_z(vt_path_basename(sbasename, vt_str_z(sbasename))), "text.txt"));
        } vt_str_destroy(sbasename);

        // make a directory
        vt_path_mkdir("hello_test_dir");
        assert(vt_path_exists("hello_test_dir"));

        // make nested directories
        vt_path_mkdir_parents("hello\\world\\of\\my\\");
        assert(vt_path_exists("hello\\world\\of\\my\\"));

        // remove a directory
        vt_path_rmdir("hello_test_dir");
        assert(!vt_path_exists("hello_test_dir"));

        // remove nested directories
        vt_path_rmdir_recurse("hello\\world\\of\\my\\");
        assert(!vt_path_exists("hello\\world\\of\\my\\"));
    #elif defined(__linux__)
        vt_str_t *s = vt_path_build_n(NULL, 4, "hello", "world", "media", "dev");
        assert(vt_str_equals_z(vt_str_z(s), "hello/world/media/dev"));
        vt_str_destroy(s);

        vt_str_t *cwd = vt_path_get_cwd(alloctr); {
            assert(vt_str_equals_z(vt_str_z(cwd), "/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/git.kirillsaidov/vita/tests/src"));
        } vt_str_destroy(cwd);

        assert(vt_path_exists("/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/git.kirillsaidov/vita/tests/src"));
        assert(vt_path_is_dir("/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/git.kirillsaidov/vita/tests/src"));
        assert(vt_path_is_file("/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/git.kirillsaidov/vita/tests/src/test_path.c"));

        vt_plist_t *pdir = vt_path_listdir(NULL, "/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/git.kirillsaidov/vita/tests/src/", true); {
            assert(vt_plist_len(pdir) == FILES_IN_DIR);
            vt_plist_apply(pdir, free_str);
        } vt_plist_destroy(pdir);

        vt_str_t *sbasename = vt_str_create("my/test/folder/text.txt", alloctr); {
            assert(vt_str_equals_z(vt_str_z(vt_path_basename(sbasename, vt_str_z(sbasename))), "text.txt"));
        } vt_str_destroy(sbasename);

        // make a directory
        vt_path_mkdir("hello_test_dir");
        assert(vt_path_exists("hello_test_dir"));

        // make nested directories
        vt_path_mkdir_parents("hello/world/of/my/");
        assert(vt_path_exists("hello/world/of/my/"));

        // remove a directory
        vt_path_rmdir("hello_test_dir");
        assert(!vt_path_exists("hello_test_dir"));

        // remove nested directories
        vt_path_rmdir_recurse("hello/world/of/my/");
        assert(!vt_path_exists("hello/world/of/my/"));
    #else
        vt_str_t *s = vt_path_build_n(NULL, 4, "hello", "world", "media", "dev");
        printf("PATH: %s\n", vt_str_z(s));
        assert(vt_str_equals_z(vt_str_z(s), "hello/world/media/dev"));
        vt_str_destroy(s);

        vt_str_t *cwd = vt_path_get_cwd(alloctr); {
            assert(vt_str_equals_z(vt_str_z(cwd), "/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/vita/tests"));
        } vt_str_destroy(cwd);

        assert(vt_path_exists("/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/vita/tests/src"));
        assert(vt_path_is_dir("/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/vita/tests/src"));
        assert(vt_path_is_file("/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/vita/tests/src/test_path.c"));

        vt_plist_t *pdir = vt_path_listdir(NULL, "/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/vita/tests/src/", true); {
            assert(vt_plist_len(pdir) == FILES_IN_DIR);
            vt_plist_apply(pdir, free_str);
        } vt_plist_destroy(pdir);

        vt_str_t *sbasename = vt_str_create("my/test/folder/text.txt", alloctr); {
            assert(vt_str_equals_z(vt_str_z(vt_path_basename(sbasename, vt_str_z(sbasename))), "text.txt"));
        } vt_str_destroy(sbasename);

        vt_str_t *new_s = vt_str_create("file.txt", alloctr);
        {
            vt_path_basename(new_s, vt_str_z(new_s));
            assert(vt_str_equals_z(vt_str_z(new_s), "file.txt"));
        }
        vt_str_destroy(new_s);

        // make a directory
        vt_path_mkdir("hello_test_dir");
        assert(vt_path_exists("hello_test_dir"));

        // make nested directories
        vt_path_mkdir_parents("hello/world/of/my/");
        assert(vt_path_exists("hello/world/of/my/"));

        // remove a directory
        vt_path_rmdir("hello_test_dir");
        assert(!vt_path_exists("hello_test_dir"));

        // remove nested directories
        vt_path_rmdir_recurse("hello/world/of/my/");
        assert(!vt_path_exists("hello/world/of/my/"));
    #endif
    
    // rename file/dirs
    // vt_path_rename("hello", "hello_renamed"); // works
    
    const size_t fs = vt_path_get_file_size("src/test_str.c");
    // assert(fs == 7077);
}

void test_expand_tilda(void) {
    const char *z_vt_path_tilda1 = "~/hello";
    const char *z_vt_path_tilda2 = "./~";
    vt_str_t *s_vt_path_tilda1 = vt_path_expand_tilda(z_vt_path_tilda1, alloctr);
    vt_str_t *s_vt_path_tilda2 = vt_path_expand_tilda(z_vt_path_tilda2, alloctr);    
    {   
        #if defined(_WIN32) || defined(_WIN64)
            assert(vt_str_equals_z(vt_str_z(s_vt_path_tilda1), "C:\\Users\\kiril/hello"));
        #elif defined(__linux__)
            assert(vt_str_equals_z(vt_str_z(s_vt_path_tilda1), "/home/kiril/hello"));
        #else
            assert(vt_str_equals_z(vt_str_z(s_vt_path_tilda1), "/Users/krillos/hello"));
        #endif

        assert(vt_str_equals_z(vt_str_z(s_vt_path_tilda2), z_vt_path_tilda2)); // since '~' does not start from [0] position, don't do anything
    }
    vt_str_destroy(s_vt_path_tilda1);
    vt_str_destroy(s_vt_path_tilda2);
}

void test_selfpath(void) {
    vt_str_t *selfpath = vt_path_get_this_exe_location(alloctr);
    VT_DEBUG_ASSERT(selfpath != NULL, "selfpath is NULL");

    #if defined(_WIN32) || defined(_WIN64)
        assert(vt_str_equals_z(vt_str_z(selfpath), "C:\\Users\\kiril\\Desktop\\MyFiles\\media\\dev\\repos\\git.kirillsaidov\\vita\\tests\\bin\\test_path.exe"));
    #elif defined(__linux__)
        assert(vt_str_equals_z(vt_str_z(selfpath), "/mnt/c/Users/kiril/Desktop/MyFiles/media/dev/repos/git.kirillsaidov/vita/tests/bin/test_path"));
    #else
        assert(vt_str_equals_z(vt_str_z(selfpath), "/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/vita/tests/bin/test_path"));
    #endif
    
    VT_DEBUG_PRINTF("this exe path: %s\n", vt_str_z(selfpath));
    vt_str_destroy(selfpath);
}

void test_path_pop(void) {
    vt_str_t *path = vt_str_create("./hello/world/bin", alloctr);

    #if defined(_WIN32) || defined(_WIN64)
        vt_path_validate((char *const)vt_str_z(path));
        assert(vt_str_equals_z(vt_str_z(path), ".\\hello\\world\\bin"));

        vt_path_pop((char *const)vt_str_z(path));
        assert(vt_str_equals_z(vt_str_z(path), ".\\hello\\world"));
        assert(vt_str_validate_len(path) == 13);

        vt_path_pop((char *const)vt_str_z(path));
        assert(vt_str_equals_z(vt_str_z(path), ".\\hello"));
        assert(vt_str_validate_len(path) == 7);
    #else
        vt_path_pop((char *const)vt_str_z(path));
        assert(vt_str_equals_z(vt_str_z(path), "./hello/world"));
        assert(vt_str_validate_len(path) == 13);

        vt_path_pop((char *const)vt_str_z(path));
        assert(vt_str_equals_z(vt_str_z(path), "./hello"));
        assert(vt_str_validate_len(path) == 7);
    #endif

    vt_path_pop((char *const)vt_str_z(path));
    assert(vt_str_equals_z(vt_str_z(path), "."));
    assert(vt_str_validate_len(path) == 1);

    vt_path_pop((char *const)vt_str_z(path));
    assert(vt_str_equals_z(vt_str_z(path), "."));
    assert(vt_str_validate_len(path) == 1);

    vt_str_destroy(path);
}















