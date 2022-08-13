#include "vita/container/str.h"

// static functions for internal usage
static str_t *str_vfmt(str_t *s, const char *const fmt, va_list args);

const str_t str_make_on_stack(const char *const z) {
    str_t s = {
        .ptr = (void*)z,
        .len = strlen(z),
        .elsize = sizeof(char)
    };

    return s;
}

str_t *str(const char *z) {
    if(z == NULL) {
        z = "";
    }

    // create str
    str_t *s = strn(strlen(z));
    if(s == NULL) {
        return NULL;
    }

    // set z to str
    if(str_set(s, z) != ve_operation_success) {
        str_free(s);
        return NULL;
    }

    return s;
}

str_t *str_fmt(str_t *s, const char *const fmt, ...) {
    if(fmt == NULL || (s == NULL && (s = strn(DEFAULT_INIT_ELEMENTS)) == NULL)) {
        return s;
    }

    // clear str_t
    str_clear(s);

    // append all data
    va_list args; va_start(args, fmt);
    s = str_vfmt(s, fmt, args);
    va_end(args);

    return s;
}

str_t *strn(const size_t n) {
    // allocate memory for a str_t struct
    str_t *s = calloc(1, sizeof(str_t));

    // check if s was allocated
    if(s == NULL) {
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
    if(s->ptr == NULL) {
        free(s);
        return NULL;
    }

    // copy str data to str_t
    memset(s->ptr, '\0', (n + 1) * s->elsize);

    return s;
}

str_t *str_dup(const str_t *const s) {
    return str(s->ptr);
}

str_t *str_take_ownership(const char *const z) {
    if(z == NULL) {
        return NULL;
    }

    // allocate memory for a str_t struct
    str_t *s = calloc(1, sizeof(str_t));

    // check if s was allocated
    if(s == NULL) {
        return NULL;
    }

    // str_t init
    const size_t zLen = strlen(z);
    *s = (str_t) {
        .ptr = (void*)(z),
        .len = zLen,
        .capacity = zLen,
        .elsize = sizeof(char),
    };

    return s;
}

void str_free(str_t *s) {
    // if NULL, exit
    if(s == NULL) {
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
    return (s == NULL) ? 0 : s->len;
}

size_t str_capacity(const str_t *const s) {
    return (s == NULL) ? 0 : s->capacity;
}

size_t str_has_space(const str_t *const s) {
    return (s == NULL) ? 0 : (s->capacity - s->len);
}

bool str_is_empty(const str_t *const s) {
    return !(s->len);
}

enum VitaError str_shrink(str_t *const s) {
    if(s == NULL) {
        return ve_error_is_null;
    }

    // if length and capacity are the same, exit the function
    if(s->len == s->capacity) {
        return ve_operation_success;
    }

    // shrink the array capacity to length
    void *newptr = realloc(s->ptr, (s->len + 1) * s->elsize);
    if(newptr == NULL) {
        return ve_error_allocation;
    }

    // update values
    s->ptr = newptr;
    s->capacity = s->len;

    // add '\0' terminator at the very end of str_t
    ((char*)s->ptr)[s->capacity] = '\0';

    return ve_operation_success;
}

enum VitaError str_clear(str_t *const s) {
    if(s == NULL) {
        return ve_error_is_null;
    }

    // set C string to ""
    ((char*)s->ptr)[0] = '\0';

    // update length
    s->len = 0;

    return ve_operation_success;
}

enum VitaError str_reserve(str_t *const s, const size_t n) {
    if(s == NULL || !n) {
        return ve_error_is_null;
    }

    // reserve memory for additional n elements
    void *newptr = realloc(s->ptr, (s->capacity + n + 1) * s->elsize);
    if(newptr == NULL) {
        return ve_error_allocation;
    }

    // update values
    s->ptr = newptr;
    s->capacity += n;

    // add '\0' terminator at the very end of str_t
    ((char*)s->ptr)[s->capacity] = '\0';

    return ve_operation_success;
}

enum VitaError str_set(str_t *const s, const char *z) {
    return str_set_n(s, z, strlen(z));
}

enum VitaError str_set_n(str_t *const s, const char *z, const size_t n) {
    // error checking
    if(s == NULL || z == NULL) {
        return ve_error_is_null;
    }

    // check if it has enough space
    if(s->capacity < n) {
        return ve_error_out_of_bounds_access;
    }

    // copy z data to str_t
    memcpy(s->ptr, z, (n * s->elsize));

    // add the '\0' terminator
    ((char*)s->ptr)[n] = '\0';

    // update values
    s->len = n;

    return ve_operation_success;
}

enum VitaError str_append(str_t *const s, const char *z) {
    return str_append_n(s, z, strlen(z));
}

enum VitaError str_appendf(str_t *const s, const char *const fmt, ...) {
    if(s == NULL || fmt == NULL) {
        return ve_error_is_null;
    }

    // iterate over all arguments
    va_list args; va_start(args, fmt);
    str_vfmt(s, fmt, args);
    va_end(args);

    return ve_operation_success;
}

enum VitaError str_append_n(str_t *const s, const char *z, const size_t n) {
    if(s == NULL || z == NULL) {
        return ve_error_is_null;
    }

    // check if new memory needs to be allocated
    if(str_has_space(s) <= n && str_reserve(s, (n - str_has_space(s))) != ve_operation_success) {
        return ve_error_allocation;
    }

    // copy z to str_t
    memcpy((s->ptr + s->len * s->elsize), z, (n * s->elsize));

    // set new length
    s->len += n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError str_insert(str_t *const s, const char *z, const size_t at) {
    if(s == NULL || z == NULL) {
        return ve_error_is_null;
    }

    // check if we have out of bounds access
    if(s->len <= at) {
        return ve_error_out_of_bounds_access;
    }

    // check if new memory needs to be allocated
    const size_t zLen = strlen(z);
    if(str_has_space(s) <= zLen && str_reserve(s, (zLen - str_has_space(s))) != ve_operation_success) {
        return ve_error_allocation;
    }

    // shift the end of string from the specified index `at` to `at + zLen` in str
    memmove((s->ptr + (at + zLen) * s->elsize), (s->ptr + at * s->elsize), ((s->len - at) * s->elsize));

    // copy the str contents to str from the specified index
    memcpy((s->ptr + at * s->elsize), z, (zLen * s->elsize));

    // set new length
    s->len += zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError str_remove(str_t *const s, const size_t from, size_t n) {
    if(s == NULL) {
        return ve_error_is_null;
    }

    // check if we have out of bounds access
    if(s->len <= from) {
        return ve_error_out_of_bounds_access;
    }

    // check if we need to remove all chars until the end
    if(n > s->len - from) {
        n = s->len - from;
    }

    // shift the characters in string from index `from + n` to `from` in strbuf
    memmove((s->ptr + from * s->elsize), (s->ptr + (from + n) * s->elsize), ((s->len - (from + n)) * s->elsize));

    // set new length
    s->len -= n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError str_remove_first(str_t *const s, const char *z) {
    const size_t zLen = strlen(z);
    if(s == NULL || z == NULL || !zLen) {
        return ve_error_is_null;
    }

    // find a substring in strbuf; if substring wasn't found, return
    void* sub = strstr(s->ptr, z);
    if(sub == NULL) {
        return ve_operation_element_not_found;
    }

    // find how many characters to copy from the end
    const size_t diff = (size_t)((s->ptr + s->len * s->elsize) - (sub + zLen * s->elsize));

    // shift the characters to the left of the string by the substring length
    memmove(sub, (sub + zLen * s->elsize), diff * s->elsize);

    // set new length
    s->len -= zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError str_remove_last(str_t *s, const char *const z) {
    const size_t zLen = strlen(z);
    if(s == NULL || z == NULL || !zLen) {
        return ve_error_is_null;
    }

    // find the last instance of sep
    char *p = strstr(s->ptr, z);
    char *lastInstance = p;
    while((p = strstr(p + zLen, z)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if(lastInstance == NULL) {
        return ve_operation_success;
    }

    // find how many characters to shrink the string
    const size_t diff = (size_t)((s->ptr + s->len * s->elsize) - ((void*)lastInstance));

    // set z to zero where it begins
    lastInstance[0]= '\0';

    // update str_t
    s->len -= diff - 1;

    return ve_operation_success;
}

enum VitaError str_remove_all(str_t *const s, const char *z) {
    if(s == NULL || z == NULL) {
        return ve_error_is_null;
    }

    // remove all instances of substring
    while(str_remove_first(s, z) == ve_operation_success);

    return ve_operation_success;
}

enum VitaError str_remove_c(str_t *const s, const char *const c) {
    if (s == NULL || c == NULL) {
        return ve_error_is_null;
    }

    // prepare
    size_t offset = 0;
    size_t last_index = 0;
    size_t *slen = &s->len;
    size_t clen = strlen(c);
    char *start = s->ptr;
    char *sdup = strdup(start);

    // remove all specitified characters
    bool copy = true;
    for(size_t i = 0; i < *slen; i++) {
        for(size_t j = 0; j < clen; j++) {
            if(start[i] == c[j]) {
                offset++;

                // do not copy characters
                copy = false;
                break;
            }
        }
        
        // copy data
        if(copy) {
            last_index = i - offset;
            sdup[last_index] = start[i];
        } else {
            copy = true;
        }
    }
    
    // update length
    *slen -= offset;

    // update ptr data
    free(start);
    sdup[*slen] = '\0';
    s->ptr = sdup;
    
    return ve_operation_success;
}

enum VitaError str_strip(str_t *const s) {
    if(s == NULL) {
        return ve_error_is_null;
    }

    // prepare
    size_t offset = 0;
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while(isspace(*curr)) {
        offset++;
        curr = p + offset;
    }

    // update length
    *len -= offset;

    // strip tailing whitespace and control symbols
    curr = p + (*len + offset - 1);
    while(isspace(*curr)) {
        (*len)--;
        curr = p + (*len + offset - 1);
    }

    // move string to the begining
    memmove(s->ptr, s->ptr + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return ve_operation_success;
}

enum VitaError str_strip_punct(str_t *const s) {
    if(s == NULL) {
        return ve_error_is_null;
    }

    // prepare
    size_t offset = 0;
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while(isspace(*curr) || ispunct(*curr)) {
        offset++;
        curr = p + offset;
    }

    // update length
    *len -= offset;

    // strip tailing whitespace and control symbols
    curr = p + (*len + offset - 1);
    while(isspace(*curr) || ispunct(*curr)) {
        (*len)--;
        curr = p + (*len + offset - 1);
    }

    // move string to the begining
    memmove(s->ptr, s->ptr + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return ve_operation_success;
}

enum VitaError str_strip_c(str_t *const s, const char *const c) {
    if(s == NULL) {
        return ve_error_is_null;
    }

    // prepare
    bool stop = false;
    size_t offset = 0;
    size_t clen = strlen(c);
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while(!stop) {
        for(size_t i = 0; i < clen; i++) {
            if(*curr == c[i]) {
                offset++;
                curr = p + offset;

                // continue looping
                stop = false;

                break;
            }

            stop = true;
        }
    }

    // update length
    *len -= offset;

    // strip tailing whitespace and control symbols
    stop = false;
    curr = p + (*len + offset - 1);
    while(!stop) {
        for(size_t i = 0; i < clen; i++) {
            if(*curr == c[i]) {
                (*len)--;
                curr = p + (*len + offset - 1);

                // continue looping
                stop = false;

                break;
            }

            stop = true;
        }
    }

    // move string to the begining
    memmove(s->ptr, s->ptr + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return ve_operation_success;
}

const char *str_find(const char *const z, const char *zsub) {
    if(z == NULL || zsub == NULL) {
        return NULL;
    }

    return strstr(z, zsub);
}

size_t str_can_find(const str_t *const s, const char *z) {
    if(s == NULL || z == NULL || !str_len(s)) {
        return 0;
    }

    // count the number of substring instances in strbuf
    size_t count = 0;
    const size_t zLen = strlen(z);
    const char *p = s->ptr;
    while((p = strstr(p + zLen, z)) != NULL) {
        count++;
    }

    return count;
}

plist_t *str_split(plist_t *ps, const str_t *const s, const char *const sep) {
    if(s == NULL || sep == NULL) {
        return NULL;
    }

    // check if s contains sep substring
    const size_t strInstances = str_can_find(s, sep);
    if(!strInstances) {
        return NULL;
    }

    // create a vec_t instance
    plist_t *p = NULL;
    if(ps == NULL) {
        p = plist_create(strInstances + 1);
        if(p == NULL) {
            return NULL;
        }
    } else {
        p = ps;
        if(plist_has_space(p) < strInstances + 1 && plist_reserve(p, (strInstances + 1) - plist_has_space(p)) != ve_operation_success) {
            return p;
        }
    }
    plist_clear(p);

    // splitting the raw C string
    const size_t sepLen = strlen(sep);
    const char *head = s->ptr;
    const char *current = head;
    while(1) {
        // find sep
        const char *current = strstr(head, sep);

        // count copy length
        size_t copyLen = strlen(head) - (current == NULL ? 0 : strlen(current));
        if(copyLen == 0) { // if head == current, move head by sepLen (if sep is in the begining)
            head += sepLen;
            continue;
        } else {
            // create a str_t instance and copy the values
            str_t *tempStr = strn(copyLen);
            str_set_n(tempStr, head, copyLen);
            plist_push(p, tempStr);

            // update head
            head = current + sepLen;
        }

        // break from loop when no more sep is found or we are at the end of string
        if(current == NULL || current[sepLen] == '\0') {
            break;
        }
    }

    return p;
}

str_t *str_pop_get_first(str_t *sr, str_t *const s, const char *const sep) {
    if(s == NULL || sep == NULL || !str_len(s) || !strlen(sep)) {
        return sr;
    }

    // check if s contains sep substring
    const char *const tempStr = strstr(s->ptr, sep);
    if(tempStr == NULL) {
        return sr;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = str_len(s) - strlen(tempStr);
    if(!copyLen) {
        return sr;
    }

    // create str_t instance
    str_t *spop = ((sr == NULL) ? (strn(copyLen)) : (sr));
    if(spop == NULL) {
        return sr;
    }

    // if not enough space, reserve more
    if(str_len(spop) < copyLen) {
        str_reserve(spop, copyLen - str_len(spop) + 1);
    }

    // copy the string before the separator
    str_set_n(spop, s->ptr, copyLen);

    // pop the part of the string with the separator
    str_remove(s, 0, copyLen + strlen(sep));

    return spop;
}

str_t *str_pop_get_last(str_t *sr, str_t *const s, const char *const sep) {
    const size_t sepLen = strlen(sep);
    if(s == NULL || sep == NULL || !str_len(s) || !sepLen) {
        return sr;
    }

    // find the last instance of sep
    const char *p = strstr(s->ptr, sep);
    const char *lastInstance = p;
    while((p = strstr(p + sepLen, sep)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if(lastInstance == NULL) {
        return sr;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = strlen(lastInstance) - sepLen;
    if(!copyLen) {
        return sr;
    }

    // create str_t instance
    str_t *spop = ((sr == NULL) ? (strn(copyLen)) : (sr));
    if(spop == NULL) {
        return sr;
    }

    // if not enough space, reserve more
    if(str_len(spop) < copyLen) {
        str_reserve(spop, copyLen - str_len(spop) + 1);
    }

    // copy the string after the separator
    str_set_n(spop, lastInstance + sepLen, copyLen);

    // pop the part of the string with the separator
    str_remove(s, str_len(s) - copyLen - sepLen, copyLen + sepLen);

    return spop;
}

bool str_equals(const char *const z1, const char *const z2) {
    const size_t cs1Len = strlen(z1);
    if(cs1Len > strlen(z2)) {
        return false;
    }

    return (!strncmp(z1, z2, cs1Len));
}

bool str_starts_with(const char *const z, const char *const sub) {
    const size_t subLen = strlen(sub);
    if(subLen > strlen(z)) {
        return false;
    }

    return (!strncmp(z, sub, subLen));
}

bool str_ends_with(const char *const z, const char *const sub) {
    const size_t zLen = strlen(z);
    const size_t subLen = strlen(sub);
    if(subLen > zLen) {
        return false;
    }

    return (!strncmp(z + zLen - subLen, sub, subLen));
}

void str_apply(const str_t *const s, void (*func)(char*, size_t)) {
    if(s == NULL || func == NULL) {
        return;
    }

    const size_t len = str_len(s);
    for(size_t i = 0; i < len; i++) {
        func(&((char*)s->ptr)[i], i);
    }
}

static str_t *str_vfmt(str_t *s, const char *const fmt, va_list args) {
    if(s == NULL || fmt == NULL) {
        return s;
    }

    // format string
    va_list args2; va_copy(args2, args); 
    {
        // check if new memory needs to be allocated
        const int32_t len = vsnprintf(NULL, (size_t)0, fmt, args);
        if(str_has_space(s) <= len && str_reserve(s, (len - str_has_space(s))) != ve_operation_success) {
            return s;
        }

        // print data to s
        vsprintf(s->ptr, fmt, args2);
    } 
    va_end(args2);

    return s;
}

bool str_is_numeric(const char *const z, const size_t max_len) {
    if(z == NULL || !max_len) {
        return false;
    }

    // check if string is a number
    const size_t zLen = strnlen(z, max_len);
    for(size_t i = 0; i < zLen; i++) {
        if(!isdigit(z[i]) && z[i] != '.') {
            return false;
        }
    }

    return true;
}

void str_capitalize(str_t *const s) {
    if(s == NULL) {
        return;
    }

    // check if string is a number
    char *const z = s->ptr;
    const size_t zLen = str_len(s);
    for(size_t i = 0; i < zLen; i++) {
        z[i] = toupper(z[i]);
    }
}


