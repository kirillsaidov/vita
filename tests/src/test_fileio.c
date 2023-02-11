#include <assert.h>
#include "../../inc/vita/system/fileio.h"

void test_filewrite(void);

int32_t main(void) {
    VT_DEBUG_DEFAULT_INIT();

    // tests
    test_filewrite();
    
    VT_DEBUG_DEFAULT_QUIT();
    return 0;
}

void test_filewrite(void) {
    #if defined(_WIN32) || defined(_WIN64)
        vt_str_t *s = vt_file_read("other\\test_file.txt"); {
            if(s != NULL) {
                assert(vt_str_len(s) == 21);
            }
        } vt_str_free(s);

        const vt_str_t sbuf = vt_str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        vt_file_write("other\\test_file2.txt", vt_cstr(&sbuf));
        vt_file_writeln("other\\test_file3.txt", vt_cstr(&sbuf));
        vt_file_appendln("other\\test_file3.txt", vt_cstr(&sbuf));
        vt_file_append("other\\test_file3.txt", vt_cstr(&sbuf));

        vt_str_t sbuf_check = vt_str_make_on_stack("hello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test");
        vt_str_t *ss = vt_file_read("other\\test_file3.txt"); {
            assert(vt_str_equals(vt_cstr(ss), vt_cstr(&sbuf_check)));
        } vt_str_free(ss);

        // vt_file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        vt_file_writef("other\\test_file4.txt", "%s, %s\n", "hello", "world");
    #else
        vt_str_t *s = vt_file_read("other/test_file.txt"); {
            if(s != NULL) {
                assert(vt_str_len(s) == 18);
            }
        } vt_str_free(s);

        const vt_str_t sbuf = vt_str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        vt_file_write("other/test_file2.txt", vt_cstr(&sbuf));
        vt_file_writeln("other/test_file3.txt", vt_cstr(&sbuf));
        vt_file_appendln("other/test_file3.txt", vt_cstr(&sbuf));
        vt_file_append("other/test_file3.txt", vt_cstr(&sbuf));

        vt_str_t sbuf_check = vt_str_make_on_stack("hello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test");
        vt_str_t *ss = vt_file_read("other/test_file3.txt"); {
            assert(vt_str_equals(vt_cstr(ss), vt_cstr(&sbuf_check)));
        } vt_str_free(ss);

        // vt_file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        vt_file_writef("other/test_file4.txt", "%s, %s\n", "hello", "world");
    #endif
}


