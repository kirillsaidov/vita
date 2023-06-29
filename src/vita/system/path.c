#include "vita/system/path.h"

vt_str_t *vt_path_build(vt_str_t *const s, const vt_plist_t *const p) {
    return vt_str_join(s, PATH_SEPARATOR, p);
}

vt_str_t *vt_path_build_n(vt_str_t *const s, const size_t n, ...) {
    // save args to list
    vt_plist_t *const p = vt_plist_create(n, s == NULL ? NULL : s->alloctr);
    va_list args; va_start(args, n);
    for (size_t i = 0; i < n; i++) {
        // get next item
        char *z = va_arg(args, char*);
        vt_plist_push(p, z);
    }
    va_end(args);
    
    // join
    vt_str_t *st = vt_str_join(s, PATH_SEPARATOR, p);

    // cleanup
    vt_plist_destroy(p);
    
    return st;
}

vt_str_t *vt_path_getcwd(struct VitaBaseAllocatorType *alloctr) {
    return vt_str_take_ownership(getcwd(NULL, 0), alloctr);
}

bool vt_path_exists(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    struct stat info;
    return (stat(z, &info) == 0);
}

bool vt_path_is_dir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // if given element exists and is a directory
    struct stat info;
    return (stat(z, &info) == 0 && S_ISDIR(info.st_mode));
}

bool vt_path_is_file(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // if given element exists and is a regular file
    struct stat info;
    return (stat(z, &info) == 0 && S_ISREG(info.st_mode));
}

int64_t vt_path_get_file_size(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
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
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if((ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals(dirtree->d_name, ".") || 
            vt_str_equals(dirtree->d_name, "..")) 
        {
            continue;
        }

        // allocate memory buffer and copy it to that buffer
        const size_t tmp_dir_len = strlen(dirtree->d_name);
        const char *const d_name = pl->alloctr 
            ? VT_ALLOCATOR_ALLOC(pl->alloctr, tmp_dir_len)
            : VT_CALLOC(tmp_dir_len);
        vt_memcopy((void*)d_name, dirtree->d_name, tmp_dir_len);

        // push directory name to vt_plist_t
        vt_plist_push(pl, d_name);
    }

    // close dir
    const int retVal = closedir(dir);
    if(retVal != 0) {
        VT_DEBUG_PRINTF("%s: Failed to close the directory <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z); 
    }

    return pl;
}

vt_plist_t *vt_path_listdir_recurse(vt_plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        VT_DEBUG_PRINTF("%s: Failed read the directory %s!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
        return NULL;
    }

    // create plist instance
    vt_plist_t *pl = (p == NULL) 
        ? vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL)
        : p;
    vt_plist_clear(pl);

    // get directory contents
    vt_str_t *st = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, pl->alloctr);
    struct dirent *dirtree = NULL;
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if((ignoreDotFiles && dirtree->d_name[0] == '.') ||
            vt_str_equals(dirtree->d_name, ".") || 
            vt_str_equals(dirtree->d_name, "..")) 
        {
            continue;
        }

        // append full path (my_path)
        vt_str_append(st, z);

        // append the next path listed (my_path + / + next_path)
        vt_str_append(st, PATH_SEPARATOR);
        vt_str_append(st, dirtree->d_name);

        // allocate memory buffer and copy it to that buffer
        const size_t tmp_dir_len = vt_str_len(st);
        const char *const d_name = pl->alloctr 
            ? VT_ALLOCATOR_ALLOC(pl->alloctr, tmp_dir_len)
            : VT_CALLOC(tmp_dir_len);
        vt_memcopy((void*)d_name, vt_str_z(st), tmp_dir_len);

        // push directory name to list
        vt_plist_push(pl, d_name);

        // check if current path is a directory; if false, then break
        if(vt_path_is_dir(vt_str_z(st)) && vt_path_listdir_recurse(pl, vt_str_z(st), ignoreDotFiles) == NULL) {
            break;
        }

        // reset vt_str_t
        vt_str_clear(st);
    }

    // free resources
    vt_str_destroy(st);

    // close dir
    const int retVal = closedir(dir);
    if(retVal != 0) { VT_DEBUG_PRINTF("%s: Failed to close the directory <%s>!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z); }

    return pl;
}

vt_str_t *vt_path_basename(vt_str_t *const s, const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create a new vt_str_t instance
    vt_str_t *st = (s == NULL)
        ? vt_str_create_len(strlen(z), NULL) 
        : s;

    // find the basename
    const char *ptr = NULL;
    const size_t sLen = vt_str_len(st);
    for(size_t i = sLen - 1; i > 0; i--) {
        if(z[i] == PATH_SEPARATOR[0] && i != sLen - 1) {
            ptr = &z[i+1];
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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

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
    s = vt_str_create(z, NULL);
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));

        status = false;
        goto vt_label_path_mkdir_parents__;
    }

    // create a fullpath variable
    sfull = vt_str_create_capacity(vt_str_len(s) + 1, NULL);
    if(sfull == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));

        status = false;
        goto vt_label_path_mkdir_parents__;
    }

    // split string into directories
    dir_list = vt_str_split(NULL, s, PATH_SEPARATOR);
    if(dir_list == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));

        status = false;
        goto vt_label_path_mkdir_parents__;
    }

    // make directories with parents
    size_t dirLen = vt_plist_len(dir_list);
    for(size_t i = 0; i < dirLen; i++) {
        // get the first directory in the directory tree that we need to make
        vt_str_t *sdir = (vt_str_t*)(vt_plist_get(dir_list, i));
        
        // append it to the full path
        vt_str_append(sfull, vt_str_z(sdir));
        vt_str_append(sfull, PATH_SEPARATOR);
        
        // make that directory
        vt_path_mkdir(vt_str_z(sfull));
    }

vt_label_path_mkdir_parents__:
    // free the strings
    if(s != NULL) { vt_str_destroy(s); }
    if(sfull != NULL) { vt_str_destroy(sfull); }
    
    // free all strings in dir_list
    if(dir_list != NULL) {
        dirLen = vt_plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            s = (vt_str_t*)(vt_plist_get(dir_list, i));
            vt_str_destroy(s);
        }
    }

    // free dir_list itself
    if(dir_list != NULL) { vt_plist_destroy(dir_list); }
    
    return status;
}

bool vt_path_rmdir(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if path exists
    if(!vt_path_exists(z)) {
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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if path exists
    if(!vt_path_exists(z)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z);
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
    const char *zpath = NULL;
    while((zpath = vt_plist_pop_get(dir_list)) != NULL) {
        // remove file/directory
        status = vt_path_is_dir(zpath) ? vt_path_rmdir(zpath) : vt_path_remove(zpath);

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
            zpath = vt_plist_get(dir_list, i);
            VT_FREE(zpath);
        }

        // free dir_list itself
        vt_plist_destroy(dir_list);
    }

    return status;
}

bool vt_path_remove(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if path exists
    if(!vt_path_exists(z)) {
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
    if(!vt_path_exists(z1)) {
        VT_DEBUG_PRINTF("%s: Path does not exist <%s>\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE), z1);
        return false;
    }

    #if defined(_WIN32) || defined(_WIN64)
        return (rename(z1, z2) != 0);
    #else
        return (rename(z1, z2) == 0);
    #endif
}

vt_str_t *vt_path_expand_tilda(const char *const z, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // find tilda `~`
    vt_str_t *s_tilda = vt_str_create(z, alloctr);
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
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return s_tilda;
    }

    // expand tilda
    vt_str_remove(s_tilda, (size_t)tilda_pos, 1);           // remove tilda
    vt_str_insert(s_tilda, z_homepath, (size_t)tilda_pos);  // insert HOME path inplace of tilda

    return s_tilda;
}

vt_str_t *vt_path_get_this_exe_location(struct VitaBaseAllocatorType *alloctr) {
    vt_str_t *spath = vt_str_create_capacity(PATH_MAX, alloctr);
    if (spath == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));
        return NULL;
    }
    
    // retrieve this exe path
    int64_t pathLen = 0;
    #if defined(_WIN32) || defined(_WIN64)
        pathLen = GetModuleFileName(NULL, spath->ptr, PATH_MAX);
    #elif defined(__APPLE__) || defined(__MACH__)
        pathLen = proc_pidpath(getpid(), spath->ptr, PATH_MAX);
    #else
        pathLen = readlink("/proc/self/exe", spath->ptr, PATH_MAX);
    #endif

    // check for errors
    if(pathLen <= 0) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        
        vt_str_destroy(spath);
        return NULL;
    }
    spath->len = pathLen;

    return spath;
}

void vt_path_pop(char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // calculate length
    size_t zlen = strlen(z);
    
    // check if path ends with path separator
    if(z[zlen-1] == PATH_SEPARATOR[0]) {
        z[zlen-1] = '\0';
        return;
    }

    // check if path contains path separator, if it does not, return
    if(vt_str_find(z, PATH_SEPARATOR) == NULL) {
        return;
    }

    // pop dir entry, step back the dir tree
    while(zlen > 0) {
        if(z[zlen-1] == PATH_SEPARATOR[0]) {
            z[zlen-1] = '\0';
            return;
        }

        zlen--;
    }
}

void vt_path_validate(char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // calculate length
    size_t zlen = strlen(z);

    // define the path separator we need to fix
    const char need_fix_path_sep = 
    #if defined(_WIN32) || defined(_WIN64)
        '/';
    #else
        '\\';
    #endif

    while(zlen > 0) {
        if(z[zlen-1] == need_fix_path_sep) {
            z[zlen-1] = PATH_SEPARATOR[0];
        }

        zlen--;
    }
}



