#include <assert.h>

#define NDEBUG
#include "../../inc/vita/system/fileio.h"

int32_t main(void) {
    DEBUG_DEFAULT_INIT;

    #if defined(_WIN32) || defined(_WIN64)
        str_t *s = file_read("src\\test_file.txt");
        {
            assert(s != NULL);
            assert(str_len(s) == 21);
        }
        str_free(s);
        
        const str_t sbuf = str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        file_write("src\\test_file2.txt", &sbuf);
        file_writeln("src\\test_file3.txt", &sbuf);
        file_appendln("src\\test_file3.txt", &sbuf);
        file_append("src\\test_file3.txt", &sbuf);
        
        str_t sbuf_check = str_make_on_stack("hello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test");
        str_t *ss = file_read("src\\test_file3.txt"); 
        {
            str_remove_c(ss, "\r");
            assert(str_equals(cstr(ss), cstr(&sbuf_check)));
        }
        str_free(ss);
        
        file_writef(NULL, NULL, "%s, %s\n", "hello", "world");
        file_writef("src\\test_file4.txt", "w", "%s, %s\n", "hello", "world");
        assert(!file_writef(NULL, "r", "%s", "hi"));
        // assert(!file_writec("src\\test_file2.txt", "r", &sbuf, false));
    #else
        str_t *s = file_read("src/test_file.txt"); {
            if(s != NULL) {
                assert(str_len(s) == 18);
            }
        } str_free(s);

        const str_t sbuf = str_make_on_stack("hello, world\nthis is a new day\n12345 test");
        file_write("src/test_file2.txt", &sbuf);
        file_writeln("src/test_file3.txt", &sbuf);
        file_appendln("src/test_file3.txt", &sbuf);
        file_append("src/test_file3.txt", &sbuf);

        str_t sbuf_check = str_make_on_stack("hello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test\nhello, world\nthis is a new day\n12345 test");
        str_t *ss = file_read("src/test_file3.txt"); {
            assert(str_equals(cstr(ss), cstr(&sbuf_check)));
        } str_free(ss);

        file_writef(NULL, NULL, "%s, %s\n", "hello", "world");
        file_writef("src/test_file4.txt", "w", "%s, %s\n", "hello", "world");
        assert(!file_writef(NULL, "r", "%s", "hi"));
        // assert(!file_writec("src/test_file2.txt", "r", &sbuf, false));
    #endif
    
    DEBUG_DEFAULT_QUIT;
    return 0;
}


