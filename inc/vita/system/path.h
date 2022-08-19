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
#include <dirent.h>
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
    z1 = raw c string
    z2 = raw c string

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build(str_t *const s, const char *const z1, const char *const z2);

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
    z = path

Returns: `true` if directory exists
*/
extern bool path_exists(const char *const z);

/**
Checks if path is a directory

Params:
    z = path directory

Returns: `true` if directory exists
*/
extern bool path_is_dir(const char *const z);

/**
Checks if path is a file

Params:
    z = path to file

Returns: `true` if file exists
*/
extern bool path_is_file(const char *const z);

/**
Returns file size

Params:
    z = path to file

Returns: size_t filesize >= 0 upon success, -1 upon failure
*/
extern int64_t path_get_file_size(const char *const z);

/**
Get all directory contents

Params:
    p = container where to save the data; if NULL is passed, it is allocated
    z = path
    ignoreDotFiles = skip hidden .files

Returns: `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir(plist_t *const p, const char *const z, const bool ignoreDotFiles);

/**
Get all files and sub-directories recursively

Params:
    p = container where to save the data; if NULL is passed, it is allocated
    z = path
    ignoreDotFiles = skip hidden .files

Returns: `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir_recurse(plist_t *const p, const char *const z, const bool ignoreDotFiles);

/**
Get path basename

Params:
    s = str_t instance (if `NULL` is passed, str_t is allocated)
    z = path

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_basename(str_t *const s, const char *const z);

/**
Creates a directory

Params:
    z = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_mkdir(const char *const z);

/**
Creates a directory and its parent directories

Params:
    z = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_mkdir_parents(const char *const z);

/**
Deletes an empty directory

Params:
    z = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_rmdir(const char *const z);

/**
Deletes an directory and its contents

Params:
    z = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_rmdir_recurse(const char *const z);

/**
Deletes a file

Params:
    z = a raw C string

Returns: `true` upon success, `false` otherwise
*/
extern bool path_remove(const char *const z);

/**
Rename/move a directory/file

Params:
    z1 = file/directory
    z2 = new file/directory

Returns: `true` upon success, `false` otherwise
*/
extern bool path_rename(const char *const z1, const char *const z2);

#endif // VITA_PATH_H
