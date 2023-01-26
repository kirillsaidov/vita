#include "vita/system/path.h"

vt_str_t *vt_path_build(vt_str_t *const s, const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // create vt_str_t if needed
    vt_str_t *st = ((s == NULL) ? (vt_strn(DEFAULT_INIT_ELEMENTS)) : (s));
    if(st == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // append the first part
    if(vt_str_append(st, plist_get(p, 0)) != ve_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));

        // if s is NULL, free st (this is done to avoid freeing user's s instance)
        if(s == NULL) {
            vt_str_free(st);
        }

        return NULL;
    }

    // continue appending
    const size_t pLen = plist_len(p);
    for(size_t i = 1; i < pLen; i++) {
        if(vt_str_append(st, PATH_SEPARATOR) != ve_operation_success && vt_str_append(st, plist_get(p, i)) != ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
            
            // if s is NULL, free st (this is done to avoid freeing user's s instance)
            if(s == NULL) {
                vt_str_free(st);
            }

            return NULL;
        }
    }

    return st;
}

vt_str_t *vt_path_build_n(vt_str_t *const s, const size_t n, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // create a new vt_str_t instance and append
    vt_str_t *st = ((s == NULL) ? (vt_strn(DEFAULT_INIT_ELEMENTS)) : (s));
    if(st == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }
    vt_str_clear(st);

    // append
    va_list args;
    va_start(args, n);
    const char* z = NULL;
    for(size_t i = 0; i < n; i++) {
        // get next item
        z = va_arg(args, char*);
        
        // append
        if(vt_str_append(st, z) != vt_ve_operation_success) { 
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));

            // if s is NULL, free st (this is done to avoid freeing user's s instance)
            if(s == NULL) {
                vt_str_free(st);
            }

            return st; 
        }

        // do not append path separator at the very end,
        // since the last element may be a file, not a directory
        if(i < n-1 && vt_str_append(st, PATH_SEPARATOR) != vt_ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
            
            // if s is NULL, free st (this is done to avoid freeing user's s instance)
            if(s == NULL) {
                vt_str_free(st);
            }
            
            return st;
        }
    }
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

    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        return NULL;
    }

    // create plist instance
    vt_plist_t *pl = (p == NULL ? (vt_plist_create(DEFAULT_INIT_ELEMENTS)) : (p));
    if(pl == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        VT_DEBUG_PRINTF("%s: Failed read the directory %s!\n", vt_get_vita_error_str(vt_ve_operation_failure), z);
        return pl;
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
        
        // push directory name to vt_plist_t
        if(vt_plist_push(pl, vt_str(dirtree->d_name)) != vt_ve_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
            return st;
        }
    }

    closedir(dir);

    return pl;
}

// TODO: CONTINUE FROM HERE
vt_plist_t *vt_path_listdir_recurse(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    if(!vt_path_exists(z)) {
        VT_DEBUG_ASSERT(vt_path_exists(z), "Path does not exist %s!", z);
        return NULL;
    }
    
    // create plist instance
    vt_plist_t *pl = (p == NULL ? (vt_plist_create(DEFAULT_INIT_ELEMENTS)) : (p));
    if(pl == NULL) {
        VT_DEBUG_ASSERT(pl != NULL, "Failed to allocate vt_plist_t instance!");
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        VT_DEBUG_ASSERT(dir != NULL, "Failed read the directory: %s!", z);
        return pl;
    }

    // get directory contents
    struct dirent *dirtree = NULL;
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if(
            (ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals(dirtree->d_name, ".") || 
            vt_str_equals(dirtree->d_name, "..")
        ) {
            continue;
        }

        // save full path
        vt_str_t *s = vt_str(z);
        vt_str_append(s, PATH_SEPARATOR);
        vt_str_append(s, dirtree->d_name);

        // push directory name to vt_plist_t
        vt_plist_push(pl, s);

        // check if current path is a directory
        if(vt_path_is_dir(cstr(s))) {
            vt_path_listdir_recurse(pl, cstr(s), ignoreDotFiles);
        }
    }

    closedir(dir);

    return pl;
}

vt_str_t *vt_path_basename(vt_str_t *const s, const char *const z) {
    if(z == NULL) {
        return NULL;
    }

    // create a new vt_str_t instance
    vt_str_t *st = ((s == NULL) ? (vt_strn(strlen(z))) : (s));
    if(st == NULL) {
        return NULL;
    }

    // find the basename
    const char *ptr = NULL;
    const size_t sLen = str_len(st);
    for(size_t i = sLen - 1; i > 0; i--) {
        if(cstr(st)[i] == PATH_SEPARATOR[0] && i != sLen - 1) {
            ptr = &cstr(st)[i+1];
            break;
        }
    }

    // check if we have enough memory
    const size_t zLen = ptr == NULL ? 0 : strlen(ptr);
    if(sLen < zLen) {
        str_reserve(st, zLen - sLen);
    }

    // save the basename
    str_set(st, ptr == NULL ? "" : ptr);

    return st;
}

bool vt_path_mkdir(const char *const z) {
    if(z == NULL) {
        VT_DEBUG_ASSERT(z != NULL, "Path supplied is NULL!");
        return false;
    }

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
    if(z == NULL) {
        VT_DEBUG_ASSERT(z != NULL, "Path supplied is NULL!");
        return false;
    }

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
        VT_DEBUG_ASSERT(s != NULL, "Failed to allocate a vt_str_t buffer!");

        status = false;
        goto vt_path_mkdir_parents_cleanup;
    }

    // create a fullpath variable
    sfull = vt_strn(str_len(s) + 1);
    if(sfull == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "Failed to allocate a vt_str_t buffer!");

        status = false;
        goto vt_path_mkdir_parents_cleanup;
    }

    // set len to 0
    vt_str_clear(sfull);

    // split string into directories
    dir_list = str_split(NULL, s, PATH_SEPARATOR);
    if(dir_list == NULL) {
        status = false;
        goto vt_path_mkdir_parents_cleanup;
    }

    // make directories with parents
    size_t dirLen = plist_len(dir_list);
    for(size_t i = 0; i < dirLen; i++) {
        // get the first directory in the directory tree that we need to make
        vt_str_t *sdir = (vt_str_t*)(plist_get(dir_list, i));
        
        // append it to the full path
        vt_str_append(sfull, cstr(sdir));
        vt_str_append(sfull, PATH_SEPARATOR);
        
        // make that directory
        vt_path_mkdir(cstr(sfull));
    }

vt_path_mkdir_parents_cleanup:
    // free the strings
    vt_str_free(s);
    vt_str_free(sfull);
    
    // free all strings in dir_list
    if(dir_list != NULL) {
        dirLen = plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            s = (vt_str_t*)(plist_get(dir_list, i));
            vt_str_free(s);
        }
    }

    // free dir_list itself
    plist_free(dir_list);
    
    return status;
}

bool vt_path_rmdir(const char *const z) {
    if(z == NULL || !vt_path_exists(z)) {
        VT_DEBUG_ASSERT(z != NULL, "Path supplied is NULL!");
        VT_DEBUG_ASSERT(vt_path_exists(z), "Path does not exist %s!", z);
        return false;
    }

    int status = rmdir(z);

    #if defined(_WIN32) || defined(_WIN64)
        return (status != 0);
    #else
        return (status == 0);
    #endif
}

bool vt_path_rmdir_recurse(const char *const z) {
    if(z == NULL || !vt_path_exists(z)) {
        VT_DEBUG_ASSERT(z != NULL, "Path supplied is NULL!");
        VT_DEBUG_ASSERT(vt_path_exists(z), "Path does not exist %s!", z);
        return false;
    }

    // return value
    bool status = true;

    // get all files in a directory
    // if none are found, delete the directory and return
    vt_plist_t *dir_list = vt_path_listdir_recurse(NULL, z, false);
    if(dir_list == NULL || !plist_len(dir_list)) {
        status = vt_path_rmdir(z);
        goto vt_path_rmdir_recurse_cleanup;
    }
    
    // iterate starting from the end and remove each element
    // checking its type
    vt_str_t *s = NULL;
    while((s = (vt_str_t*)(plist_pop_get(dir_list))) != NULL) {
        // remove file/directory
        status = vt_path_is_dir(cstr(s)) ? vt_path_rmdir(cstr(s)) : vt_path_remove(cstr(s));
        
        // free vt_str_t instance
        vt_str_free(s);

        if(!status) {
            goto vt_path_rmdir_recurse_cleanup;
        }  
    }

vt_path_rmdir_recurse_cleanup:
    // free all strings in dir_list
    if(dir_list != NULL) {
        const size_t dirLen = plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            s = (vt_str_t*)(plist_get(dir_list, i));
            vt_str_free(s);
        }
    }

    // free dir_list itself
    plist_free(dir_list);
 
    return status;
}

bool vt_path_remove(const char *const z) {
    if(z == NULL || !vt_path_exists(z)) {
        VT_DEBUG_ASSERT(z != NULL, "Path supplied is NULL!");
        VT_DEBUG_ASSERT(vt_path_exists(z), "Path does not exist %s!", z);
        return false;
    }

    return (remove(z) == 0);
}

bool vt_path_rename(const char *const z1, const char *const z2) {
    if(z1 == NULL || z2 == NULL || !vt_path_exists(z1)) {
        VT_DEBUG_ASSERT(z1 != NULL, "Path/File (z1) supplied is NULL!");
        VT_DEBUG_ASSERT(z2 != NULL, "Path/File (z2) supplied is NULL!");
        VT_DEBUG_ASSERT(vt_path_exists(z1), "Path/File does not exist %s!", z1);
        return false;
    }

    #if defined(_WIN32) || defined(_WIN64)
        return (rename(z1, z2) != 0);
    #else
        return (rename(z1, z2) == 0);
    #endif
}

vt_str_t *vt_path_expand_tilda(const char *const z) {
    if(z == NULL) {
        VT_DEBUG_ASSERT(z != NULL, "Path supplied is NULL!");
        return NULL;
    }

    // find tilda `~`
    vt_str_t *s_tilda = vt_str(z);
    const int64_t tilda_pos = str_index_of(s_tilda, '~');
    if(str_len(s_tilda) < 1 || tilda_pos != 0) {
        return s_tilda;
    }
    
    // get HOME path
    const char *const z_homepath = 
    #if defined(_WIN32) || defined(_WIN64)
        getenv("USERPROFILE");
    #else
        getenv("HOME");
    #endif

    if(z_homepath == NULL) {
        VT_DEBUG_ASSERT(z_homepath != NULL, "Environmental variable \'HOME\' was not found, returning as-is!");
        return s_tilda;
    }

    // expand tilda
    str_remove(s_tilda, 0, 1);                  // remove tilda
    str_insert(s_tilda, z_homepath, tilda_pos); // insert HOME path inplace of tilda

    return s_tilda;
}

vt_str_t *vt_path_get_this_exe_location(void) {
    vt_str_t *spath = strn_empty(PATH_MAX);
    if (spath == NULL) {
        VT_DEBUG_ASSERT(spath != NULL, "Failed to allocate vt_str_t instance!");
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
        VT_DEBUG_ASSERT(pathLen > 0, "Failed to retrieve this exe path!");
        vt_str_free(spath);
        return NULL;
    }
    spath->len = pathLen;

    return spath;
}





