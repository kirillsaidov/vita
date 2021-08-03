#include "vita/string/strbuf.h"

// strbuf struct
struct StrBuf {
    char* buf;

    size_t len;
    size_t capacity;
};

// strbuf internal memory handler
static memhandler_pt strbufMemhandlerInternal = NULL;

// strbuf memory management strategy
static bool strbufManualCollect = false;

void strbuf_manual_collect(const bool status) {
	strbufManualCollect = status;
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
	// create a strbuf struct
	strbuf_pt sb = ((strbufManualCollect) ? 
		(mem_malloc(1, sizeof(struct StrBuf))) : 
		(memhandler_malloc(strbufMemhandlerInternal, 1, sizeof(struct StrBuf)))
	);

	// checking if sb was allocated
	if(is_null(sb)) {
		logger_error(str("unable to create a strbuf struct!"), str("strbuf"));
		return NULL;
	}

	// create strbuf string
	*sb = (struct StrBuf) {
		.buf = ((strbufManualCollect) ? 
			(mem_malloc((s.len + s.len/3 + 1), sizeof(char))) : 
			(memhandler_calloc(strbufMemhandlerInternal, (s.len + s.len/3 + 1), sizeof(char)))),
		.len = s.len,
		.capacity = s.len + s.len/3
	};

	// checking if sb->buf was allocated
	if(is_null(sb->buf)) {
		logger_error(str("unable to create a strbuf string!"), str("strbuf"));
		return NULL;
	}

	// copy str data to strbuf
	memcpy(sb->buf, as_cstr(s), (s.len * sizeof(char)));
	sb->buf[s.len] = '\0';

	return sb;
}

strbuf_pt strbuf_new(const str_t s) {
	return strbuf(s);
}

void strbuf_free(strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL, safely exiting..."), str("strbuf_free"));
		return;
	}

	// free the strbuf string and strbuf struct
	if(strbufManualCollect) {
		mem_free(sb->buf);
		mem_free(sb);
	} else {
		memhandler_free(strbufMemhandlerInternal, sb->buf);
		memhandler_free(strbufMemhandlerInternal, sb);
	}
}







const char* strbuf_cstr(const strbuf_pt sb) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; returning NULL..."), str("strbuf_cstr"));
		return NULL;
	}

	return sb->buf;
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

	// shrink the array capacity to length
	bool success = ((strbufManualCollect) ? 
		(mem_realloc((void**)&sb->buf, (sb->len + 1), sizeof(char))) : 
		(memhandler_realloc(strbufMemhandlerInternal, (void**)&sb->buf, (sb->len + 1), sizeof(char)))
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

	// shrink the array capacity to length
	bool success = ((strbufManualCollect) ? 
		(mem_realloc((void**)&sb->buf, (sb->capacity + n + 1), sizeof(char))) : 
		(memhandler_realloc(strbufMemhandlerInternal, (void**)&sb->buf, (sb->capacity + n + 1), sizeof(char)))
	);
	

	// check if the operation was successfull
	if(!success) {
		logger_error(str("cannot reserve more memory for the string!"), str("strbuf_reserve"));
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
		memcpy((sb->buf + sb->len * sizeof(char)), as_cstr(s), (s.len * sizeof(char)));

		// set new length
		sb->len += s.len;

		// add the '\0' terminator
		sb->buf[sb->len] = '\0';
	}

	return success;
}

bool strbuf_insert(strbuf_pt sb, const str_t s, const size_t fromIndex) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_insert"));
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
		memmove((sb->buf + (fromIndex + s.len) * sizeof(char)), (sb->buf + fromIndex * sizeof(char)), ((sb->len - fromIndex) * sizeof(char)));

		// copy the str contents to strbuf from the specified index
		memcpy((sb->buf + fromIndex * sizeof(char)), as_cstr(s), (s.len * sizeof(char)));

		// set new length
		sb->len += s.len;

		// add the '\0' terminator
		sb->buf[sb->len] = '\0';
	}

	return success;
}

bool strbuf_remove(strbuf_pt sb, const size_t fromIndex, const size_t n) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_remove"));
		return false;
	}

	// shift the characters in string from index `fromIndex + n` to `fromIndex` in strbuf
	memcpy((sb->buf + fromIndex * sizeof(char)), (sb->buf + (fromIndex + n) * sizeof(char)), ((sb->len - (fromIndex + n)) * sizeof(char)));

	// set new length
	sb->len -= n;

	// add the '\0' terminator
	sb->buf[sb->len] = '\0';

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
	sb->buf[sb->len] = '\0';

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

size_t strbuf_split(const strbuf_pt sb, const str_t s) {
	//...
	
	return 0;
}






























