#include <assert.h>

#include "../../inc/vita/system/fileio.h"

int main(void) {
    str_t *s = file_read("src/test_file.txt"); {
        assert(str_len(s) == 18);
    } str_free(s);

    return 0;
}


