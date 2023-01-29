#include "vita/system/fileio.h"

str_t *vt_file_read(const char *const z_filename) {
    // check for invalid input
    VT_DEBUG_ASSERT(z_filename != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // open file
    FILE *fp = 
    #if defined(_WIN32) || defined(_WIN64)
        fopen(z_filename, "rb");
    #else
        fopen(z_filename, "r");
    #endif
    if(fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z_filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = vt_path_get_file_size(z_filename);
    if(fsize < 0) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));

        fclose(fp);
        return NULL;
    }

    // create data buffer
    str_t *sbuffer = vt_strn(fsize);
    if(sbuffer == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(sbuffer->ptr, sbuffer->elsize, sbuffer->len, fp);
    if(bytes_read != sbuffer->len) {
        VT_DEBUG_PRINTF(
            "%s: Failed to read file data! Inconsistent read (%zu) != file size (%zu) bytes\n", 
            vt_get_vita_error_str(vt_ve_operation_failure),
            bytes_read,
            fsize
        );
        vt_str_free(sbuffer);
        sbuffer = NULL;
    }

    // close file
    fclose(fp);

    return sbuffer;
}

str_t *vt_file_readb(const char *const z_filename) {
    // check for invalid input
    VT_DEBUG_ASSERT(z_filename != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // open file
    FILE *fp = fopen(z_filename, "rb");
    if(fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z_filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = vt_path_get_file_size(z_filename);
    if(fsize < 0) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));

        fclose(fp);
        return NULL;
    }

    // create data buffer
    str_t *sbuffer = vt_strn(fsize);
    if(sbuffer == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(sbuffer->ptr, sbuffer->elsize, sbuffer->len, fp);
    if(bytes_read != sbuffer->len) {
        VT_DEBUG_PRINTF(
            "%s: Failed to read file data! Inconsistent read (%zu) != file size (%zu) bytes\n", 
            vt_get_vita_error_str(vt_ve_operation_failure),
            bytes_read,
            fsize
        );        
        vt_str_free(sbuffer);
        sbuffer = NULL;
    }

    // close file
    fclose(fp);

    return sbuffer;
}

bool vt_file_write(const char *const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, false, false, false, "%s", z_buffer);
}

bool vt_file_writeln(const char *const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, false, false, true, "%s", z_buffer);
}

bool vt_file_writeb(const char *const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, true, false, false, "%s", z_buffer);
}

bool vt_file_writebln(const char *const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, true, false, true, "%s", z_buffer);
}

bool vt_file_append(const char *const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, false, true, false, "%s", z_buffer);
}

bool vt_file_appendln(const char* const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, false, true, true, "%s", z_buffer);
}

bool vt_file_appendb(const char *const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, true, true, false, "%s", z_buffer);
}

bool vt_file_appendbln(const char* const z_filename, const char *const z_buffer) {
    return vt_file_writefc(z_filename, true, true, true, "%s", z_buffer);
}

bool vt_file_writef(const char *const z_filename, const char *const z_fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(z_filename != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(z_fmt != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // open file
    FILE *fp = fopen(z_filename, "w");
    if(fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z_filename);
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

bool vt_file_writefc(const char *const z_filename, const bool use_binary_mode, const bool use_append_mode, const bool add_ln, const char *const z_fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(z_filename != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(z_fmt != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // open file
    FILE *fp = fopen(z_filename, (use_binary_mode ? (use_append_mode ? "ab" : "wb") : (use_append_mode ? "a" : "w")));
    if(fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z_filename);
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


