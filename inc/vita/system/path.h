#ifndef VITA_PATH_H
#define VITA_PATH_H

/** VITA_PATH MODULE
    - path_build
    - path_build_n
    - path_getcwd
    - path_exists
    - path_is_dir
    - path_is_file
    - path_get_file_size
    - path_listdir
    - path_listdir_recurse
    - path_basename
    - path_mkdir
    - path_mkdir_parents
    - path_rmdir
    - path_rmdir_recurse
    - path_remove
    - path_rename
*/

#include "../core/core.h"
#include "../container/str.h"
#include "../container/plist.h"

#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #include <direct.h>
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
    #define getcwd _getcwd
    #define mkdir CreateDirectory
    #define rmdir RemoveDirectory
    #define rename MoveFile
    #define DIR_PERMISSIONS NULL
#else
    #include <unistd.h>
    #include <dirent.h>
    #include <libgen.h>
    #define PATH_SEPARATOR "/"
    #define DIR_PERMISSIONS (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

/**
Builds path from raw C strings

Params:
    s = str_t instance (if `NULL` is passed, str_t is allocated)
    cs1 = raw c string
    cs2 = raw c string

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build(str_t *const s, const char *const cs1, const char *const cs2);

/**
Builds path from raw C strings

Params:
    s = str_t instance (if `NULL` is passed, str_t is allocated)
    p = array of raw C strings

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build_n(str_t *const s, const plist_t *const p);

/**
Get current working directory

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_getcwd();

/**
Checks if path exists

Params:
    cs = path

Returns: `true` if directory exists
*/
extern bool path_exists(const char *const cs);

/**
Checks if path is a directory

Params:
    cs = path directory

Returns: `true` if directory exists
*/
extern bool path_is_dir(const char *const cs);

/**
Checks if path is a file

Params:
    cs = path to file

Returns: `true` if file exists
*/
extern bool path_is_file(const char *const cs);

/**
Returns file size

Params:
    cs = path to file

Returns: size_t filesize >= 0 upon success, -1 upon failure
*/
extern int64_t path_get_file_size(const char *const cs);

/**
Get all directory contents

Params:
    p = container where to save the data; if NULL is passed, it is allocated
    cs = path
    ignoreDotFiles = skip hidden .files

Returns: `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir(plist_t *const p, const char *const cs, const bool ignoreDotFiles);

/**
Get all files and sub-directories recursively

Params:
    p = container where to save the data; if NULL is passed, it is allocated
    cs = path
    ignoreDotFiles = skip hidden .files

Returns: `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir_recurse(plist_t *const p, const char *const cs, const bool ignoreDotFiles);

/**
Get path basename

Params:
    s = str_t instance (if `NULL` is passed, str_t is allocated)
    cs = path

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_basename(str_t *const s, const char *const cs);

/**
Creates a directory

Params:
    cs = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_mkdir(const char *const cs);

/**
Creates a directory and its parent directories

Params:
    cs = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_mkdir_parents(const char *const cs);

/**
Deletes an empty directory

Params:
    cs = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_rmdir(const char *const cs);

/**
Deletes an directory and its contents

Params:
    cs = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_rmdir_recurse(const char *const cs);

/**
Deletes a file

Params:
    cs = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_remove(const char *const cs);

/**
Rename/move a directory/file

Params:
    cs1 = file/directory
    cs2 = new file/directory

Returns: `true` upon success, `false` otherwise
*/
extern bool path_rename(const char *const cs1, const char *const cs2);

#endif // VITA_PATH_H
