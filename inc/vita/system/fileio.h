#ifndef VITA_FILEIO_H
#define VITA_FILEIO_H

/** VITA_FILEIO MODULE (working with files)
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

/** Reads contents of a file in 'r' and 'rb' mode (vt_file_read, vt_file_readb)
    @param z_filename file name
    @returns `vt_str_t*` upon success, `NULL` otherwise
*/
extern vt_str_t *vt_file_read(const char *const z_filename);
extern vt_str_t *vt_file_readb(const char *const z_filename);

/** Writes to a file in 'w' mode (vt_file_write, vt_file_writeln)
    @param z_filename file name
    @param z_buffer contents to write to file

    @returns `true` upon success
*/
extern bool vt_file_write(const char *const z_filename, const char *const z_buffer);
extern bool vt_file_writeln(const char *const z_filename, const char *const z_buffer);

/** Writes to a file in binary 'wb' mode (vt_file_writeb, vt_file_writebln)
    @param z_filename file name
    @param z_buffer contents to write to file
    @param add_ln add new line at the end

    @returns `true` upon success
*/
extern bool vt_file_writeb(const char *const z_filename, const char *const z_buffer);
extern bool vt_file_writebln(const char *const z_filename, const char *const z_buffer);

/** Appends to a file in 'a' mode (vt_file_append, vt_file_appendln)
    @param z_filename file name
    @param z_buffer contents to write to file

    @returns `true` upon success
*/
extern bool vt_file_append(const char *const z_filename, const char *const z_buffer);
extern bool vt_file_appendln(const char* const z_filename, const char *const z_buffer);

/** Appends to a file in bnary 'ab' mode (vt_file_appendb, vt_file_appendbln)
    @param z_filename file name
    @param z_buffer contents to write to file

    @returns `true` upon success
*/
extern bool vt_file_appendb(const char *const z_filename, const char *const z_buffer);
extern bool vt_file_appendbln(const char* const z_filename, const char *const z_buffer);


/** Writes to a file using a formatted string in 'w' mode
    @param z_filename file name 
    @param z_fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool vt_file_writef(const char *const z_filename, const char *const z_fmt, ...);

/** Custom writer to file using a formatted string
    @param z_filename file name
    @param use_binary_mode write in binary mode
    @param use_append_mode append to file
    @param add_ln add a new line
    @param z_fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool vt_file_writefc(const char *const z_filename, const bool use_binary_mode, const bool use_append_mode, const bool add_ln, const char *const z_fmt, ...);

#endif // VITA_FILEIO_H

