#ifndef VITA_PATH_H
#define VITA_PATH_H

/** VITA_PATH MODULE
    - vt_path_build
    - vt_path_build_n
    - vt_path_getcwd
    - vt_path_exists
    - vt_path_is_dir
    - vt_path_is_file
    - vt_path_get_file_size
    - vt_path_listdir
    - vt_path_listdir_recurse
    - vt_path_basename
    - vt_path_mkdir
    - vt_path_mkdir_parents
    - vt_path_rmdir
    - vt_path_rmdir_recurse
    - vt_path_remove
    - vt_path_rename
    - vt_path_expand_tilda
    - vt_path_get_this_exe_location
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
    @param p array of raw C strings

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *vt_path_build(str_t *const s, const plist_t *const p);

/** Builds path from raw C strings
    @param s str_t instance (if `NULL` is passed, str_t is allocated)
    @param n number of strings to concatenate into path
    @param ... c strings to combine into path

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *vt_path_build_n(str_t *const s, const size_t n, ...);

/** Get current working directory
    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *vt_path_getcwd(void);

/** Checks if path exists
    @param z path
    @returns `true` if directory exists
*/
extern bool vt_path_exists(const char *const z);

/** Checks if path is a directory
    @param z path directory
    @returns `true` if directory exists
*/
extern bool vt_path_is_dir(const char *const z);

/** Checks if path is a file
    @param z path to file
    @returns `true` if file exists
*/
extern bool vt_path_is_file(const char *const z);

/** Returns file size
    @param z path to file
    @returns size_t filesize >= 0 upon success, -1 upon failure
*/
extern int64_t vt_path_get_file_size(const char *const z);

/** Get all directory contents
    @param p container where to save the data; if NULL is passed, it is allocated
    @param z path
    @param ignoreDotFiles skip hidden .files

    @returns `plist_t*` of str_t upon success, `NULL` otherwise

    @note returns immidiately if faces an error
*/
extern plist_t *vt_path_listdir(plist_t *const p, const char *const z, const bool ignoreDotFiles);

/** Get all files and sub-directories recursively
    @param p container where to save the data; if NULL is passed, it is allocated
    @param z path
    @param ignoreDotFiles skip hidden .files

    @returns `plist_t*` of str_t upon success, `NULL` otherwise

    @note returns immidiately if faces an error
*/
extern plist_t *vt_path_listdir_recurse(plist_t *const p, const char *const z, const bool ignoreDotFiles);

/** Get path basename
    @param s str_t instance (if `NULL` is passed, str_t is allocated)
    @param z path

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *vt_path_basename(str_t *const s, const char *const z);

/** Creates a directory
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_mkdir(const char *const z);

/** Creates a directory and its parent directories
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_mkdir_parents(const char *const z);

/** Deletes an empty directory
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_rmdir(const char *const z);

/** Deletes an directory and its contents
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_rmdir_recurse(const char *const z);

/** Deletes a file
    @param z a raw C string
    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_remove(const char *const z);

/** Rename/move a directory/file
    @param z1 file/directory
    @param z2 new file/directory

    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_rename(const char *const z1, const char *const z2);

/** Expands tilda `~` to HOMEPATH
    @param z1 path
    @returns a newly allocated `str_t` path with the expanded tilda `~` upon success, `NULL` otherwise
*/
extern str_t *vt_path_expand_tilda(const char *const z);

/** Returns executable path
    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *vt_path_get_this_exe_location(void);

#endif // VITA_PATH_H
