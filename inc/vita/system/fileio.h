#ifndef VITA_FILEIO_H
#define VITA_FILEIO_H

/** VITA_FILEIO MODULE (working with files)
    - file_read
    - file_readb
    - file_write
    - file_writeln
    - file_writeb
    - file_writebln
    - file_append
    - file_appendln
    - file_appendb
    - file_appendbln
    - file_writef
    - file_writefc
*/

#include <stdarg.h>
#include "path.h"
#include "../util/debug.h"

/** Reads contents of a file in 'r' and 'rb' mode (file_read, file_readb)
    @param z_filename file name
    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *file_read(const char *const z_filename);
extern str_t *file_readb(const char *const z_filename);

/** Writes to a file in 'w' mode (file_write, file_writeln)
    @param z_filename file name
    @param zbuffer contents to write to file

    @returns `true` upon success
*/
extern bool file_write(const char *const z_filename, const char *const zbuffer);
extern bool file_writeln(const char *const z_filename, const char *const zbuffer);

/** Writes to a file in binary 'wb' mode (file_writeb, file_writebln)
    @param z_filename file name
    @param zbuffer contents to write to file
    @param add_ln add new line at the end

    @returns `true` upon success
*/
extern bool file_writeb(const char *const z_filename, const char *const zbuffer);
extern bool file_writebln(const char *const z_filename, const char *const zbuffer);

/** Appends to a file in 'a' mode (file_append, file_appendln)
    @param z_filename file name
    @param zbuffer contents to write to file

    @returns `true` upon success
*/
extern bool file_append(const char *const z_filename, const char *const zbuffer);
extern bool file_appendln(const char* const z_filename, const char *const zbuffer);

/** Appends to a file in bnary 'ab' mode (file_appendb, file_appendbln)
    @param z_filename file name
    @param zbuffer contents to write to file

    @returns `true` upon success
*/
extern bool file_appendb(const char *const z_filename, const char *const zbuffer);
extern bool file_appendbln(const char* const z_filename, const char *const zbuffer);


/** Writes to a file using a formatted string in 'w' mode
    @param z_filename file name 
    @param z_fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool file_writef(const char *const z_filename, const char *const z_fmt, ...);

/** Custom writer to file using a formatted string
    @param z_filename file name
    @param use_binary_mode write in binary mode
    @param use_append_mode append to file
    @param add_ln add a new line
    @param z_fmt formatted string
    @param ... other variable arguments

    @returns `true` upon success
*/
extern bool file_writefc(const char *const z_filename, const bool use_binary_mode, const bool use_append_mode, const bool add_ln, const char *const z_fmt, ...);

#endif // VITA_FILEIO_H

