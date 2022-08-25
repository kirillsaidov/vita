#include "vita/system/path.h"

str_t *path_build(str_t *const s, const char *const z1, const char *const z2) {
    if(z1 == NULL || z2 == NULL) {
        return NULL;
    }

    // create a new str_t instance and append
    str_t *st = ((s == NULL) ? (strn(strlen(z1) + strlen(z2))) : (s));
    if(st == NULL) {
        return NULL;
    }

    // append
    if(str_append(st, PATH_SEPARATOR) != ve_operation_success && str_append(st, z2) != ve_operation_success) {
        // if s is NULL, free st (this is done to avoid freeing user's s instance)
        if(s == NULL) {
            str_free(st);
        }

        return NULL;
    }

    return st;
}

str_t *path_build_n(str_t *const s, const plist_t *const p) {
    if(p == NULL) {
        return NULL;
    }

    str_t *st = ((s == NULL) ? (strn(DEFAULT_INIT_ELEMENTS)) : (s));
    if(st == NULL) {
        return NULL;
    }

    // append the first part
    if(str_append(st, plist_get(p, 0)) != ve_operation_success) {
        // if s is NULL, free st (this is done to avoid freeing user's s instance)
        if(s == NULL) {
            str_free(st);
        }

        return NULL;
    }

    // continue appending
    const size_t pLen = plist_len(p);
    for(size_t i = 1; i < pLen; i++) {
        if(str_append(st, PATH_SEPARATOR) != ve_operation_success && str_append(st, plist_get(p, i)) != ve_operation_success) {
            // if s is NULL, free st (this is done to avoid freeing user's s instance)
            if(s == NULL) {
                str_free(st);
            }

            return NULL;
        }
    }

    return st;
}

str_t *path_getcwd() {
    return str_take_ownership(getcwd(0, 0));
}

bool path_exists(const char *const z) {
    if(z == NULL) {
        return false;
    }

    struct stat info;
    return (stat(z, &info) == 0);
}

bool path_is_dir(const char *const z) {
    if(z == NULL) {
        return false;
    }

    // if given element exists and is a directory
    struct stat info;
    return (stat(z, &info) == 0 && S_ISDIR(info.st_mode));
}

bool path_is_file(const char *const z) {
    if(z == NULL) {
        return false;
    }

    // if given element exists and is a regular file
    struct stat info;
    return (stat(z, &info) == 0 && S_ISREG(info.st_mode));
}

int64_t path_get_file_size(const char *const z) {
    if(z == NULL || !path_exists(z)) {
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

plist_t *path_listdir(plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    if(!path_exists(z)) {
        return NULL;
    }

    // create plist instance
    plist_t *pl = (p == NULL ? (plist_create(DEFAULT_INIT_ELEMENTS)) : (p));
    if(pl == NULL) {
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        return pl;
    }

    // get directory contents
    struct dirent *dirtree = NULL;
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if(
            (ignoreDotFiles && dirtree->d_name[0] == '.') ||
            str_equals(dirtree->d_name, ".") || 
            str_equals(dirtree->d_name, "..")
        ) {
            continue;
        }
        
        // push directory name to plist_t
        plist_push(pl, str(dirtree->d_name));
    }

    closedir(dir);

    return pl;
}

plist_t *path_listdir_recurse(plist_t *const p, const char *const z, const bool ignoreDotFiles) {
    if(!path_exists(z)) {
        return NULL;
    }
    
    // create plist instance
    plist_t *pl = (p == NULL ? (plist_create(DEFAULT_INIT_ELEMENTS)) : (p));
    if(pl == NULL) {
        return NULL;
    }

    // open directory
    DIR *dir = opendir(z);
    if(dir == NULL) {
        return pl;
    }

    // get directory contents
    struct dirent *dirtree = NULL;
    while((dirtree = readdir(dir)) != NULL) {
        // ignore "." and ".." directories
        if(
            (ignoreDotFiles && dirtree->d_name[0] == '.') ||
            str_equals(dirtree->d_name, ".") || 
            str_equals(dirtree->d_name, "..")
        ) {
            continue;
        }

        // save full path
        str_t *s = str(z);
        str_append(s, PATH_SEPARATOR);
        str_append(s, dirtree->d_name);

        // push directory name to plist_t
        plist_push(pl, s);

        // check if current path is a directory
        if(path_is_dir(cstr(s))) {
            path_listdir_recurse(pl, cstr(s), ignoreDotFiles);
        }
    }

    closedir(dir);

    return pl;
}

str_t *path_basename(str_t *const s, const char *const z) {
    if(z == NULL) {
        return NULL;
    }

    // create a new str_t instance
    str_t *st = ((s == NULL) ? (strn(strlen(z))) : (s));
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

bool path_mkdir(const char *const z) {
    if(z == NULL) {
        return false;
    }

    // if directory exists, don't do anything
    if(path_exists(z)) {
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

bool path_mkdir_parents(const char *const z) {
    if(z == NULL) {
        return false;
    }

    // if directory exists, don't do anything
    if(path_exists(z)) {
        return true;
    }

    // create variables
    bool status = true;
    str_t *s = NULL;
    str_t *sfull = NULL;
    plist_t *dir_list = NULL;
    
    // copy the raw C string into str_t for ease of use
    s = str(z);
    if(s == NULL) {
        status = false;
        goto path_mkdir_parents_cleanup;
    }

    // create a fullpath variable
    sfull = strn(str_len(s) + 1);
    if(sfull == NULL) {
        status = false;
        goto path_mkdir_parents_cleanup;
    }

    // set len to 0
    str_clear(sfull);

    // split string into directories
    dir_list = str_split(NULL, s, PATH_SEPARATOR);
    if(dir_list == NULL) {
        status = false;
        goto path_mkdir_parents_cleanup;
    }

    // make directories with parents
    const size_t dirLen = plist_len(dir_list);
    for(size_t i = 0; i < dirLen; i++) {
        // get the first directory in the directory tree that we need to make
        str_t *sdir = (str_t*)(plist_get(dir_list, i));
        
        // append it to the full path
        str_append(sfull, cstr(sdir));
        str_append(sfull, PATH_SEPARATOR);
        
        // make that directory
        path_mkdir(cstr(sfull));
    }

path_mkdir_parents_cleanup:
    // free the strings
    str_free(s);
    str_free(sfull);
    
    // free all strings in dir_list
    if(dir_list != NULL) {
        const size_t dirLen = plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            str_t *s = (str_t*)(plist_get(dir_list, i));
            str_free(s);
        }
    }

    // free dir_list itself
    plist_free(dir_list);
    
    return status;
}

bool path_rmdir(const char *const z) {
    if(z == NULL || !path_exists(z)) {
        return false;
    }

    int status = rmdir(z);

    #if defined(_WIN32) || defined(_WIN64)
        return (status != 0);
    #else
        return (status == 0);
    #endif
}

bool path_rmdir_recurse(const char *const z) {
    if(z == NULL || !path_exists(z)) {
        return false;
    }

    // return value
    bool status = true;

    // get all files in a directory
    // if none are found, delete the directory and return
    plist_t *dir_list = path_listdir_recurse(NULL, z, false);
    if(dir_list == NULL || !plist_len(dir_list)) {
        status = path_rmdir(z);
        goto path_rmdir_recurse_cleanup;
    }
    
    // iterate starting from the end and remove each element
    // checking its type
    str_t *s = NULL;
    while((s = (str_t*)(plist_pop_get(dir_list))) != NULL) {
        // remove file/directory
        status = path_is_dir(cstr(s)) ? path_rmdir(cstr(s)) : path_remove(cstr(s));
        
        // free str_t instance
        str_free(s);

        if(!status) {
            goto path_rmdir_recurse_cleanup;
        }  
    }

path_rmdir_recurse_cleanup:
    // free all strings in dir_list
    if(dir_list != NULL) {
        const size_t dirLen = plist_len(dir_list);
        for(size_t i = 0; i < dirLen; i++) {
            str_t *s = (str_t*)(plist_get(dir_list, i));
            str_free(s);
        }
    }

    // free dir_list itself
    plist_free(dir_list);
 
    return status;
}

bool path_remove(const char *const z) {
    if(z == NULL || !path_exists(z)) {
        return false;
    }

    return (remove(z) == 0);
}

bool path_rename(const char *const z1, const char *const z2) {
    if(z1 == NULL || z2 == NULL || !path_exists(z1)) {
        return false;
    }

    #if defined(_WIN32) || defined(_WIN64)
        return (rename(z1, z2) != 0);
    #else
        return (rename(z1, z2) == 0);
    #endif
}








