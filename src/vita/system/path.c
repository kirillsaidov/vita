#include "vita/system/path.h"

str_t *path_build(const char *const cs1, const char *const cs2) {
	if(is_null(cs1) || is_null(cs2)) {
		return NULL;
	} 

	// create a new str_t instance and append
	str_t *s = str(cs1);
	if(is_null(s)) {
		vita_warn("str_t allocation faled!", __FUNCTION__);
		return NULL;
	}

	if(str_append(s, PATH_SEPARATOR) != ce_operation_success && str_append(s, cs2) != ce_operation_success) {
		vita_warn("unable to build path!", __FUNCTION__);
		str_free(s);

		return NULL;
	}
	
	return s;
}

str_t *path_build_n(const plist_t *p) {
	if(is_null(p)) {
		return NULL;
	}

	str_t *s = strn(10 * plist_len(p));
	if(is_null(s)) {
		vita_warn("str_t allocation faled!", __FUNCTION__);
		return NULL;
	}
	
	// append the first part
	if(str_append(s, plist_get(p, 0)) != ce_operation_success) {
		vita_warn("unable to build path!", __FUNCTION__);
	}

	// continue appending
	const size_t pLen = plist_len(p);
	for(size_t i = 1; i < pLen; i++) {
		if(str_append(s, PATH_SEPARATOR) != ce_operation_success && str_append(s, plist_get(p, i)) != ce_operation_success) {
			vita_warn("unable to build path!", __FUNCTION__);
		}
	}

	return s;
}








