#include "vita/system/path.h"

vt_str_t *vt_path_join_list(vt_str_t *const s, const vt_plist_t *const p) {
    VT_DEBUG_ASSERT(vt_array_is_valid_object(p), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    return vt_str_join_list(s, VT_PATH_SEPARATOR, p);
}

vt_str_t *vt_path_join_array(vt_str_t *const s, const char *array[], const size_t n) {
    VT_DEBUG_ASSERT(array != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    return vt_str_join_array(s, VT_PATH_SEPARATOR, array, n);
}

vt_span_t vt_path_get_cwd(char *const buf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(buf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(len > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // get current working directory and check for failure
    // failed: return empty structure with `head == NULL` and `len == 0`
    if (!getcwd(buf, len)) return (vt_span_t) {0};

    // wrap up the result in a span
    return vt_span_from(buf, vt_strnlen(buf, len), sizeof(char));
}

bool vt_path_exists(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    struct stat info;
    return (stat(z, &info) == 0);
}

bool vt_path_is_dir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // if given element exists and is a directory
    struct stat info;
    return (stat(z, &info) == 0 && S_ISDIR(info.st_mode));
}

bool vt_path_is_file(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // if given element exists and is a regular file
    struct stat info;
    return (stat(z, &info) == 0 && S_ISREG(info.st_mode));
}

int64_t vt_path_get_file_size(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    if (!vt_path_exists(z)) {
        return -1;
    }
    
    int64_t file_size = 0;

    #if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER fsize = {0};
        WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesEx(z, GetFileExInfoStandard, &fad)){
			fsize.LowPart = fad.nFileSizeLow;
			fsize.HighPart = fad.nFileSizeHigh;
		}

        file_size = (size_t)(fsize.QuadPart);
    #else
        // get file stats
        struct stat info;
        if (stat(z, &info) != 0) {
            return -1;
        }

        file_size = (int64_t)(info.st_size);
    #endif
    
    return file_size;
}

vt_plist_t *vt_path_dir_list(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if path exists
    if (!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if (dir == NULL) {
        VT_DEBUG_PRINTF("%s: Failed read the directory <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // create plist instance
    vt_plist_t *pl = (p == NULL) 
        ? vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL)
        : p;
    vt_plist_clear(pl);

    // get directory contents
    struct dirent *dirtree = NULL;
    while ((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if ((ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals_z(dirtree->d_name, ".") || 
            vt_str_equals_z(dirtree->d_name, "..")) 
        {
            continue;
        }

        // save directory name
        vt_str_t *d_name = vt_str_create(dirtree->d_name, pl->alloctr);

        // push directory name to vt_plist_t
        vt_plist_push_back(pl, d_name);
    }

    // close dir
    const int retVal = closedir(dir);
    if (retVal != 0) {
        VT_DEBUG_PRINTF("%s: Failed to close the directory <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z); 
    }

    return pl;
}

vt_plist_t *vt_path_dir_list_recurse(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if path exists
    if (!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if (dir == NULL) {
        VT_DEBUG_PRINTF("%s: Failed read the directory %s!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // create plist instance
    vt_plist_t *pl = (p == NULL) 
        ? vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL)
        : p;

    // get directory contents
    vt_str_t *st = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, pl->alloctr);
    struct dirent *dirtree = NULL;
    while ((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if ((ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals_z(dirtree->d_name, ".") || 
            vt_str_equals_z(dirtree->d_name, "..")) 
        {
            continue;
        }

        // append full path (my_path)
        vt_str_append(st, z);

        // append the next path listed (my_path + / + next_path)
        vt_str_append(st, VT_PATH_SEPARATOR);
        vt_str_append(st, dirtree->d_name);

        // check if current path is a directory; if false, then break
        if (vt_path_is_dir(vt_str_z(st)) && vt_path_dir_list_recurse(pl, vt_str_z(st), ignoreDotFiles) == NULL) {
            break;
        }

        // save directory name
        vt_str_t *d_name = vt_str_dup(st);

        // push directory name to list
        vt_plist_push_back(pl, d_name);

        // reset vt_str_t
        vt_str_clear(st);
    }

    // free resources
    vt_str_destroy(st);

    // close dir
    const int retVal = closedir(dir);
    if (retVal != 0) { VT_DEBUG_PRINTF("%s: Failed to close the directory <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z); }

    return pl;
}

void vt_path_dir_free(vt_plist_t *p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if list is not empty
    if (vt_plist_len(p)) {
        vt_str_t *item = NULL;
        while((item = vt_plist_slide_front(p)) != NULL) {
            vt_str_destroy(item);
        }
    }

    // destory list itself
    vt_plist_destroy(p);
}

vt_span_t vt_path_dirname(const char *const z, char *const buf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(buf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // find dirname
    const size_t ret = vt_dirname_n(z, vt_strnlen(z, VT_PATH_MAX), VT_PATH_SEPARATOR);
    const size_t dirname_len = ret ? ret : 1;

    // check if we have enough space (+1 account for '\0')
    if (dirname_len + 1 > len) return (vt_span_t) {0};

    // make span
    vt_span_t span = vt_span_from(buf, dirname_len, sizeof(char));
    vt_str_set_n(&span.instance, ret ? z : ".", dirname_len);

    return span;
}

vt_span_t vt_path_basename(const char *const z, char *const buf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(buf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // find basename
    const char *ret = vt_basename_n(z, vt_strnlen(z, VT_PATH_MAX), VT_PATH_SEPARATOR);
    const size_t ret_len = vt_strnlen(ret, VT_PATH_MAX);

    // check if we have enough space (+1 account for '\0')
    if (ret_len + 1 > len) return (vt_span_t) {0};

    // make span
    vt_span_t span = vt_span_from(buf, ret_len, sizeof(char));
    vt_str_set_n(&span.instance, ret, ret_len);

    return span;
}

bool vt_path_mkdir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // if directory exists, don't do anything
    if (vt_path_exists(z)) {
        return true;
    }

    int status = mkdir(z, VT_DIR_PERMISSIONS);
    /* Permissions (on unix):
        S_IRWXU - 00700 user (file owner) has read, write and execute permission
        S_IRWXG - 00070 group has read, write and execute permission
        S_IROTH - 00004 others have read permission
        S_IXOTH - 00001 others have execute permission
    */

    #if defined(_WIN32) || defined(_WIN64)
        return (status != 0);
    #else
        return (status == 0);
    #endif
}

bool vt_path_mkdir_parents(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // if directory exists, don't do anything
    if (vt_path_exists(z)) {
        return true;
    }

    // create variables
    bool status = true;
    vt_str_t *s = NULL;
    vt_str_t *sfull = NULL;
    vt_plist_t *dir_list = NULL;
    
    // copy the raw C string into vt_str_t for ease of use
    s = vt_str_create(z, NULL);
    if (s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));

        status = false;
        goto vt_label_path_mkdir_parents__;
    }

    // create a fullpath variable
    sfull = vt_str_create_capacity(vt_str_len(s) + 1, NULL);
    if (sfull == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));

        status = false;
        goto vt_label_path_mkdir_parents__;
    }

    // prepend root VT_PATH_SEPARATOR if neccessary 
    if (z[0] == VT_PATH_SEPARATOR[0]) {
        vt_str_append(sfull, VT_PATH_SEPARATOR);
    }

    // split string into directories
    dir_list = vt_str_split(NULL, s, VT_PATH_SEPARATOR);
    if (dir_list == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));

        status = false;
        goto vt_label_path_mkdir_parents__;
    }

    // make directories with parents
    size_t dirLen = vt_plist_len(dir_list);
    for (size_t i = 0; i < dirLen; i++) {
        // get the first directory in the directory tree that we need to make
        vt_str_t *sdir = (vt_str_t*)(vt_plist_get(dir_list, i));
        
        // append it to the full path
        vt_str_append(sfull, vt_str_z(sdir));
        vt_str_append(sfull, VT_PATH_SEPARATOR);
        
        // make that directory
        vt_path_mkdir(vt_str_z(sfull));
    }

vt_label_path_mkdir_parents__:
    // free the strings
    if (s) { vt_str_destroy(s); }
    if (sfull) { vt_str_destroy(sfull); }
    
    // free all strings in dir_list
    if (dir_list) {
        dirLen = vt_plist_len(dir_list);
        for (size_t i = 0; i < dirLen; i++) {
            s = (vt_str_t*)(vt_plist_get(dir_list, i));
            vt_str_destroy(s);
        }

        // free dir_list itself
        vt_plist_destroy(dir_list);
    }
    
    return status;
}

bool vt_path_rmdir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if path exists
    if (!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return false;
    }

    const int status = rmdir(z);

    #if defined(_WIN32) || defined(_WIN64)
        return (status != 0);
    #else
        return (status == 0);
    #endif
}

bool vt_path_rmdir_recurse(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if path exists
    if (!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return false;
    }

    // return value
    bool status = true;


    // get all files in a directory
    // if none are found, delete the directory and return
    vt_plist_t *dir_list = vt_path_dir_list_recurse(NULL, z, false);
    if (dir_list == NULL || !vt_plist_len(dir_list)) {
        status = vt_path_rmdir(z);
        goto vt_path_rmdir_recurse_cleanup__;
    }
    
    // iterate starting from the end and remove each element
    // checking its type (file or directory)
    vt_str_t *path = NULL;
    while ((path = vt_plist_slide_front(dir_list)) != NULL) {
        // remove file/directory
        status = vt_path_is_dir(vt_str_z(path)) ? vt_path_rmdir(vt_str_z(path)) : vt_path_remove(vt_str_z(path));

        // check status
        if (!status) {
            goto vt_path_rmdir_recurse_cleanup__;
        }  
    }
    vt_path_rmdir(z); // remove the folder when done with its contents

vt_path_rmdir_recurse_cleanup__:
    // free all strings in dir_list
    if (dir_list != NULL) {
        const size_t dirLen = vt_plist_len(dir_list);
        for (size_t i = 0; i < dirLen; i++) {
            vt_str_t *path = vt_plist_get(dir_list, i);
            vt_str_destroy(path);
        }

        // free dir_list itself
        vt_plist_destroy(dir_list);
    }

    return status;
}

bool vt_path_remove(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if path exists
    if (!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return false;
    }

    return (remove(z) == 0);
}

bool vt_path_rename(const char *const z1, const char *const z2) {
    // check for invalid input
    VT_DEBUG_ASSERT(z1 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(z2 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if path exists
    if (!vt_path_exists(z1)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z1);
        return false;
    }

    #if defined(_WIN32) || defined(_WIN64)
        return (rename(z1, z2) != 0);
    #else
        return (rename(z1, z2) == 0);
    #endif
}

vt_span_t vt_path_expand_tilda(const char *const z, char *const buf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(buf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if tilda `~` is in correct position and path length
    const size_t zLen = vt_strnlen(z, VT_PATH_MAX);
    if (!zLen || z[0] != '~') return (vt_span_t) {0};

    // get HOME path
    const char *const homepath = 
    #if defined(_WIN32) || defined(_WIN64)
        getenv("USERPROFILE");
    #else
        getenv("HOME");
    #endif
    
    // check return value from getenv
    if (!homepath) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return (vt_span_t) {0};
    }

    // check if we have enough space (+1 for '\0' is already accounted in zLen instead of tilda `~`)
    const size_t homepath_len = vt_strnlen(homepath, VT_PATH_MAX);
    if (homepath_len + zLen > len) return (vt_span_t) {0};

    // expand tilda
    vt_memmove(buf, homepath, homepath_len * sizeof(char));
    vt_memmove(buf + homepath_len * sizeof(char), z + 1 * sizeof(char), (zLen - 1) * sizeof(char));
    buf[homepath_len + zLen] = '\0';

    return vt_span_from(buf, homepath_len + zLen - 1, sizeof(char)); // zLen-1 for tilda
}

vt_str_t *vt_path_get_this_exe_location(struct VitaBaseAllocatorType *alloctr) {
    vt_str_t *spath = vt_str_create_capacity(VT_PATH_MAX, alloctr);
    if (spath == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));
        return NULL;
    }
    
    // retrieve this exe path
    int64_t pathLen = 0;
    char buffer[VT_PATH_MAX] = {0};
    #if defined(_WIN32) || defined(_WIN64)
        pathLen = GetModuleFileName(NULL, buffer, VT_PATH_MAX);
    #elif defined(__APPLE__) || defined(__MACH__)
        pathLen = proc_pidpath(getpid(), buffer, VT_PATH_MAX);
    #else
        pathLen = readlink("/proc/self/exe", buffer, VT_PATH_MAX);
    #endif

    // check for errors
    if (pathLen <= 0) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        
        vt_str_destroy(spath);
        return NULL;
    }
    vt_str_append_n(spath, buffer, pathLen);

    return spath;
}

void vt_path_pop(char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // create vt_str_t
    vt_str_t s = vt_str_create_static(z);
    
    // check if path ends with path separator
    size_t len = vt_str_len(&s);
    if (z[len-1] == VT_PATH_SEPARATOR[0]) {
        z[len-1] = '\0';
        return;
    }

    // check if path contains path separator
    if (!vt_str_find(&s, VT_PATH_SEPARATOR)) {
        return;
    }

    // pop dir entry, step back the dir tree
    while (len > 0) {
        if (z[len-1] == VT_PATH_SEPARATOR[0]) {
            z[len-1] = '\0';
            return;
        }
        len--;
    }
}

void vt_path_validate(char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // calculate length
    size_t zlen = strlen(z);

    // define the path separator we need to fix
    const char need_fix_path_sep = 
    #if defined(_WIN32) || defined(_WIN64)
        '/';
    #else
        '\\';
    #endif

    while (zlen > 0) {
        if (z[zlen-1] == need_fix_path_sep) {
            z[zlen-1] = VT_PATH_SEPARATOR[0];
        }

        zlen--;
    }
}



