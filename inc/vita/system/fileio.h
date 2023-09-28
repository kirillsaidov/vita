#ifndef VITA_SYSTEM_FILEIO_H
#define VITA_SYSTEM_FILEIO_H

/** FILEIO MODULE (working with files)
    - vt_file_read
    - vt_file_readb
    - vt_file_write
    - vt_file_writeln
    - vt_file_writeb
    - vt_file_writebln
    - vt_file_append
    - vt_file_appendln
    - vt_file_appendb
    - vt_file_appendbln
    - vt_file_writef
    - vt_file_writefc
*/

#include <stdarg.h>
#include "path.h"
#include "../util/debug.h"

/** Reads contents of a file in 'r' mode
    @param filename file name
    @param alloctr allocator instance
    @returns `vt_str_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_file_read(const char *const filename, struct VitaBaseAllocatorType *const alloctr);

/** Reads contents of a file in binary 'rb' mode
    @param filename file name
    @param alloctr allocator instance
    @returns `vt_str_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_file_readb(const char *const filename, struct VitaBaseAllocatorType *const alloctr);

/** Reads contents of a file in 'r' mode to the specified buffer
    @param filename file name
    @param buffer a valid pointer to buffer
    @param buffer_len buffer length

    @returns `char*` pointer to the specified buffer upon success, `NULL` otherwise
*/
extern const char *vt_file_read_to_buffer(const char *const filename, char *const buffer, const size_t buffer_len);

/** Writes to a file in 'w' mode
    @param filename file name
    @param buffer contents to write to file
    @returns `true` upon success
*/
extern bool vt_file_write(const char *const filename, const char *const buffer);

/** Writes to a file in 'w' mode and appends a new line at the end
    @param filename file name
    @param buffer contents to write to file
    @returns `true` upon success
*/
extern bool vt_file_writeln(const char *const filename, const char *const buffer);

/** Writes to a file in binary 'wb' mode
    @param filename file name
    @param buffer contents to write to file
    @param add_ln add new line at the end

    @returns `true` upon success
*/
extern bool vt_file_writeb(const char *const filename, const char *const buffer);

/** Writes to a file in binary 'wb' mode and appends a new line at the end
    @param filename file name
    @param buffer contents to write to file
    @param add_ln add new line at the end

    @returns `true` upon success
*/
extern bool vt_file_writebln(const char *const filename, const char *const buffer);

/** Appends to a file in 'a' mode 
    @param filename file name
    @param buffer contents to write to file
    @returns `true` upon success
*/
extern bool vt_file_append(const char *const filename, const char *const buffer);

/** Appends to a file in 'a' mode and appends a new line at the end
    @param filename file name
    @param buffer contents to write to file
    @returns `true` upon success
*/
extern bool vt_file_appendln(const char* const filename, const char *const buffer);

/** Appends to a file in binary 'ab' mode 
    @param filename file name
    @param buffer contents to write to file
    @returns `true` upon success
*/
extern bool vt_file_appendb(const char *const filename, const char *const buffer);

/** Appends to a file in binary 'ab' mode and appends a new line at the end
    @param filename file name
    @param buffer contents to write to file
    @returns `true` upon success
*/
extern bool vt_file_appendbln(const char* const filename, const char *const buffer);


/** Writes to a file using a formatted string in 'w' mode
    @param filename file name 
    @param fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool vt_file_writef(const char *const filename, const char *const fmt, ...);

/** Writes to a file using a formatted string in 'w' mode and appends a new line at the end
    @param filename file name 
    @param fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool vt_file_writefln(const char *const filename, const char *const fmt, ...);

/** Custom writer to file using a formatted string
    @param filename file name
    @param use_binary_mode write in binary mode
    @param use_append_mode append to file
    @param add_ln add a new line
    @param fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool vt_file_writefc(const char *const filename, const bool use_binary_mode, const bool use_append_mode, const bool add_ln, const char *const fmt, ...);

#endif // VITA_SYSTEM_FILEIO_H

