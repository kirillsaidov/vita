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

vt_mallocator_t *alloctr;

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

    vt_str_t *s = vt_path_join(NULL, "hello", "world", "media", "dev");
    assert(vt_str_equals_z(vt_str_z(s), "hello/world/media/dev"));
    vt_str_destroy(s);

    // cwd
    char buffer[VT_PATH_MAX] = {0};
    vt_span_t _cwd = vt_path_get_cwd(buffer, 5);
    assert(vt_span_len(_cwd) == 0);
    assert(vt_span_head(_cwd) == NULL);
    assert(!vt_span_is_valid_object(_cwd));
    _cwd = vt_path_get_cwd(buffer, VT_PATH_MAX);

    char path_buf[VT_PATH_MAX] = {0};
    vt_str_t *cwd = vt_str_create(vt_span_head(_cwd), alloctr); {
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "tests"));

        // test basename 1
        vt_str_clear(cwd);
        cwd = vt_path_basename(cwd, "hello/world");
        assert(vt_str_equals_z(vt_str_z(cwd), "world"));

        vt_str_clear(cwd);
        cwd = vt_path_basename(cwd, "hello/world/");
        assert(vt_str_equals_z(vt_str_z(cwd), "world/"));

        vt_str_clear(cwd);
        cwd = vt_path_basename(cwd, "world");
        assert(vt_str_equals_z(vt_str_z(cwd), "world"));

        vt_str_clear(cwd);
        cwd = vt_path_basename(cwd, "world/");
        assert(vt_str_equals_z(vt_str_z(cwd), "world/"));

        vt_str_clear(cwd);
        cwd = vt_path_basename(cwd, "/world/");
        assert(vt_str_equals_z(vt_str_z(cwd), "world/"));

        vt_str_clear(cwd);
        cwd = vt_path_basename(cwd, "/world");
        assert(vt_str_equals_z(vt_str_z(cwd), "world"));

        // test basename 2
        vt_str_clear(cwd);
        vt_str_append(cwd, "apple/juice");
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "juice"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "apple/juice/");
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "juice/"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "juice");
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "juice"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "juice/");
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "juice/"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "/juice/");
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "juice/"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "/juice");
        cwd = vt_path_basename(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "juice"));

        // test dirname 1
        vt_str_clear(cwd);
        cwd = vt_path_dirname(cwd, "this/is/path/file.txt");
        assert(vt_str_equals_z(vt_str_z(cwd), "this/is/path"));

        vt_str_clear(cwd);
        cwd = vt_path_dirname(cwd, "this/is/path");
        assert(vt_str_equals_z(vt_str_z(cwd), "this/is"));

        vt_str_clear(cwd);
        cwd = vt_path_dirname(cwd, "this/is/path/");
        assert(vt_str_equals_z(vt_str_z(cwd), "this/is"));

        vt_str_clear(cwd);
        cwd = vt_path_dirname(cwd, "this");
        assert(vt_str_equals_z(vt_str_z(cwd), "."));

        // test dirname 2
        vt_str_clear(cwd);
        vt_str_append(cwd, "another/one/here.txt");
        cwd = vt_path_dirname(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "another/one"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "another/one/");
        cwd = vt_path_dirname(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "another"));

        vt_str_clear(cwd);
        vt_str_append(cwd, "another");
        cwd = vt_path_dirname(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "."));

        vt_str_clear(cwd);
        vt_str_append(cwd, "/another");
        cwd = vt_path_dirname(cwd, vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(cwd), "."));
    } vt_str_destroy(cwd);

    assert(vt_path_exists("./src"));
    assert(vt_path_is_dir("./src"));
    assert(vt_path_is_file("./src/test_path.c"));

    vt_plist_t *pdir = vt_path_dir_list(NULL, "./src/", true); {
        assert(vt_plist_len(pdir) == FILES_IN_DIR);
    } vt_path_dir_free(pdir);

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
    vt_path_rename("hello", "hello_renamed");
    assert(vt_path_exists("hello_renamed"));

    // rename file/dirs
    vt_path_rmdir_recurse("hello_renamed");
    assert(!vt_path_exists("hello_renamed"));

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
            assert(vt_str_equals_z(vt_str_z(s_vt_path_tilda1), "/home/kirillsaidov/hello"));
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
    assert(selfpath != NULL);

    // get cwd to check exe path below
    char buffer[VT_PATH_MAX] = {0};
    vt_span_t _cwd = vt_path_get_cwd(buffer, VT_PATH_MAX);

    vt_str_t *cwd = vt_str_create(vt_span_head(_cwd), alloctr);
    {
        // append exe path
        #if defined(_WIN32) || defined(_WIN64)
            vt_str_append(cwd, "\\bin\\test_path.exe");
        #else
            vt_str_append(cwd, "/bin/test_path");
        #endif
        // printf("<%s> == <%s>\n", vt_str_z(selfpath), vt_str_z(cwd));
        assert(vt_str_equals(selfpath, cwd));
    }
    vt_str_destroy(cwd);
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
