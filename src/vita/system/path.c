#include "vita/system/path.h"

str_t *path_build(str_t *const s, const char *const cs1, const char *const cs2) {
	if(is_null(cs1) || is_null(cs2)) {
		return NULL;
	}

	// create a new str_t instance and append
	str_t *st = ((is_null(s)) ? (strn(strlen(cs1) + strlen(cs2))) : (s));
	if(is_null(st)) {
		vita_warn("str_t allocation faled!", __FUNCTION__);
		return NULL;
	}

	// append
	if(str_append(st, PATH_SEPARATOR) != ce_operation_success && str_append(st, cs2) != ce_operation_success) {
		vita_warn("unable to build path!", __FUNCTION__);
		
		// if s is NULL, free st (this is done to avoid freeing user's s instance)
		if(is_null(s)) {
			str_free(st);
		}

		return NULL;
	}
	
	return st;
}

str_t *path_build_n(str_t *const s, const plist_t *const p) {
	if(is_null(p)) {
		return NULL;
	}

	str_t *st = ((is_null(s)) ? (strn(DEFAULT_INIT_ELEMENTS)) : (s));
	if(is_null(st)) {
		vita_warn("str_t allocation faled!", __FUNCTION__);
		return NULL;
	}
	
	// append the first part
	if(str_append(st, plist_get(p, 0)) != ce_operation_success) {
		vita_warn("unable to build path!", __FUNCTION__);

		// if s is NULL, free st (this is done to avoid freeing user's s instance)
		if(is_null(s)) {
			str_free(st);
		}

		return NULL;
	}

	// continue appending
	const size_t pLen = plist_len(p);
	for(size_t i = 1; i < pLen; i++) {
		if(str_append(st, PATH_SEPARATOR) != ce_operation_success && str_append(st, plist_get(p, i)) != ce_operation_success) {
			vita_warn("unable to build path!", __FUNCTION__);

			// if s is NULL, free st (this is done to avoid freeing user's s instance)
			if(is_null(s)) {
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
	if(is_null(cs)) {
		return false;
	}

	struct stat info;
	return (stat(cs, &info) == 0);
}

bool path_is_dir(const char *const cs) {
	if(is_null(cs)) {
		return false;
	}

	// if given element exists and is a directory
	struct stat info;
	return (stat(cs, &info) == 0 && S_ISDIR(info.st_mode));
}

bool path_is_file(const char *const cs) {
	if(is_null(cs)) {
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
	if(is_null(dir)) {
		return NULL;
	}
	
	// create a container of str_t
	plist_t *p = plist_create(DEFAULT_INIT_ELEMENTS);
	if(is_null(p)) {
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

plist_t *path_listdir_deep(plist_t *const p, const char *const cs, const bool ignoreDotFiles) {
	if(!path_exists(cs)) {
		return NULL;
	}

	plist_t *pl = (is_null(p) ? (plist_create(DEFAULT_INIT_ELEMENTS)) : (p));
	if(is_null(pl)) {
		vita_warn("plist_t allocation faled!", __FUNCTION__);
		return NULL;
	}

	// open directory
	DIR *dir = opendir(cs);
	if(is_null(dir)) {
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
			path_listdir_deep(pl, cstr(s), ignoreDotFiles);
		}
	}

	closedir(dir);

	return pl;
}

str_t *path_basename(str_t *const s, const char *const cs) {
	if(is_null(cs)) {
		return NULL;
	}

	// create a new str_t instance
	str_t *st = ((is_null(s)) ? (strn(strlen(cs))) : (s));
	if(is_null(st)) {
		vita_warn("str_t allocation faled!", __FUNCTION__);
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








