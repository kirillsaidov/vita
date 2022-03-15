#ifndef VITA_FILEIO_H
#define VITA_FILEIO_H

/** VITA_FILEIO MODULE (working with files)
    - file_read *
    - file_write *
    - file_writeln *
*/

#include "path.h"

/**
Reads contents of a file

Params:
    cs = file name

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *file_read(const char *const cs);

/**
Writes to a file

Params:
    cs = file name
    s = contents to write to file

Returns: `true` upon success
*/
extern bool file_write(const char *const cs, const str_t *const s);
extern bool file_writeln(const char *const cs, const str_t *const s);

/**
Appends to a file

Params:
    cs = file name
    s = contents to write to file

Returns: `true` upon success
*/
extern bool file_append(const char *const cs, const str_t *const s);

#endif // VITA_FILEIO_H

