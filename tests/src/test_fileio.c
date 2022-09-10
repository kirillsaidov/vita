#include <assert.h>
#include "../../inc/vita/system/fileio.h"

void test_filewrite(void);

int32_t main(void) {
    DEBUG_DEFAULT_INIT;

    // tests
    test_filewrite();
    
    DEBUG_DEFAULT_QUIT;
    return 0;
}

void test_filewrite(void) {
    #if defined(_WIN32) || defined(_WIN64)
        str_t *s = file_read("src\\test_file.txt"); {
            if(s != NULL) {
                assert(str_len(s) == 21);
            }
        } str_free(s);

        const str_t sbuf = str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        file_write("src\\test_file2.txt", cstr(&sbuf));
        file_writeln("src\\test_file3.txt", cstr(&sbuf));
        file_appendln("src\\test_file3.txt", cstr(&sbuf));
        file_append("src\\test_file3.txt", cstr(&sbuf));

        str_t sbuf_check = str_make_on_stack("hello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test\r\nhello, world\r\nthis is a new day\r\n12345 test");
        str_t *ss = file_read("src\\test_file3.txt"); {
            assert(str_equals(cstr(ss), cstr(&sbuf_check)));
        } str_free(ss);

        // file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        file_writef("src\\test_file4.txt", "%s, %s\n", "hello", "world");
    #else
        str_t *s = file_read("src/test_file.txt"); {
            if(s != NULL) {
                assert(str_len(s) == 18);
            }
        } str_free(s);

        const str_t sbuf = str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        file_write("src/test_file2.txt", cstr(&sbuf));
        file_writeln("src/test_file3.txt", cstr(&sbuf));
        file_appendln("src/test_file3.txt", cstr(&sbuf));
        file_append("src/test_file3.txt", cstr(&sbuf));

        str_t sbuf_check = str_make_on_stack("hello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test");
        str_t *ss = file_read("src/test_file3.txt"); {
            assert(str_equals(cstr(ss), cstr(&sbuf_check)));
        } str_free(ss);

        // file_writef(NULL, "%s, %s\n", "hello", "world"); // error, filename is NULL
        file_writef("src/test_file4.txt", "%s, %s\n", "hello", "world");
    #endif
}


