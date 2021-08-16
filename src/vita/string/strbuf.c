#include "vita/string/strbuf.h"

// strbuf internal memory handler
static memhandler_pt strbufMemhandlerInternal = NULL;

// strbuf memory management strategy
static bool strbufManualCollect = false;

void strbuf_manual_collect(const bool status) {
	strbufManualCollect = status;
}

bool strbuf_manual_collect_state() {
	return strbufManualCollect;
}







bool strbuf_memhandler_internal_create(void) {
	strbufMemhandlerInternal = ((is_null(strbufMemhandlerInternal)) ? (memhandler_create()) : (strbufMemhandlerInternal));
	if(is_null(strbufMemhandlerInternal)) {
		return false;
	}

	return true;
}

void strbuf_memhandler_internal_destroy(void) {
	if(!is_null(strbufMemhandlerInternal)) {
		memhandler_destroy(strbufMemhandlerInternal);
	}
}

const memhandler_pt strbuf_memhandler_internal(void) {
	return strbufMemhandlerInternal;
}







strbuf_pt strbuf(const str_t s) {
		return strbuf_new(s);
}

strbuf_pt strbuf_new(const str_t s) {
	// create a strbuf struct
	strbuf_pt sb = ((strbufManualCollect) ? 
		(mem_malloc(1, sizeof(str_t))) : 
		(memhandler_malloc(strbufMemhandlerInternal, 1, sizeof(str_t)))
	);

	// checking if sb was allocated
	if(is_null(sb)) {
		logger_error(str("unable to create a strbuf struct!"), str("strbuf_new"));

		if(!strbufManualCollect) {
			logger_info(str("ManualCollect is false? Then don't forget to initialize the module\'s internal memhandler."));
		}
		
		return NULL;
	}

	// create strbuf string
	*sb = (str_t) {
		.ptr = ((strbufManualCollect) ? 
			(mem_malloc((s.len + s.len/3 + 1), sizeof(char))) : 
			(memhandler_calloc(strbufMemhandlerInternal, (s.len + s.len/3 + 1), sizeof(char)))),
		.len = s.len,
		.capacity = s.len + s.len/3
	};

	// checking if sb->ptr was allocated
	if(is_null(sb->ptr)) {
		logger_error(str("unable to create a strbuf string!"), str("strbuf_new"));
		return NULL;
	}

	// copy str data to strbuf
	memcpy(sb->ptr, as_cstr(s), (s.len * sizeof(char)));
	sb->ptr[s.len] = '\0';

	return sb;
}

void strbuf_free(strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL, safely exiting..."), str("strbuf_free"));
		return;
	}

	// free the strbuf string and strbuf struct
	if(strbufManualCollect) {
		mem_free(sb->ptr);
		mem_free(sb);
	} else {
		memhandler_free(strbufMemhandlerInternal, sb->ptr);
		memhandler_free(strbufMemhandlerInternal, sb);
	}
}







const char* strbuf_cstr(const strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; returning NULL..."), str("strbuf_cstr"));
		return NULL;
	}

	return sb->ptr;
}

size_t strbuf_len(const strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; returning 0!"), str("strbuf_len"));
		return 0;
	}

	return sb->len;
}

size_t strbuf_capacity(const strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; returning 0!"), str("strbuf_capacity"));
		return 0;
	}

	return sb->capacity;
}

size_t strbuf_has_space(const strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; returning 0!"), str("strbuf_has_space"));
		return 0;
	}

	return (sb->capacity - sb->len);
}







bool strbuf_shrink(strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_shrink"));
		return false;
	}

	// if length and capacity are the same, exit the function
	if(sb->len == sb->capacity) {
		return false;
	}

	// shrink the array capacity to length
	bool success = ((strbufManualCollect) ? 
		(mem_realloc((void**)&sb->ptr, (sb->len + 1), sizeof(char))) : 
		(memhandler_realloc(strbufMemhandlerInternal, (void**)&sb->ptr, (sb->len + 1), sizeof(char)))
	);
	

	// check if the operation was successfull
	if(!success) {
		logger_error(str("unable to shrink the string!"), str("strbuf_shrink"));
	} else {
		sb->capacity = sb->len;
	}

	return success;
}

bool strbuf_reserve(strbuf_pt sb, const size_t n) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_reserve"));
		return false;
	}

	// reserve memory for additional n elements
	bool success = ((strbufManualCollect) ? 
		(mem_realloc((void**)&sb->ptr, (sb->capacity + n + 1), sizeof(char))) : 
		(memhandler_realloc(strbufMemhandlerInternal, (void**)&sb->ptr, (sb->capacity + n + 1), sizeof(char)))
	);
	

	// check if the operation was successfull
	if(!success) {
		logger_error(str("cannot reserve memory for strbuf!"), str("strbuf_reserve"));
	} else {
		sb->capacity += n;
	}

	return success;
}

bool strbuf_append(strbuf_pt sb, const str_t s) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_append"));
		return false;
	}

	// check if new memory needs to be allocated
	bool success = true;
	if(strbuf_has_space(sb) <= s.len) {
		success = strbuf_reserve(sb, (s.len - strbuf_has_space(sb)));
	}

	// copy str contents to strbuf
	if(!success) {
		logger_error(str("cannot append to string; failed to reserve more memory!"), str("strbuf_append"));
	} else {
		// copy str to strbuf
		memcpy((sb->ptr + sb->len * sizeof(char)), as_cstr(s), (s.len * sizeof(char)));

		// set new length
		sb->len += s.len;

		// add the '\0' terminator
		sb->ptr[sb->len] = '\0';
	}

	return success;
}

bool strbuf_insert(strbuf_pt sb, const str_t s, const size_t fromIndex) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_insert"));
		return false;
	}

	// check index for out-of-bounds access
	if(fromIndex >= sb->len) {
		logger_error(str("out-of-bounds index access; safely exiting..."), str("strbuf_insert"));
		return false;
	}

	// check if new memory needs to be allocated
	bool success = true;
	if(strbuf_has_space(sb) <= s.len) {
		success = strbuf_reserve(sb, (s.len - strbuf_has_space(sb)));
	}

	// copy str contents to strbuf
	if(!success) {
		logger_error(str("cannot insert str to strbuf; failed to reserve more memory!"), str("strbuf_insert"));
	} else {
		// shift the end of string from the specified index `fromIndex` to `fromIndex + s.len` in strbuf
		memmove((sb->ptr + (fromIndex + s.len) * sizeof(char)), (sb->ptr + fromIndex * sizeof(char)), ((sb->len - fromIndex) * sizeof(char)));

		// copy the str contents to strbuf from the specified index
		memcpy((sb->ptr + fromIndex * sizeof(char)), as_cstr(s), (s.len * sizeof(char)));

		// set new length
		sb->len += s.len;

		// add the '\0' terminator
		sb->ptr[sb->len] = '\0';
	}

	return success;
}

bool strbuf_remove(strbuf_pt sb, const size_t fromIndex, const size_t n) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_remove"));
		return false;
	}

	// check index for out-of-bounds access
	if(fromIndex >= sb->len) {
		logger_error(str("out-of-bounds index access; safely exiting..."), str("strbuf_remove"));
		return false;
	}

	// shift the characters in string from index `fromIndex + n` to `fromIndex` in strbuf
	memcpy((sb->ptr + fromIndex * sizeof(char)), (sb->ptr + (fromIndex + n) * sizeof(char)), ((sb->len - (fromIndex + n)) * sizeof(char)));

	// set new length
	sb->len -= n;

	// add the '\0' terminator
	sb->ptr[sb->len] = '\0';

	return true;
}

bool strbuf_remove_str(strbuf_pt sb, const str_t s) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_remove_str"));
		return false;
	}

	// find a substring in strbuf
	char* sub = strstr(strbuf_cstr(sb), as_cstr(s));
	if(is_null(sub)) {
		logger_error(str("substring wasn't found; exiting..."), str("strbuf_remove_str"));
		return false;
	}

	// find how many characters to copy from the end
	const size_t diff = (size_t)((strbuf_cstr(sb) + sb->len * sizeof(char)) - (sub + s.len * sizeof(char)));

	// shift the characters to the left of the string by the substring length
	memcpy(sub, (sub + s.len * sizeof(char)), diff);

	// set new length
	sb->len -= s.len;

	// add the '\0' terminator
	sb->ptr[sb->len] = '\0';

	return true;
}

bool strbuf_remove_str_all(strbuf_pt sb, const str_t s) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_remove_str_all"));
		return false;
	}

	// remove all instances of a substring
	while(strbuf_remove_str(sb, s)) {}
		
	return true;
}

size_t strbuf_contains(const strbuf_pt sb, const str_t s) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_contains"));
		return false;
	}
		
	// count the number of substring instances in strbuf
	size_t count = 0;
	char* p = strstr(strbuf_cstr(sb), as_cstr(s));
	while(p != NULL) {
		count++;
		p = strstr((p + s.len*sizeof(char)), as_cstr(s));
	}
	
	return count;
}

array_pt strbuf_split(const strbuf_pt sb, const str_t sep) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_split"));
		return NULL;
	}

	// check if strbuf dependency 'array' is initialized
	if(is_null(array_memhandler_internal()) && array_manual_collect_status()) {
		array_manual_collect(true);
	} else {
		logger_error(str("strbuf_split depends on array_memhandler_internal. Either initialize it or do array_manual_collect(true)!"), str("strbuf_split"));
		return NULL;
	}

	// create an array of strbufs
	const size_t count = strbuf_contains(sb, sep) + 1;
	array_pt sbList = array_new(count, sizeof(strbuf_pt));
	if(is_null(sbList)) {
		logger_error(str("unable to create an array! exiting..."), str("strbuf_split"));
		return NULL;
	}

	// split the string by sep
	strbuf_pt sbToken = strbuf(str(strtok((char*)strbuf_cstr(sb), as_cstr(sep))));
	while(!is_null(strbuf_cstr(sbToken))) {
		array_push(sbList, sbToken);
		printf("%s\n", strbuf_cstr(sbToken));
		break;
		//sbToken = strbuf(str(strtok(NULL, as_cstr(sep))));
	}

	/* the problem here is that i need a 2d array, because I'm creating 
	an array of strbuf pointers to arrays of chars
	*/

	return sbList;
}

/*void strbuf_split(const strbuf_pt sb, const str_t s) {
	size_t i = 0;
	char* token = strtok((char*)strbuf_cstr(sb), as_cstr(s));
	while(!is_null(token)) {
		printf("%s\n", token);

		token = strtok(NULL, as_cstr(s));
	}
}*/






























