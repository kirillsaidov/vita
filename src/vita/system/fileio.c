#include "vita/system/fileio.h"

str_t *file_read(const char *const cs) {
    if(cs == NULL) {
        return NULL;
    }

    // open file
    FILE *fp = fopen(cs, "r");
    if(fp == NULL) {
        return NULL;
    }

    // create data buffer
    str_t *sbuffer = strn(path_get_file_size(cs));

    // copy file contents into the buffer
    fread(sbuffer->ptr, sizeof(unsigned char), sbuffer->len, fp);

    // close file
    fclose(fp);

    return sbuffer;
}

bool file_write(const char *const cs, const str_t *const s) {
    return true;
}

//bool file_writeln(const char *const cs, const str_t *const s) {}
//bool file_append(const char *const cs, const str_t *const s) {}









