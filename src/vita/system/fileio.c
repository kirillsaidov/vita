#include "vita/system/fileio.h"

str_t *file_read(const char *const z_filename) {
    if(z_filename == NULL) {
        DEBUG_ASSERT(z_filename != NULL, "Filename supplied is NULL!");
        return NULL;
    }

    // open file
    FILE *fp = 
    #if defined(_WIN32) || defined(_WIN64)
        fopen(z_filename, "rb");
    #else
        fopen(z_filename, "r");
    #endif
    if(fp == NULL) {
        DEBUG_ASSERT(fp != NULL, "Failed to open %s!", z_filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = path_get_file_size(z_filename);
    if(fsize < 0) {
        DEBUG_ASSERT(fsize >= 0, "Unable to fetch file size data!");

        fclose(fp);
        return NULL;
    }

    // create data buffer
    str_t *sbuffer = strn(fsize);
    if(sbuffer == NULL) {
        DEBUG_ASSERT(sbuffer != NULL, "Failed to create a str_t buffer!");

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(sbuffer->ptr, sbuffer->elsize, sbuffer->len, fp);
    if(bytes_read != sbuffer->len) {
        DEBUG_ASSERT(bytes_read == sbuffer->len, "Failed to read file data! Inconsistent: read(%zu) != file size (%zu) bytes", bytes_read, fsize);
        
        str_free(sbuffer);
        sbuffer = NULL;
    }

    // close file
    fclose(fp);

    return sbuffer;
}

str_t *file_readb(const char *const z_filename) {
    if(z_filename == NULL) {
        DEBUG_ASSERT(z_filename != NULL, "Filename supplied is NULL!");
        return NULL;
    }

    // open file
    FILE *fp = fopen(z_filename, "rb");
    if(fp == NULL) {
        DEBUG_ASSERT(fp != NULL, "Failed to open %s!", z_filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = path_get_file_size(z_filename);
    if(fsize < 0) {
        DEBUG_ASSERT(fsize >= 0, "Unable to fetch file size data!");

        fclose(fp);
        return NULL;
    }

    // create data buffer
    str_t *sbuffer = strn(fsize);
    if(sbuffer == NULL) {
        DEBUG_ASSERT(sbuffer != NULL, "Failed to create a str_t buffer!");

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(sbuffer->ptr, sbuffer->elsize, sbuffer->len, fp);
    if(bytes_read != sbuffer->len) {
        DEBUG_ASSERT(bytes_read == sbuffer->len, "Failed to read file data! Inconsistent: read(%zu) != file size (%zu) bytes", bytes_read, fsize);
        
        str_free(sbuffer);
        sbuffer = NULL;
    }

    // close file
    fclose(fp);

    return sbuffer;
}

bool file_write(const char *const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, false, false, false, "%s", zbuffer);
}

bool file_writeln(const char *const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, false, false, true, "%s", zbuffer);
}

bool file_writeb(const char *const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, true, false, false, "%s", zbuffer);
}

bool file_writebln(const char *const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, true, false, true, "%s", zbuffer);
}

bool file_append(const char *const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, false, true, false, "%s", zbuffer);
}

bool file_appendln(const char* const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, false, true, true, "%s", zbuffer);
}

bool file_appendb(const char *const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, true, true, false, "%s", zbuffer);
}

bool file_appendbln(const char* const z_filename, const char *const zbuffer) {
    return file_writefc(z_filename, true, true, true, "%s", zbuffer);
}

bool file_writef(const char *const z_filename, const char *const z_fmt, ...) {
    if(z_filename == NULL || z_fmt == NULL) {
        DEBUG_ASSERT(z_filename != NULL, "Filename supplied is NULL!");
        DEBUG_ASSERT(z_fmt != NULL, "Formatting supplied is NULL!");
        return false;
    }

    // open file
    FILE *fp = fopen(z_filename, "w");
    if(fp == NULL) {
        DEBUG_ASSERT(fp != NULL, "Failed to open %s!", z_filename);
        return false;
    }
    
    // write to file
    va_list args;
    va_start(args, z_fmt);
    vfprintf(fp, z_fmt, args);
    va_end(args);
    
    // close file
    fclose(fp); 

    return true;
}

bool file_writefc(const char *const z_filename, const bool use_binary_mode, const bool use_append_mode, const bool add_ln, const char *const z_fmt, ...) {
    if(z_filename == NULL || z_fmt == NULL) {
        DEBUG_ASSERT(z_filename != NULL, "Filename supplied is NULL!");
        DEBUG_ASSERT(z_fmt != NULL, "Formatting supplied is NULL!");
        return false;
    }

    // open file
    FILE *fp = fopen(z_filename, (use_binary_mode ? (use_append_mode ? "ab" : "wb") : (use_append_mode ? "a" : "w")));
    if(fp == NULL) {
        DEBUG_ASSERT(fp != NULL, "Failed to open %s!", z_filename);
        return false;
    }
    
    // write to file
    va_list args;
    va_start(args, z_fmt);
    vfprintf(fp, z_fmt, args);
    va_end(args);

    // add a new line
    if(add_ln) {
        fprintf(fp, "\n");
    }
    
    // close file
    fclose(fp); 

    return true;
}


