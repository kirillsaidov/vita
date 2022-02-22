#include "vita/system/path.h"

str_t *path_build(str_t *const s, const char *const cs1, const char *const cs2) {
	if(cs1 == NULL || cs2 == NULL) {
		return NULL;
	}

	// create a new str_t instance and append
	str_t *st = ((s == NULL) ? (strn(strlen(cs1) + strlen(cs2))) : (s));
	if(st == NULL) {
		return NULL;
	}

	// append
	if(str_append(st, PATH_SEPARATOR) != ve_operation_success && str_append(st, cs2) != ve_operation_success) {
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

bool path_exists(const char *const cs) {
	if(cs == NULL) {
		return false;
	}

	struct stat info;
	return (stat(cs, &info) == 0);
}

bool path_is_dir(const char *const cs) {
	if(cs == NULL) {
		return false;
	}

	// if given element exists and is a directory
	struct stat info;
	return (stat(cs, &info) == 0 && S_ISDIR(info.st_mode));
}

bool path_is_file(const char *const cs) {
	if(cs == NULL) {
		return false;
	}

	// if given element exists and is a regular file
	struct stat info;
	return (stat(cs, &info) == 0 && S_ISREG(info.st_mode));
}

plist_t *path_listdir(const char *const cs) {
	if(!path_exists(cs)) {
		return NULL;
	}

	// open directory
	DIR *dir = opendir(cs);
	if(dir == NULL) {
		return NULL;
	}

	// create a container of str_t
	plist_t *p = plist_create(DEFAULT_INIT_ELEMENTS);
	if(p == NULL) {
		return NULL;
	}

	// get directory contents
	struct dirent *dirtree = readdir(dir);
	while(dirtree != NULL) {
		// push directory name to plist_t
		plist_push(p, str(dirtree->d_name));

		// update directory name
		dirtree = readdir(dir);
	}

	closedir(dir);

	return p;
}

plist_t *path_listdir_recurse(plist_t *const p, const char *const cs, const bool ignoreDotFiles) {
	if(!path_exists(cs)) {
		return NULL;
	}

	plist_t *pl = (p == NULL ? (plist_create(DEFAULT_INIT_ELEMENTS)) : (p));
	if(pl == NULL) {
		return NULL;
	}

	// open directory
	DIR *dir = opendir(cs);
	if(dir == NULL) {
		return pl;
	}

	// get directory contents
	struct dirent *dirtree = NULL;
	while((dirtree = readdir(dir)) != NULL) {
		// ignore "." and ".." directories
		if((ignoreDotFiles && dirtree->d_name[0] == '.') ||
			(str_equals(dirtree->d_name, ".") && str_equals(dirtree->d_name, ".."))) {
			continue;
		}

		// save full path
		str_t *s = str(cs);
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

str_t *path_basename(str_t *const s, const char *const cs) {
	if(cs == NULL) {
		return NULL;
	}

	// create a new str_t instance
	str_t *st = ((s == NULL) ? (strn(strlen(cs))) : (s));
	if(st == NULL) {
		return NULL;
	}

	// find the basename
	const char *ptr;
	for(size_t i = str_len(st) - 1; i >= 0; i--) {
		if(cstr(st)[i] == PATH_SEPARATOR[0] && i != str_len(st) - 1) {
			ptr = &cstr(st)[i+1];
			break;
		}
	}

	// save the basename
	if(str_has_space(st) < strlen(ptr)) {
		str_reserve(st, strlen(ptr) - str_has_space(st));
		str_set(st, ptr);
	}

	return st;
}

bool path_mkdir(const str_t *const s) {
	if(s == NULL || path_exists(s)) {
		return false;
	}

	int status = mkdir(cstr(s), DIR_PERMISSIONS);
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

/*bool path_mkdir_recurse(const str_t *const s) {
	if(s == NULL) {
		return false;
	}

	if(path_exists(s)) {
		return true;
	}

	path_mkdir_recurse(dirname())
}*/

bool path_rmdir(const str_t *const s) {
	if(s == NULL || !path_exists(s)) {
		return false;
	}

	int status = rmdir(cstr(s));

	#if defined(_WIN32) || defined(_WIN64)
		return (status != 0);
	#else
		return (status == 0);
	#endif
}
