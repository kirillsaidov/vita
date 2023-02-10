#include <assert.h>
#include "../../inc/vita/system/fileio.h"

void test_filewrite(void);

int32_t main(void) {
    DEBUG_DEFAULT_INIT();

    // tests
    test_filewrite();
    
    DEBUG_DEFAULT_QUIT();
    return 0;
}

void test_filewrite(void) {
    #if defined(_WIN32) || defined(_WIN64)
        vt_str_t *s = file_read("other\\test_file.txt"); {
            if(s != NULL) {
                assert(str_len(s) == 21);
            }
        } str_free(s);

        const vt_str_t sbuf = str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        file_write("other\\test_file2.txt", vt_cstr(&sbuf));
        file_writeln("other\\test_file3.txt", vt_cstr(&sbuf));
        file_appendln("other\\test_file3.txt", vt_cstr(&sbuf));
        file_append("other\\test_file3.txt", vt_cstr(&sbuf));

        vt_str_t sbuf_check = str_make_on_stack("hello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test");
        vt_str_t *ss = file_read("other\\test_file3.txt"); {
            assert(str_equals(vt_cstr(ss), vt_cstr(&sbuf_check)));
        } str_free(ss);

        // file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        file_writef("other\\test_file4.txt", "%s, %s\n", "hello", "world");
    #else
        vt_str_t *s = file_read("other/test_file.txt"); {
            if(s != NULL) {
                assert(str_len(s) == 18);
            }
        } str_free(s);

        const vt_str_t sbuf = str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        file_write("other/test_file2.txt", vt_cstr(&sbuf));
        file_writeln("other/test_file3.txt", vt_cstr(&sbuf));
        file_appendln("other/test_file3.txt", vt_cstr(&sbuf));
        file_append("other/test_file3.txt", vt_cstr(&sbuf));

        vt_str_t sbuf_check = str_make_on_stack("hello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test");
        vt_str_t *ss = file_read("other/test_file3.txt"); {
            assert(str_equals(vt_cstr(ss), vt_cstr(&sbuf_check)));
        } str_free(ss);

        // file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        file_writef("other/test_file4.txt", "%s, %s\n", "hello", "world");
    #endif
}


