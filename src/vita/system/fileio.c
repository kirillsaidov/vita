#include "vita/system/fileio.h"

vt_str_t *vt_file_read(const char *const filename, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open file
    FILE *fp = 
    #if defined(_WIN32) || defined(_WIN64)
        fopen(filename, "rb");
    #else
        fopen(filename, "r");
    #endif
    if (fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = vt_path_get_file_size(filename);
    if (fsize < 0) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));

        fclose(fp);
        return NULL;
    }

    // create data buffer
    vt_str_t *sbuffer = vt_str_create_len(fsize, alloctr);
    if (sbuffer == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(sbuffer->ptr, sbuffer->elsize, sbuffer->len, fp);
    if (bytes_read != sbuffer->len) {
        VT_DEBUG_PRINTF(
            "%s: Failed to read file data! Inconsistent read (%zu) != file size (%zu) bytes\n", 
            vt_status_to_str(VT_STATUS_OPERATION_FAILURE),
            bytes_read,
            fsize
        );
        vt_str_destroy(sbuffer);
        sbuffer = NULL;
    }

    // close file
    fclose(fp);

    return sbuffer;
}

vt_str_t *vt_file_readb(const char *const filename, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open file
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = vt_path_get_file_size(filename);
    if (fsize < 0) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));

        fclose(fp);
        return NULL;
    }

    // create data buffer
    vt_str_t *sbuffer = vt_str_create_len(fsize, alloctr);
    if (sbuffer == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(sbuffer->ptr, sbuffer->elsize, sbuffer->len, fp);
    if (bytes_read != sbuffer->len) {
        VT_DEBUG_PRINTF(
            "%s: Failed to read file data! Inconsistent read (%zu) != file size (%zu) bytes\n", 
            vt_status_to_str(VT_STATUS_OPERATION_FAILURE),
            bytes_read,
            fsize
        );        
        vt_str_destroy(sbuffer);
        sbuffer = NULL;
    }

    // close file
    fclose(fp);

    return sbuffer;
}

const char *vt_file_read_to_buffer(const char *const filename, char *const buffer, const size_t buffer_len) {
    // check for invalid input
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(buffer_len > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open file
    FILE *fp = 
    #if defined(_WIN32) || defined(_WIN64)
        fopen(filename, "rb");
    #else
        fopen(filename, "r");
    #endif
    if (fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), filename);
        return NULL;
    }

    // get file size
    const int64_t fsize = vt_path_get_file_size(filename);
    if (fsize < 0) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));

        fclose(fp);
        return NULL;
    }

    // copy file contents into the buffer
    const size_t bytes_read = fread(buffer, sizeof(char), buffer_len <= (size_t)fsize ? buffer_len : (size_t)fsize, fp);
    if (bytes_read != (buffer_len <= (size_t)fsize ? buffer_len : (size_t)fsize)) {
        VT_DEBUG_PRINTF(
            "%s: Failed to read file data! Inconsistent read (%zu) != file size (%zu) bytes\n", 
            vt_status_to_str(VT_STATUS_OPERATION_FAILURE),
            bytes_read,
            buffer_len <= (size_t)fsize ? buffer_len : (size_t)fsize
        );
        fclose(fp);
        return NULL;
    }

    // close file
    fclose(fp);

    return buffer;
}

bool vt_file_write(const char *const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, false, false, false, "%s", buffer);
}

bool vt_file_writeln(const char *const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, false, false, true, "%s", buffer);
}

bool vt_file_writeb(const char *const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, true, false, false, "%s", buffer);
}

bool vt_file_writebln(const char *const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, true, false, true, "%s", buffer);
}

bool vt_file_append(const char *const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, false, true, false, "%s", buffer);
}

bool vt_file_appendln(const char* const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, false, true, true, "%s", buffer);
}

bool vt_file_appendb(const char *const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, true, true, false, "%s", buffer);
}

bool vt_file_appendbln(const char* const filename, const char *const buffer) {
    // check for invalid input
    VT_DEBUG_ASSERT(buffer != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_file_writefc(filename, true, true, true, "%s", buffer);
}

bool vt_file_writef(const char *const filename, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open file
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), filename);
        return false;
    }
    
    // write to file
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    
    // close file
    fclose(fp); 

    return true;
}

bool vt_file_writefln(const char *const filename, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open file
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), filename);
        return false;
    }
    
    // write to file
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    // add a new line
    fprintf(fp, "\n");
    
    // close file
    fclose(fp); 

    return true;
}

bool vt_file_writefc(const char *const filename, const bool use_binary_mode, const bool use_append_mode, const bool add_ln, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open file
    FILE *fp = fopen(filename, (use_binary_mode ? (use_append_mode ? "ab" : "wb") : (use_append_mode ? "a" : "w")));
    if (fp == NULL) {
        VT_DEBUG_PRINTF("%s: Failed to open <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), filename);
        return false;
    }
    
    // write to file
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    // add a new line
    if (add_ln) {
        fprintf(fp, "\n");
    }
    
    // close file
    fclose(fp); 

    return true;
}


