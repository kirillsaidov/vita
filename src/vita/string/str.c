#include "vita/string/str.h"

str_t *str(const char *cs) {
	if(is_null(cs)) {
		cs = "";
	}

	// allocate memory for a str_t struct
	str_t *s = malloc(sizeof(str_t));

	// check if s was allocated
	if(is_null(s)) {		
		return NULL;
	}

	// str_t init
	const size_t csLen = strlen(cs);
	*s = (str_t) {
		.ptr = calloc(csLen + 1, sizeof(char)),
		.len = csLen,
		.capacity = csLen,
		.elsize = sizeof(char),
	};

	// checking if s->ptr was allocated
	if(is_null(s->ptr)) {
		free(s);
		return NULL;
	}

	// copy str data to str_t
	memcpy(s->ptr, cs, (csLen * s->elsize));

	// add the '\0' terminator
	((char*)s->ptr)[csLen] = '\0';

	return s;
}

str_t *strn(const size_t n) {
	if(!n) {
		return NULL;
	}

	// allocate memory for a str_t struct
	str_t *s = malloc(sizeof(str_t));

	// check if s was allocated
	if(is_null(s)) {
		return NULL;
	}

	// str_t init
	*s = (str_t) {
		.ptr = calloc(n + 1, sizeof(char)),
		.len = n,
		.capacity = n,
		.elsize = sizeof(char),
	};

	// checking if s->ptr was allocated
	if(is_null(s->ptr)) {
		free(s);
		return NULL;
	}

	// copy str data to str_t
	memset(s->ptr, '\0', n * s->elsize);

	// add the '\0' terminator
	((char*)s->ptr)[n] = '\0';

	return s;
}

str_t *str_dup(const str_t *const s) {
	return str(s->ptr);
}

void str_free(str_t *s) {
	// if NULL, exit
	if(is_null(s)) {
		return;
	}

	// free the str_t string and str_t struct
	free(s->ptr);
	free(s);

	// reset to NULL
	s = NULL;
}







const char *cstr(const str_t *const s) {
	return (const char*)(s->ptr);
}

size_t str_len(const str_t *const s) {
	return s->len;
}

size_t str_capacity(const str_t *const s) {
	return s->capacity;
}

size_t str_has_space(const str_t *const s) {
	return (s->capacity - s->len);
}

bool str_is_empty(const str_t *const s) {
	return !(s->len);
}







bool str_shrink(str_t *const s) {
	if(is_null(s)) {
		return false;
	}

	// if length and capacity are the same, exit the function
	if(s->len == s->capacity) {
		return true;
	}

	// shrink the array capacity to length
	void *newptr = realloc(s->ptr, (s->len + 1) * s->elsize);
	if(is_null(newptr)) {
		return false;
	}

	// update values
	s->ptr = newptr;
	s->capacity = s->len;

	return true;
}

bool str_clear(str_t *const s) {
	if(is_null(s)) {
		return false;
	}

	// set C string to ""
	((char*)s->ptr)[0] = '\0';

	// update length
	s->len = 0;

	return true;
}

bool str_reserve(str_t *const s, const size_t n) {
	if(is_null(s) || !n) {
		return false;
	}

	// reserve memory for additional n elements
	void *newptr = realloc(s->ptr, (s->capacity + n + 1) * s->elsize);
	if(is_null(newptr)) {
		return false;
	}

	// update values
	s->ptr = newptr;
	s->capacity += n;

	// add '\0' terminator at the very end of str_t
	((char*)s->ptr)[s->capacity] = '\0';

	return true;
}

bool str_set(str_t *const s, const char *cs) {
	// error checking
	const size_t csLen = strlen(cs);
	if(is_null(s) || is_null(cs) || (s->capacity < csLen)) {
		return false;
	}

	// copy cs data to str_t
	memcpy(s->ptr, cs, (csLen * s->elsize));

	// add the '\0' terminator
	((char*)s->ptr)[csLen] = '\0';

	// update values
	s->len = csLen;

	return true;
}

bool str_append(str_t *const s, const char *cs) {
	if(is_null(s) || is_null(cs)) {
		return false;
	}

	// check if new memory needs to be allocated
	const size_t csLen = strlen(cs);
	if(str_has_space(s) <= csLen && !str_reserve(s, (csLen - str_has_space(s)))) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// copy cs to str_t
	memcpy((s->ptr + s->len * s->elsize), cs, (csLen * s->elsize));

	// set new length
	s->len += csLen;

	// add the '\0' terminator
	((char*)s->ptr)[s->len] = '\0';

	return true;
}

bool str_insert(str_t *const s, const char *cs, const size_t at) {
	if(is_null(s) || is_null(cs) || (s->len <= at)) {
		return false;
	}

	// check if new memory needs to be allocated
	const size_t csLen = strlen(cs);
	if(str_has_space(s) <= csLen && !str_reserve(s, (csLen - str_has_space(s)))) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// shift the end of string from the specified index `at` to `at + csLen` in str
	memmove((s->ptr + (at + csLen) * s->elsize), (s->ptr + at * s->elsize), ((s->len - at) * s->elsize));

	// copy the str contents to str from the specified index
	memcpy((s->ptr + at * s->elsize), cs, (csLen * s->elsize));

	// set new length
	s->len += csLen;

	// add the '\0' terminator
	((char*)s->ptr)[s->len] = '\0';

	return true;
}

bool str_remove(str_t *const s, const size_t from, const size_t n) {
	if(is_null(s) || (s->len <= from) || (n > s->len - from)) {
		return false;
	}

	// shift the characters in string from index `from + n` to `from` in strbuf
	memcpy((s->ptr + from * s->elsize), (s->ptr + (from + n) * s->elsize), ((s->len - (from + n)) * s->elsize));

	// set new length
	s->len -= n;

	// add the '\0' terminator
	((char*)s->ptr)[s->len] = '\0';

	return true;
}

bool str_remove_str(str_t *const s, const char *cs) {
	if(is_null(s) || is_null(cs)) {
		return false;
	}

	// find a substring in strbuf
	const size_t csLen = strlen(cs);
	void* sub = strstr(s->ptr, cs);
	if(is_null(sub)) {
		return false;
	}

	// find how many characters to copy from the end
	const size_t diff = (size_t)((s->ptr + s->len * s->elsize) - (sub + csLen * s->elsize));

	// shift the characters to the left of the string by the substring length
	memcpy(sub, (sub + csLen * s->elsize), diff);

	// set new length
	s->len -= csLen;

	// add the '\0' terminator
	((char*)s->ptr)[s->len] = '\0';

	return true;
}

size_t str_contains(const str_t *const s, const char *cs) {
	if(is_null(s) || is_null(cs)) {
		return 0;
	}
		
	// count the number of substring instances in strbuf
	size_t count = 0;
	const size_t csLen = strlen(cs);
	const char *p = strstr(s->ptr, cs);
	while(p != NULL) {
		count++;
		p = strstr(p + csLen * s->elsize, s->ptr);
	}
	
	return count;
}

/*vec_t *str_split(const str_t *const s, const char *sep) {
	if(is_null(s) || is_null(sep)) {
		return NULL;
	}

	// check if s contains sep substring
	const size_t strInstances = str_contains(s, sep);
	if(!strInstances) {
		return NULL;
	}

	// create a vec_t instance
	vec_t *v = vec(strInstances + 1, sizeof(str_t));

	// seperate strings
	const size_t sepLen = strlen(sep);
	const char *previous = s->ptr;
	const char *current = strstr(s->ptr, sep);
	while(current != NULL) {
		// save current position
		previous = current;

		// update new position
		current = strstr(current + sepLen * s->elsize, sep);

		// count copy length
		size_t tempLen = strlen(previous) - (is_null(current) ? 0 : strlen(current));

		// create a str_t instance and copy the values
		str_t *tempStr = strn(tempLen + 1);
		strncpy(tempStr->ptr, previous + sepLen, tempLen);

		// push str_t to vec_t
		vec_push(v, tempStr);
	}

	return v;
}*/ // i believe i need mat_t here

bool str_equals(const char *cs1, const char *cs2) {
	return (!strncmp(cs1, cs2, strlen(cs1)));
}






























