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
    const size_t fsize = path_get_file_size(z_filename);
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
    const size_t fsize = path_get_file_size(z_filename);
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

bool file_writec(const char *const z_filename, const char *const z_mode, const str_t *const sbuffer, const bool add_ln) {
    if(z_filename == NULL || z_mode == NULL || !strnlen(z_mode, 3) || z_mode[0] == 'r' || sbuffer == NULL) {
        DEBUG_ASSERT(z_filename != NULL, "Filename supplied is NULL!");
        DEBUG_ASSERT(z_mode != NULL, "File write mode is NULL!");
        DEBUG_ASSERT(z_mode[0] != 'r', "Invalied file write mode supplied: \"%s\"!", z_mode);
        DEBUG_ASSERT(sbuffer != NULL, "Buffer supplied is NULL. Nothing to write to a file!");
        return false;
    }
    
    // open file
    FILE *fp = fopen(z_filename, z_mode);
    if(fp == NULL) {
        DEBUG_ASSERT(fp != NULL, "Failed to open %s!", z_filename);
        return false;
    }
    
    // write to file
    bool status = true;
    const size_t bytes_written = fwrite((const void*)(sbuffer->ptr), sbuffer->elsize, sbuffer->len, fp);
    if(bytes_written != sbuffer->len) {
        DEBUG_ASSERT(bytes_written == sbuffer->len, "Failed to write data to a file! Inconsistent: written(%zu) != buffer size (%zu) bytes", bytes_written, sbuffer->len);
        status = false;
    }

    // add new line
    if(status && add_ln) {
        fprintf(fp, "\n");
    }
    
    // close file
    fclose(fp);

    return status;
}

bool file_writef(const char *const z_filename, const char *const z_mode, const char *const z_fmt, ...) {
    if(z_fmt == NULL || z_mode == NULL || !strnlen(z_mode, 3) ||  z_mode[0] == 'r') {
        DEBUG_ASSERT(z_fmt != NULL, "Formatting supplied is NULL!");
        return false;
    }

    // open file
    FILE *fp = z_filename == NULL ? stderr : fopen(z_filename, (z_mode == NULL || z_mode[0] == 'r') ? "w" : z_mode);
    if(fp == NULL) {
        DEBUG_ASSERT(fp != NULL, "Failed to open %s!", z_filename);
        return false;
    }
    
    // write to file
    bool status = true;
    va_list args;
    va_start(args, z_fmt);
    vfprintf(fp, z_fmt, args);
    va_end(args);
    
    // close file
    if(fp != stderr) {
        fclose(fp); 
    }

    return status;
}

/* -------------------- SPECIALIZED FUNCTIONS -------------------- */

bool file_write(const char *const z_filename, const str_t *const sbuffer) {
    return file_writec(z_filename, "w", sbuffer, false);
}

bool file_writeln(const char *const z_filename, const str_t *const sbuffer) {
    return file_writec(z_filename, "w", sbuffer, true);
}

bool file_append(const char *const z_filename, const str_t *const sbuffer) {
    return file_writec(z_filename, "a", sbuffer, false);
}

bool file_appendln(const char* const z_filename, const str_t *const sbuffer) {
    return file_writec(z_filename, "a", sbuffer, true);
}
















