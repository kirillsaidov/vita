#include "vita/system/path.h"

vt_str_t *vt_path_build(vt_str_t *const s, const vt_plist_t *const p) {
    return vt_str_join(s, PATH_SEPARATOR, p);
}

vt_str_t *vt_path_build_n(vt_str_t *const s, const size_t n, ...) {
    va_list args; va_start(args, n); 
    vt_str_t *st = vt_str_join_n(s, PATH_SEPARATOR, n, args);
    va_end(args);
    
    return st;
}

vt_str_t *vt_path_getcwd(void) {
    return vt_str_take_ownership(getcwd(NULL, 0));
}

bool vt_path_exists(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    struct stat info;
    return (stat(z, &info) == 0);
}

bool vt_path_is_dir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // if given element exists and is a directory
    struct stat info;
    return (stat(z, &info) == 0 && S_ISDIR(info.st_mode));
}

bool vt_path_is_file(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // if given element exists and is a regular file
    struct stat info;
    return (stat(z, &info) == 0 && S_ISREG(info.st_mode));
}

int64_t vt_path_get_file_size(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    if(!vt_path_exists(z)) {
        return -1;
    }
    
    int64_t file_size = 0;

    #if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER fsize = {0};
        WIN32_FILE_ATTRIBUTE_DATA fad;
		if(GetFileAttributesEx(z, GetFileExInfoStandard, &fad)){
			fsize.LowPart = fad.nFileSizeLow;
			fsize.HighPart = fad.nFileSizeHigh;
		}

        file_size = (size_t)(fsize.QuadPart);
    #else
        // get file stats
        struct stat info;
        if(stat(z, &info) != 0) {
            return -1;
        }

        file_size = (int64_t)(info.st_size);
    #endif
    
    return file_size;
}

vt_plist_t *vt_path_listdir(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        return NULL;
    }

    // create plist instance
    vt_plist_t *pl = (p == NULL ? (vt_plist_create(VT_DEFAULT_INIT_ELEMENTS)) : (p));
    if(pl == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        VT_DEBUG_PRINTF("%s: Failed read the directory <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        
        // free only if it was allocated
        if(p == NULL) {
            vt_plist_free(pl);
        }

        return NULL;
    }

    // get directory contents
    struct dirent *dirtree = NULL;
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if((ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals(dirtree->d_name, ".") || 
            vt_str_equals(dirtree->d_name, "..")) 
        {
            continue;
        }
        
        // save directory
        const char *const d_name = strdup(dirtree->d_name);
        if(d_name == NULL) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
            break;
        }

        // push directory name to vt_plist_t
        if(vt_plist_push(pl, d_name) != vt_ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
            break;
        }
    }

    // close dir
    const int retVal = closedir(dir);
    if(retVal != 0) { VT_DEBUG_PRINTF("%s: Failed to close the directory <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z); }

    return pl;
}

vt_plist_t *vt_path_listdir_recurse(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        return NULL;
    }

    // create plist instance
    vt_plist_t *pl = (p == NULL ? (vt_plist_create(VT_DEFAULT_INIT_ELEMENTS)) : (p));
    if(pl == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        VT_DEBUG_PRINTF("%s: Failed read the directory %s!\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        
        // free only if it was allocated
        if(p == NULL) {
            vt_plist_free(pl);
        }

        return NULL;
    }

    // get directory contents
    vt_str_t *st = vt_strn_empty(VT_DEFAULT_INIT_ELEMENTS);
    struct dirent *dirtree = NULL;
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if((ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals(dirtree->d_name, ".") || 
            vt_str_equals(dirtree->d_name, "..")) 
        {
            continue;
        }

        // save full path
        if(st == NULL || vt_str_append(st, z) != vt_ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
            break;
        }

        // save path
        if(vt_str_append(st, PATH_SEPARATOR) != vt_ve_operation_success || vt_str_append(st, dirtree->d_name) != vt_ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
            break;
        }

        // push directory name to list
        if(vt_plist_push(pl, strdup(vt_cstr(st))) != vt_ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
            break;
        }

        // check if current path is a directory
        if(vt_path_is_dir(vt_cstr(st)) && vt_path_listdir_recurse(pl, vt_cstr(st), ignoreDotFiles) == NULL) {
            break;
        }

        // reset vt_str_t
        vt_str_clear(st);
    }

    // free resources
    vt_str_free(st);

    // close dir
    const int retVal = closedir(dir);
    if(retVal != 0) { VT_DEBUG_PRINTF("%s: Failed to close the directory <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z); }

    return pl;
}

vt_str_t *vt_path_basename(vt_str_t *const s, const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // create a new vt_str_t instance
    vt_str_t *st = ((s == NULL) ? (vt_strn(strlen(z))) : (s));
    if(st == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // find the basename
    const char *ptr = NULL;
    const size_t sLen = vt_str_len(st);
    for(size_t i = sLen - 1; i > 0; i--) {
        if(vt_cstr(st)[i] == PATH_SEPARATOR[0] && i != sLen - 1) {
            ptr = &vt_cstr(st)[i+1];
            break;
        }
    }

    // check if we have enough memory
    const size_t zLen = ptr == NULL ? 0 : strlen(ptr);
    if(sLen < zLen) {
        vt_str_reserve(st, zLen - sLen);
    }

    // save the basename
    vt_str_set(st, ptr == NULL ? "" : ptr);

    return st;
}

bool vt_path_mkdir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // if directory exists, don't do anything
    if(vt_path_exists(z)) {
        return true;
    }

    int status = mkdir(z, DIR_PERMISSIONS);
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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // if directory exists, don't do anything
    if(vt_path_exists(z)) {
        return true;
    }

    // create variables
    bool status = true;
    vt_str_t *s = NULL;
    vt_str_t *sfull = NULL;
    vt_plist_t *dir_list = NULL;
    
    // copy the raw C string into vt_str_t for ease of use
    s = vt_str(z);
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        status = false;
        goto label_vt_path_mkdir_parents_cleanup__;
    }

    // create a fullpath variable
    sfull = vt_strn(vt_str_len(s) + 1);
    if(sfull == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        status = false;
        goto label_vt_path_mkdir_parents_cleanup__;
    }

    // set len to 0
    vt_str_clear(sfull);

    // split string into directories
    dir_list = vt_str_split(NULL, s, PATH_SEPARATOR);
    if(dir_list == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));

        status = false;
        goto label_vt_path_mkdir_parents_cleanup__;
    }

    // make directories with parents
    size_t dirLen = vt_plist_len(dir_list);
    for(size_t i = 0; i < dirLen; i++) {
        // get the first directory in the directory tree that we need to make
        vt_str_t *sdir = (vt_str_t*)(vt_plist_get(dir_list, i));
        
        // append it to the full path
        vt_str_append(sfull, vt_cstr(sdir));
        vt_str_append(sfull, PATH_SEPARATOR);
        
        // make that directory
        vt_path_mkdir(vt_cstr(sfull));
    }

label_vt_path_mkdir_parents_cleanup__:
    // free the strings
    if(s != NULL) { vt_str_free(s); }
    if(sfull != NULL) { vt_str_free(sfull); }
    
    // free all strings in dir_list
    if(dir_list != NULL) {
        dirLen = vt_plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            s = (vt_str_t*)(vt_plist_get(dir_list, i));
            vt_str_free(s);
        }
    }

    // free dir_list itself
    if(dir_list != NULL) { vt_plist_free(dir_list); }
    
    return status;
}

bool vt_path_rmdir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        return false;
    }

    // return value
    bool status = true;

    // get all files in a directory
    // if none are found, delete the directory and return
    vt_plist_t *dir_list = vt_path_listdir_recurse(NULL, z, false);
    if(dir_list == NULL || !vt_plist_len(dir_list)) {
        status = vt_path_rmdir(z);
        goto vt_path_rmdir_recurse_cleanup__;
    }
    
    // iterate starting from the end and remove each element
    // checking its type
    vt_str_t *s = NULL;
    while((s = (vt_str_t*)(vt_plist_pop_get(dir_list))) != NULL) {
        // remove file/directory
        const char *const ztmp = vt_cstr(s);
        status = vt_path_is_dir(ztmp) ? vt_path_rmdir(ztmp) : vt_path_remove(ztmp);

        // check status
        if(!status) {
            goto vt_path_rmdir_recurse_cleanup__;
        }  
    }

vt_path_rmdir_recurse_cleanup__:
    // free all strings in dir_list
    if(dir_list != NULL) {
        const size_t dirLen = vt_plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            s = (vt_str_t*)(vt_plist_get(dir_list, i));
            vt_str_free(s);
        }

        // free dir_list itself
        vt_plist_free(dir_list);
    }

    return status;
}

bool vt_path_remove(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        return false;
    }

    return (remove(z) == 0);
}

bool vt_path_rename(const char *const z1, const char *const z2) {
    // check for invalid input
    VT_DEBUG_ASSERT(z1 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(z2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if path exists
    if(!vt_path_exists(z1)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_get_vita_error_str(vt_ve_operation_failure), z1);
        return false;
    }

    #if defined(_WIN32) || defined(_WIN64)
        return (rename(z1, z2) != 0);
    #else
        return (rename(z1, z2) == 0);
    #endif
}

vt_str_t *vt_path_expand_tilda(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // find tilda `~`
    vt_str_t *s_tilda = vt_str(z);
    const int64_t tilda_pos = vt_str_index_of(s_tilda, '~');
    if(!vt_str_len(s_tilda) || tilda_pos != 0) {
        return s_tilda;
    }
    
    // get HOME path
    const char *const z_homepath = 
    #if defined(_WIN32) || defined(_WIN64)
        getenv("USERPROFILE");
    #else
        getenv("HOME");
    #endif

    // check return value from getenv
    if(z_homepath == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
        return s_tilda;
    }

    // expand tilda
    vt_str_remove(s_tilda, (size_t)tilda_pos, 1);           // remove tilda
    vt_str_insert(s_tilda, z_homepath, (size_t)tilda_pos);  // insert HOME path inplace of tilda

    return s_tilda;
}

vt_str_t *vt_path_get_this_exe_location(void) {
    vt_str_t *spath = vt_strn_empty(PATH_MAX);
    if (spath == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }
    
    // retrieve this exe path
    int64_t pathLen = 0;
    #if defined(_WIN32) || defined(_WIN64)
        pathLen = GetModuleFileName(NULL, spath->ptr, PATH_MAX-1);
    #else
        pathLen = readlink("/proc/self/exe", spath->ptr, PATH_MAX-1);
    #endif

    // check for errors
    if(pathLen <= 0) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
        
        vt_str_free(spath);
        return NULL;
    }
    spath->len = pathLen;

    return spath;
}





