#ifndef VITA_SYSTEM_PATH_H
#define VITA_SYSTEM_PATH_H

/** PATH MODULE
    - vt_path_join
    - vt_path_join_list
    - vt_path_join_array
    - vt_path_get_cwd
    - vt_path_exists
    - vt_path_is_dir
    - vt_path_is_file
    - vt_path_get_file_size
    - vt_path_dir_list
    - vt_path_dir_list_recurse
    - vt_path_dirname
    - vt_path_basename
    - vt_path_mkdir
    - vt_path_mkdir_parents
    - vt_path_rmdir
    - vt_path_rmdir_recurse
    - vt_path_remove
    - vt_path_rename
    - vt_path_expand_tilda
    - vt_path_get_this_exe_location
    - vt_path_pop
    - vt_path_validate
*/

#include "vita/core/core.h"
#include "vita/container/str.h"
#include "vita/container/span.h"
#include "vita/container/plist.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #include <direct.h>
    #include <windows.h>
    
    #define VT_PATH_SEPARATOR "\\"
    #define VT_DIR_PERMISSIONS NULL

    #define getcwd _getcwd
    #define mkdir CreateDirectory
    #define rmdir RemoveDirectory
    #define rename MoveFile
#else
    #if defined(__APPLE__) || defined(__MACH__)
        #include <libproc.h>        
    #endif

    #include <unistd.h>
    #include <dirent.h>
    #include <libgen.h>

    #define VT_PATH_SEPARATOR "/"
    #define VT_DIR_PERMISSIONS (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

/// max path length
#define VT_PATH_MAX 4096

/** Builds path from zero-terminated C strings
    @param s vt_str_t instance (if `NULL` is passed, vt_str_t is allocated)
    @param p array of zero-terminated C strings

    @returns `vt_str_t*` upon success, `s` otherwise
*/
extern vt_str_t *vt_path_join_list(vt_str_t *const s, const vt_plist_t *const p);

/** Builds path from zero-terminated C strings
    @param s vt_str_t instance (if `NULL` is passed, vt_str_t is allocated)
    @param array array of zero-terminated strings
    @param n number of strings to join

    @returns `vt_str_t*` upon success, `s` otherwise
*/
extern vt_str_t *vt_path_join_array(vt_str_t *const s, const char *array[], const size_t n);

/** Builds path from zero-terminated C strings
    @param s vt_str_t instance (if `NULL` is passed, vt_str_t is allocated)
    @param ... zero-terminated strings

    @returns `vt_str_t` joined string, `NULL` upon failure
*/
#define vt_path_join(s, ...) vt_path_join_array(s, (const char*[]){__VA_ARGS__}, sizeof((const char*[]){__VA_ARGS__}) / sizeof(const char*))

/** Get current working directory
    @param buf a pointer to a valid buffer
    @param len the size of the provided buffer
    @returns `vt_span_t` representing the resulting path. Empty span upon failure or insufficient buffer size.

    @note use `VT_PATH_MAX` to be safe.
*/
extern vt_span_t vt_path_get_cwd(char *const buf, const size_t len);

/** Checks if path or file exists
    @param z path, zero-terminated C string
    @returns `true` if directory or file exists
*/
extern bool vt_path_exists(const char *const z);

/** Checks if path is a directory
    @param z path, zero-terminated C string directory
    @returns `true` if directory exists
*/
extern bool vt_path_is_dir(const char *const z);

/** Checks if path is a file
    @param z path, zero-terminated C string to file
    @returns `true` if file exists
*/
extern bool vt_path_is_file(const char *const z);

/** Returns file size
    @param z path, zero-terminated C string to file
    @returns size_t filesize >= 0 upon success, -1 upon failure
*/
extern int64_t vt_path_get_file_size(const char *const z);

/** Get all directory contents
    @param p container where to save the data; if NULL is passed, it is allocated
    @param z path, zero-terminated C string
    @param ignoreDotFiles skip hidden .files

    @returns list of `vt_str_t*` upon success, `p` otherwise

    @note passing in `NULL` for the container instance results in that instance being allocated and managed with vt_calloc/realloc/free.
    @note use `vt_path_dir_free(p)` to free the directory tree.
*/
extern vt_plist_t *vt_path_dir_list(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles);

/** Get all files and sub-directories recursively
    @param p container where to save the data; if NULL is passed, it is allocated
    @param z path, zero-terminated C string
    @param ignoreDotFiles skip hidden .files

    @returns list of `vt_str_t*` upon success, `p` otherwise

    @note passing in `NULL` for the container instance results in that instance being allocated and managed with vt_calloc/realloc/free.
    @note use `vt_path_dir_free(p)` to free the directory tree.
*/
extern vt_plist_t *vt_path_dir_list_recurse(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles);

/** Free directory tree 
    @param p dir container object
    @returns None
*/
extern void vt_path_dir_free(vt_plist_t *p);

/** Get path directory name
    @param z path, zero-terminated C string
    @param buf a pointer to a valid buffer
    @param len the size of the provided buffer
    @returns `vt_span_t` representing the resulting path. Empty span upon failure or insufficient buffer size.

    @note use `VT_PATH_MAX` to be safe.
*/
extern vt_span_t vt_path_dirname(const char *const z, char *const buf, const size_t len);

/** Get path basename
    @param z path, zero-terminated C string
    @param buf a pointer to a valid buffer
    @param len the size of the provided buffer
    @returns `vt_span_t` representing the resulting path. Empty span upon failure or insufficient buffer size.

    @note use `VT_PATH_MAX` to be safe.
*/
extern vt_span_t vt_path_basename(const char *const z, char *const buf, const size_t len);

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

/** Deletes a directory and its contents
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
    @param z1 old file/directory name
    @param z2 new file/directory name

    @returns `true` upon success, `false` otherwise
*/
extern bool vt_path_rename(const char *const z1, const char *const z2);

/** Expands tilda `~` to HOMEPATH
    @param z path, zero-terminated C string
    @param buf a pointer to a valid buffer
    @param len the size of the provided buffer
    @returns `vt_span_t` representing the resulting path. Empty span upon failure or insufficient buffer size.

    @note use `VT_PATH_MAX` to be safe.
    @note fails if incorrect tilda position, insufficient buffer size or getenv returns invalid value.
*/
extern vt_span_t vt_path_expand_tilda(const char *const z, char *const buf, const size_t len);

/** Returns executable path
    @param z path, zero-terminated C string
    @param buf a pointer to a valid buffer
    @param len the size of the provided buffer
    @returns `vt_span_t` representing the resulting path. Empty span upon failure or insufficient buffer size.

    @note use `VT_PATH_MAX` to be safe.
*/
extern vt_span_t vt_path_get_this_exe_location(char *const buf, const size_t len);

// /** Push an item up the directory tree
//     @param z path, zero-terminated C string
// */
// extern void vt_path_push(char *const z);

/** Pops off directory "/" - i. e., steps back up the directory tree by modifying the original string
    @param z path, zero-terminated C string
    @returns modified path

    @note if returned path length is 0, then it is current path "."
    @note if you do not want to modify the original string use `vt_path_dirname`
*/
extern const char *vt_path_pop(char *const z);

/** Validates the path and attempts to fix the VT_PATH_SEPARATOR if neccessary
    @param z path, zero-terminated C string
*/
extern void vt_path_validate(char *const z);

#endif // VITA_SYSTEM_PATH_H
