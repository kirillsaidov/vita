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
    - path_expand_tilda
    - path_get_this_exe_location
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

/** Builds path from raw C strings
    @param s str_t instance (if `NULL` is passed, str_t is allocated)
    @param n number of strings to concatenate into path
    @param ... c strings to combine into path

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build(str_t *const s, const size_t n, ...);

/** Builds path from raw C strings
    @param s str_t instance (if `NULL` is passed, str_t is allocated)
    @param p array of raw C strings

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_build_n(str_t *const s, const plist_t *const p);

/** Get current working directory
    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_getcwd(void);

/** Checks if path exists
    @param z path
    @returns `true` if directory exists
*/
extern bool path_exists(const char *const z);

/** Checks if path is a directory
    @param z path directory
    @returns `true` if directory exists
*/
extern bool path_is_dir(const char *const z);

/** Checks if path is a file
    @param z path to file
    @returns `true` if file exists
*/
extern bool path_is_file(const char *const z);

/** Returns file size
    @param z path to file
    @returns size_t filesize >= 0 upon success, -1 upon failure
*/
extern int64_t path_get_file_size(const char *const z);

/** Get all directory contents
    @param p container where to save the data; if NULL is passed, it is allocated
    @param z path
    @param ignoreDotFiles skip hidden .files

    @returns `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir(plist_t *const p, const char *const z, const bool ignoreDotFiles);

/** Get all files and sub-directories recursively
    @param p container where to save the data; if NULL is passed, it is allocated
    @param z path
    @param ignoreDotFiles skip hidden .files

    @returns `plist_t*` of str_t upon success, `NULL` otherwise
*/
extern plist_t *path_listdir_recurse(plist_t *const p, const char *const z, const bool ignoreDotFiles);

/** Get path basename
    @param s str_t instance (if `NULL` is passed, str_t is allocated)
    @param z path

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_basename(str_t *const s, const char *const z);

/** Creates a directory
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool path_mkdir(const char *const z);

/** Creates a directory and its parent directories
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool path_mkdir_parents(const char *const z);

/** Deletes an empty directory
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool path_rmdir(const char *const z);

/** Deletes an directory and its contents
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool path_rmdir_recurse(const char *const z);

/** Deletes a file
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool path_remove(const char *const z);

/** Rename/move a directory/file
    @param z1 file/directory
    @param z2 new file/directory

    @returns `true` upon success, `false` otherwise
*/
extern bool path_rename(const char *const z1, const char *const z2);

/** Expands tilda `~` to HOMEPATH
    @param z1 path
    @returns a newly allocated `str_t` path with the expanded tilda `~` upon success, `NULL` otherwise
*/
extern str_t *path_expand_tilda(const char *const z);

/** Returns executable path
    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *path_get_this_exe_location(void);

#endif // VITA_PATH_H
