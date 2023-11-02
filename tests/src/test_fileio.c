#include <assert.h>
#include "vita/system/fileio.h"

void test_filewrite(void);

int32_t main(void) {
    // tests
    test_filewrite();

    return 0;
}

void test_filewrite(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    #if defined(_WIN32) || defined(_WIN64)
        vt_str_t *s = vt_file_read("other\\test_file.txt", alloctr); {
            assert(s != NULL);
            assert(vt_str_len(s) == 18);
        } vt_str_destroy(s);

        const vt_str_t sbuf = vt_str_create_static("hello, world\nthis is a new day\n12345 test");
        vt_file_write("other\\test_file2.txt", vt_str_z(&sbuf));
        vt_file_writeln("other\\test_file3.txt", vt_str_z(&sbuf));
        vt_file_appendln("other\\test_file3.txt", vt_str_z(&sbuf));
        vt_file_append("other\\test_file3.txt", vt_str_z(&sbuf));

        vt_str_t sbuf_check = vt_str_create_static("hello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test");
        vt_str_t *ss = vt_file_read("other\\test_file3.txt", alloctr); {
            assert(vt_str_equals_z(vt_str_z(ss), vt_str_z(&sbuf_check)));
        } vt_str_destroy(ss);

        // vt_file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        vt_file_writef("other\\test_file4.txt", "%s, %s\n", "hello", "world");

        // read into buffer
        char buf[512] = {0};
        vt_file_read_to_buffer("other/test_file2.txt", buf, sizeof(buf));
        assert(vt_str_equals_z(buf, "hello, world\r\nthis is a new day\r\n12345 test")); 

    #else
        vt_str_t *s = vt_file_read("other/test_file.txt", alloctr); {
            assert(s != NULL);
            assert(vt_str_len(s) == 18);
        } vt_str_destroy(s);

        const vt_str_t sbuf = vt_str_create_static("hello, world\nthis is a new day\n12345 test");
        vt_file_write("other/test_file2.txt", vt_str_z(&sbuf));
        vt_file_writeln("other/test_file3.txt", vt_str_z(&sbuf));
        vt_file_appendln("other/test_file3.txt", vt_str_z(&sbuf));
        vt_file_append("other/test_file3.txt", vt_str_z(&sbuf));

        vt_str_t sbuf_check = vt_str_create_static("hello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test");
        vt_str_t *ss = vt_file_read("other/test_file3.txt", alloctr); {
            assert(vt_str_equals_z(vt_str_z(ss), vt_str_z(&sbuf_check)));
        } vt_str_destroy(ss);

        // vt_file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        vt_file_writef("other/test_file4.txt", "%s, %s\n", "hello", "world");

        // read into buffer
        char buf[512] = {0};
        vt_file_read_to_buffer("other/test_file2.txt", buf, sizeof(buf));
        assert(vt_str_equals_z(buf, "hello, world\nthis is a new day\n12345 test")); 
    #endif

    vt_mallocator_destroy(alloctr);
}


