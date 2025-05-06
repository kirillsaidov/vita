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
    vt_span_t span = {0};
    char buffer[VT_PATH_MAX] = {0};

    assert(vt_path_exists("/home/lala") == false); // must fail

    vt_str_t *s = vt_path_join(NULL, "hello", "world", "media", "dev");
    assert(vt_str_equals_z(vt_str_z(s), "hello/world/media/dev"));
    vt_str_destroy(s);

    // cwd
    vt_span_t _cwd = vt_path_get_cwd(buffer, 5);
    assert(vt_span_len(_cwd) == 0);
    assert(vt_span_head(_cwd) == NULL);
    assert(!vt_span_is_valid_object(_cwd));
    _cwd = vt_path_get_cwd(buffer, VT_PATH_MAX);

    vt_str_t *cwd = vt_str_create(vt_span_head(_cwd), alloctr); {
        // test basename 1
        char _buf_s[6] = {0};
        span = vt_path_basename("hello/world", _buf_s, sizeof(_buf_s)/sizeof(_buf_s[0]));
        assert(vt_span_is_valid_object(span));
        assert(vt_str_equals_n("world", vt_span_head(span), vt_span_len(span)));
        assert(vt_span_len(span) == 5);

        const char *test_cases_basename[][2] = {
            {vt_str_z(cwd), "tests"},
            {"hello/world/", "world"},
            {"world", "world"},
            {"world/", "world"},
            {"/world/", "world"},
            {"/world", "world"},
            {"apple/juice", "juice"},
            {"apple/juice/", "juice"},
        };
        VT_FOREACH(i, 0, sizeof(test_cases_basename)/sizeof(test_cases_basename[0])) {
            span = vt_path_basename(test_cases_basename[i][0], buffer, sizeof(buffer)/sizeof(buffer[0]));
            printf("%s <==> %s | %s\n", test_cases_basename[i][0], test_cases_basename[i][1], vt_span_head(span));
            assert(vt_str_equals_n(test_cases_basename[i][1], vt_span_head(span), vt_span_len(span)));
        }

        // test dirname 1
        char _buf[13] = {0};    
        span = vt_path_dirname("this/is/path/file.txt", _buf, sizeof(_buf)/sizeof(_buf[0]));
        assert(vt_span_is_valid_object(span));
        assert(vt_str_equals_n("this/is/path", vt_span_head(span), vt_span_len(span)));
        assert(vt_span_len(span) == 12);

        const char *test_cases_dirname[][2] = {
            {"this/is/path", "this/is"},
            {"this", "."},
            {"another/one/here.txt", "another/one"},
            {"another/one/", "another"},
            {"another", "."},
            {"/another", "."},
        };
        VT_FOREACH(i, 0, sizeof(test_cases_dirname)/sizeof(test_cases_dirname[0])) {
            span = vt_path_dirname(test_cases_dirname[i][0], buffer, sizeof(buffer)/sizeof(buffer[0]));
            assert(vt_str_equals_n(test_cases_dirname[i][1], vt_span_head(span), vt_span_len(span)));
        }
    } vt_str_destroy(cwd);

    assert(vt_path_exists("./src"));
    assert(vt_path_is_dir("./src"));
    assert(vt_path_is_file("./src/test_path.c"));

    vt_plist_t *pdir = vt_path_dir_list(NULL, "./src/", true); {
        assert(vt_plist_len(pdir) == FILES_IN_DIR);
    } vt_path_dir_free(pdir);

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
