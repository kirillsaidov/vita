#include <assert.h>
#include "vita/system/path.h"

#define FILES_IN_DIR 18

// helper functions
void free_str(void *ptr, size_t i);

// test functions
void test_path(void);
void test_expand_tilda(void);
void test_selfpath(void);
void test_path_pop_push(void);
void test_path_realpath(void);
void test_path_file_attr(void);

vt_mallocator_t *alloctr;

int main(void) {
    alloctr = vt_mallocator_create();

    // debug_disable_output(true);

    // tests
    test_path();
    test_expand_tilda();
    test_selfpath();
    test_path_pop_push();
    test_path_realpath();
    test_path_file_attr();

    vt_mallocator_destroy(alloctr);
    return 0;
}

void free_str(void *ptr, size_t i) {
    // printf("* (%zu) %s\n", i, vt_str_z((vt_str_t*)ptr));
    vt_str_destroy((vt_str_t*)ptr);
}

void test_path(void) {
    vt_span_t span = {0};
    struct VitaResultSpan ret = {0};
    char buffer[VT_PATH_MAX] = {0};

    assert(vt_path_exists("/home/lala") == false); // must fail

    vt_str_t *s = vt_path_join(NULL, "hello", "world", "media", "dev");
    assert(vt_str_equals_z(vt_str_z(s), "hello/world/media/dev"));
    vt_str_destroy(s);

    // cwd
    ret = vt_path_get_cwd(buffer, 5);
    span = ret.value;
    assert(ret.error);
    assert(vt_span_len(span) == 0);
    assert(vt_span_head(span) == NULL);
    assert(!vt_span_is_valid_object(span));

    ret = vt_path_get_cwd(buffer, VT_PATH_MAX);
    span = ret.value;
    assert(!ret.error);
    assert(vt_span_is_valid_object(span));

    vt_str_t *cwd = vt_str_create(vt_span_head(span), alloctr); {
        // test dirname
        char _buf[13] = {0};    
        span = vt_path_dirname("this/is/path/file.txt", _buf, sizeof(_buf)/sizeof(_buf[0]));
        assert(vt_span_is_valid_object(span));
        assert(vt_str_equals_n("this/is/path", vt_span_head(span), vt_span_len(span)));
        assert(vt_span_len(span) == 12);

        const char *test_cases_dirname[][2] = {
            {"hello/world", "hello"},
            {"hello/world/", "hello"},
            {"/hello/world/", "/hello"},
            {"world", "."},
            {"world/", "."},
            {"/world/", "/"},
            {"/world", "/"},
            {"/", "/"},
            {"/////", "/"},
            {"", "."},
        };
        VT_FOREACH(i, 0, sizeof(test_cases_dirname)/sizeof(test_cases_dirname[0])) {
            span = vt_path_dirname(test_cases_dirname[i][0], buffer, sizeof(buffer)/sizeof(buffer[0]));
            // printf("(%zu) [%s] \t==> [%s]\n", i, test_cases_dirname[i][0], (char*)vt_span_head(span));
            assert(vt_str_equals_n(test_cases_dirname[i][1], vt_span_head(span), vt_span_len(span)));
            assert(vt_span_len(span) == strlen(test_cases_dirname[i][1]));
        }

        // test basename
        char _buf_s[6] = {0};
        span = vt_path_basename("hello/world", _buf_s, sizeof(_buf_s)/sizeof(_buf_s[0]));
        assert(vt_span_is_valid_object(span));
        assert(vt_str_equals_n("world", vt_span_head(span), vt_span_len(span)));
        assert(vt_span_len(span) == 5);
        
        const char *test_cases_basename[][2] = {
            {"hello/world", "world"},
            {"hello/world/", "world/"},
            {"world", "world"},
            {"world/", "world/"},
            {"/world/", "world/"},
            {"/world", "world"},
            {"/", "/"},
            {"/////", "/"},
            {"", "."},
        };
        VT_FOREACH(i, 0, sizeof(test_cases_basename)/sizeof(test_cases_basename[0])) {
            span = vt_path_basename(test_cases_basename[i][0], buffer, sizeof(buffer)/sizeof(buffer[0]));
            // printf("(%zu) [%s] \t==> [%s]\n", i, test_cases_basename[i][0], (char*)vt_span_head(span));
            assert(vt_str_equals_n(test_cases_basename[i][1], vt_span_head(span), vt_span_len(span)));
            assert(vt_span_len(span) == strlen(test_cases_basename[i][1]));
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
    char buffer[VT_PATH_MAX] = {0};

    const char *path_tilda = "~/hello/world";
    vt_span_t span = vt_path_expand_tilda(path_tilda, buffer, sizeof(buffer)/sizeof(buffer[0]));
    {
        #if defined(_WIN32) || defined(_WIN64)
            assert(vt_str_equals_z(vt_str_z(&span.instance), "C:\\Users\\kiril/hello/world"));
            assert(vt_span_len(span) == strlen("C:\\Users\\kiril/hello/world"));
        #elif defined(__linux__)
            assert(vt_str_equals_z(vt_str_z(&span.instance), "/home/kirillsaidov/hello/world"));
            assert(vt_span_len(span) == strlen("/home/kirillsaidov/hello/world"));
        #else
            assert(vt_str_equals_z(vt_str_z(&span.instance), "/Users/krillos/hello/world"));
            assert(vt_span_len(span) == strlen("/Users/krillos/hello/world"));
        #endif
    }

    path_tilda = "./~";
    span = vt_path_expand_tilda(path_tilda, buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(!vt_span_is_valid_object(span));
}

void test_selfpath(void) {
    char buffer[VT_PATH_MAX] = {0};
    struct VitaResultSpan ret = {0}; 

    vt_span_t selfpath = vt_path_get_this_exe_location(buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(vt_span_is_valid_object(selfpath));

    // get cwd to check exe path below
    char buffer_cwd[VT_PATH_MAX] = {0};
    ret = vt_path_get_cwd(buffer_cwd, VT_PATH_MAX);
    assert(!ret.error);
    assert(vt_span_is_valid_object(ret.value));
    vt_str_t *cwd = vt_str_create(buffer_cwd, alloctr);
    {
        // append exe path
        #if defined(_WIN32) || defined(_WIN64)
            vt_str_append(cwd, "\\bin\\test_path.exe");
        #else
            vt_str_append(cwd, "/bin/test_path");
        #endif
        // printf("<%s> == <%s>\n", vt_str_z(selfpath), vt_str_z(cwd));
        assert(vt_str_equals_z(vt_str_z(&selfpath.instance), vt_str_z(cwd)));
    }
    vt_str_destroy(cwd);
}

void test_path_pop_push(void) {
    vt_span_t span;

    // path_push
    char buffer[32] = {0};
    char test_cases_push[8][64] = {"", "hello", "world/", "this", "is", "", "me", "very_long_something_folder_directory"};
    size_t test_cases_push_len[8] = {0, 5, 12, 16, 19, 19, 22, 0};
    VT_FOREACH(i, 0, sizeof(test_cases_push)/sizeof(test_cases_push[0])) {
        span = vt_path_push(test_cases_push[i], buffer, sizeof(buffer)/sizeof(buffer[0]));
        printf("(%zu) [%s] ==> [%s]\n", i, test_cases_push[i], buffer);
        assert(vt_span_len(span) == test_cases_push_len[i]);
        if (i == sizeof(test_cases_push)/sizeof(test_cases_push[0]) - 1) assert(!vt_span_is_valid_object(span));
    }

    // path_pop
    char test_cases_pop[][2][32] = {
        {"hello/world", "hello"},
        {"hello/world/", "hello"},
        {"/hello/world/", "/hello"},
        {"world", "world"},
        {"world/", "world"},
        {"/world/", "/"},
        {"/world", "/"},
        {"/", "/"},
        {"/////", "/"},
        {"", ""},
    };
    VT_FOREACH(i, 0, sizeof(test_cases_pop)/sizeof(test_cases_pop[0])) {
        // printf("(%zu) [%s] ==> [%s]\n", i, test_cases_pop[i][0], test_cases_pop[i][1]);
        span = vt_path_pop(test_cases_pop[i][0]);
        assert(vt_span_is_valid_object(span) == (bool)strlen(test_cases_pop[i][1]));
        assert(vt_str_equals_z(test_cases_pop[i][0], test_cases_pop[i][1]));
    }
}

void test_path_realpath(void) {
    char buffer[VT_PATH_MAX] = {0};

    vt_span_t path = vt_path_get_realpath("./", buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(vt_span_is_valid_object(path));

    path = vt_path_basename(buffer, buffer, sizeof(buffer)/sizeof(buffer[0]));
    assert(vt_str_equals_z(buffer, "tests"));
}

void test_path_file_attr(void) {
    char buffer[VT_DATETIME_BUFFER_SIZE] = {0};

    // set access time
    struct utimbuf times = vt_path_get_file_times("src/test_math.c");
    times.actime = vt_datetime_to_secs(vt_datetime_from_text("2025-07-31 16:05:56"));
    vt_path_set_file_times("src/test_math.c", times);

    // get times
    times = vt_path_get_file_times("src/test_math.c");

    // modification time
    vt_datetime_to_text(
        vt_datetime_from_secs(times.modtime),
        buffer, sizeof(buffer)
    );
    assert(vt_str_equals_z(buffer, "2025-04-20 18:28:26"));


    // access time
    vt_datetime_to_text(
        vt_datetime_from_secs(times.actime),
        buffer, sizeof(buffer)
    );
    assert(vt_str_equals_z(buffer, "2025-07-31 16:05:56"));

    // copy file
    vt_path_copy_file("src/test_math.c", "src/test_math.c2");
    assert(vt_path_exists("src/test_math.c2"));
    assert(vt_path_get_file_size("src/test_math.c") == vt_path_get_file_size("src/test_math.c2"));
    vt_path_remove("src/test_math.c2");
}

