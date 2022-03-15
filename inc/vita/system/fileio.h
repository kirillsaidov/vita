#ifndef VITA_FILEIO_H
#define VITA_FILEIO_H

/** VITA_FILEIO MODULE (working with files)
    - file_read
    - file_writec (custom mode)
    - file_write
    - file_writeln
    - file_append
    - file_appendln
*/

#include "path.h"

/**
Reads contents of a file

Params:
    cs_filename = file name

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *file_read(const char *const cs_filename);

/**
Writes to a file in custom mode

Params:
    cs_filename = file name
    cs_mode = file open mode
    sbuffer = contents to write to file
    add_ln = add new line at the end

Returns: `true` upon success
*/
extern bool file_writec(const char *const cs_filename, const char *const cs_mode, const str_t *const sbuffer, const bool add_ln);

/**
Writes to a file

Params:
    cs_filename = file name
    sbuffer = contents to write to file

Returns: `true` upon success
*/
extern bool file_write(const char *const cs_filename, const str_t *const sbuffer);
extern bool file_writeln(const char *const cs_filename, const str_t *const sbuffer);

/**
Appends to a file

Params:
    cs_filename = file name
    sbuffer = contents to write to file

Returns: `true` upon success
*/
extern bool file_append(const char *const cs_filename, const str_t *const sbuffer);
extern bool file_appendln(const char* const cs_filename, const str_t *const sbuffer);

#endif // VITA_FILEIO_H

