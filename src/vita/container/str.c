#include "vita/container/str.h"

// static functions for internal usage
static vt_str_t *vt_str_vfmt(vt_str_t *s, const char *const fmt, va_list args);

vt_str_t vt_str_make_on_stack(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // create vt_str_t instance
    vt_str_t s = {
        .ptr = (void*)z,
        .len = strlen(z),
        .elsize = sizeof(char)
    };

    return s;
}

vt_str_t *vt_str(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // create str
    vt_str_t *s = vt_strn(strlen(z));
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return NULL;
    }

    // set z to str
    if(vt_str_set(s, z) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("Failed to copy \"%s\" to vt_str_t!", z);
        vt_str_free(s);
        return NULL;
    }

    return s;
}

vt_str_t *vt_str_fmt(vt_str_t *s, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // allocate s if s == NULL
    if(s == NULL && (s = vt_strn(VT_ARRAY_DEFAULT_INIT_ELEMENTS)) == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return s;
    }

    // clear vt_str_t
    if(vt_str_clear(s) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s: Failed to clear out the string!\n", vt_get_vita_error_str(vt_status_operation_failure));
        return s;
    }

    // append all data
    va_list args; va_start(args, fmt);
    s = vt_str_vfmt(s, fmt, args);
    va_end(args);

    return s;
}

vt_str_t *vt_strn(const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // allocate memory for a vt_str_t struct
    vt_str_t *s = VT_DEBUG_CALLOC(sizeof(vt_str_t));

    // check if s was allocated
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return NULL;
    }

    // vt_str_t init
    *s = (vt_str_t) {
        .ptr = VT_DEBUG_CALLOC((n + 1) * sizeof(char)),
        .len = n,
        .capacity = n,
        .elsize = sizeof(char),
    };

    // checking if s->ptr was allocated
    if(s->ptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        VT_DEBUG_FREE(s);
        return NULL;
    }

    // copy str data to vt_str_t
    memset(s->ptr, '\0', (n + 1) * s->elsize);

    return s;
}

vt_str_t *vt_strn_empty(const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // create and clear vt_str_t
    vt_str_t *s = vt_strn(n);
    vt_str_clear(s);

    return s;
}

vt_str_t *vt_str_dup(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    return vt_str(s->ptr);
}

vt_str_t *vt_str_take_ownership(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // allocate memory for a vt_str_t struct
    vt_str_t *s = VT_DEBUG_CALLOC(sizeof(vt_str_t));

    // check if s was allocated
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return NULL;
    }

    // vt_str_t init
    const size_t zLen = strlen(z);
    *s = (vt_str_t) {
        .ptr = (void*)(z),
        .len = zLen,
        .capacity = zLen,
        .elsize = sizeof(char),
    };

    return s;
}

void vt_str_free(vt_str_t *s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // free the vt_str_t string and vt_str_t struct
    VT_DEBUG_FREE(s->ptr);
    VT_DEBUG_FREE(s);

    // reset to NULL
    s = NULL;
}

const char *vt_cstr(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    return (const char*)(s->ptr);
}

size_t vt_str_len(const vt_str_t *const s) {
    return vt_bat_len(s);
}

size_t vt_str_capacity(const vt_str_t *const s) {
    return vt_bat_capacity(s);
}

size_t vt_str_has_space(const vt_str_t *const s) {
    return vt_bat_has_space(s);
}

bool vt_str_is_empty(const vt_str_t *const s) {
    return !vt_bat_len(s);
}

size_t vt_str_validate_len(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // calculate valid length
    const size_t s_len = strnlen(s->ptr, s->capacity);

    // reset
    ((char*)s->ptr)[s_len] = '\0';
    s->len = s_len;

    return s_len;
}

enum VitaStatus vt_str_shrink(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // if length and capacity are the same, exit the function
    if(s->len == s->capacity) {
        return vt_status_operation_success;
    }

    // shrink the array capacity to length
    void *newptr = VT_DEBUG_REALLOC(s->ptr, (s->len + 1) * s->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return vt_status_error_allocation;
    }

    // update values
    s->ptr = newptr;
    s->capacity = s->len;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[s->capacity] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_clear(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // set C string to ""
    ((char*)s->ptr)[0] = '\0';

    // update length
    s->len = 0;

    return vt_status_operation_success;
}

enum VitaStatus vt_str_reserve(vt_str_t *const s, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // reserve memory for additional n elements
    void *newptr = VT_DEBUG_REALLOC(s->ptr, (s->capacity + n + 1) * s->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return vt_status_error_allocation;
    }

    // update values
    s->ptr = newptr;
    s->capacity += n;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[s->capacity] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_set(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    return vt_str_set_n(s, z, strlen(z));
}

enum VitaStatus vt_str_set_n(vt_str_t *const s, const char *z, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // check if it has enough space
    if(s->capacity < n) {
        VT_DEBUG_PRINTF(
            "%s: Supplied string length is %zu, but vt_str_t length is %zu!\n", 
            vt_get_vita_error_str(vt_status_error_out_of_memory), 
            n, 
            s->len
        );
        return vt_status_error_out_of_memory;
    }

    // copy z data to vt_str_t
    memmove(s->ptr, z, (n * s->elsize));

    // add the '\0' terminator
    ((char*)s->ptr)[n] = '\0';

    // update values
    s->len = n;

    return vt_status_operation_success;
}

enum VitaStatus vt_str_append(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    return vt_str_append_n(s, z, strlen(z));
}

enum VitaStatus vt_str_appendf(vt_str_t *const s, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // iterate over all arguments
    va_list args; va_start(args, fmt); 
    if(vt_str_vfmt(s, fmt, args) == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));
        return vt_status_operation_failure;
    }
    va_end(args);

    return vt_status_operation_success;
}

enum VitaStatus vt_str_append_n(vt_str_t *const s, const char *z, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // check if new memory needs to be allocated
    if(vt_str_has_space(s) < n && vt_str_reserve(s, (n - vt_str_has_space(s))) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return vt_status_error_allocation;
    }

    // copy z to vt_str_t
    memmove((char*)(s->ptr) + s->len * s->elsize, z, n * s->elsize);

    // set new length
    s->len += n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_insert(vt_str_t *const s, const char *z, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at <= s->len,
        "%s: Inserts at %zu, but vt_str_t length is %zu!\n", 
        vt_get_vita_error_str(vt_status_error_out_of_bounds_access), 
        at, 
        s->len
    );

    // check if new memory needs to be allocated
    const size_t zLen = strlen(z);
    const size_t hasSpace = vt_str_has_space(s);
    if(hasSpace < zLen && vt_str_reserve(s, (zLen - hasSpace)) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return vt_status_error_allocation;
    }

    // shift the end of string from the specified index `at` to `at + zLen` in str
    memmove((char*)(s->ptr) + (at + zLen) * s->elsize, (char*)(s->ptr) + at * s->elsize, (s->len - at) * s->elsize);

    // copy the str contents to str from the specified index
    memmove((char*)(s->ptr) + at * s->elsize, z, zLen * s->elsize);

    // set new length
    s->len += zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_remove(vt_str_t *const s, const size_t from, size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        from <= s->len,
        "%s: Start from %zu, but vt_str_t length is %zu!\n", 
        vt_get_vita_error_str(vt_status_error_out_of_bounds_access), 
        from, 
        s->len
    );

    // check if we need to remove all chars until the end
    if(n > s->len - from) {
        n = s->len - from;
    }

    // shift the characters in string from index `from + n` to `from` in strbuf
    memmove((char*)(s->ptr) + from * s->elsize, (char*)(s->ptr) + (from + n) * s->elsize, (s->len - (from + n)) * s->elsize);

    // set new length
    s->len -= n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_remove_first(vt_str_t *const s, const char *z) {
    const size_t zLen = strlen(z);

    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // find a substring in strbuf; if substring wasn't found, return
    char* sub = strstr(s->ptr, z);
    if(sub == NULL) {
        return vt_status_error_element_not_found;
    }

    // find how many characters to copy from the end
    // it cannot be negative, since the substring will always be to the right of the begining
    const size_t diff = (size_t)(((char*)(s->ptr) + s->len * s->elsize) - (sub + zLen * s->elsize));

    // shift the characters to the left of the string by the substring length
    memmove(sub, (sub + zLen * s->elsize), diff * s->elsize);

    // set new length
    s->len -= zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_remove_last(vt_str_t *s, const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // find the last instance of sep
    const size_t zLen = strlen(z);
    char *p = strstr(s->ptr, z);
    char *lastInstance = p;
    while((p = strstr(p + zLen, z)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if(lastInstance == NULL) {
        return vt_status_error_element_not_found;
    }

    // find how many characters to shrink the string
    // it cannot be negative, since the substring will always be to the right of the begining
    const size_t diff = (size_t)(((char*)(s->ptr) + s->len * s->elsize) - lastInstance);

    // set z to zero where it begins
    lastInstance[0]= '\0';

    // update vt_str_t
    s->len -= diff - 1;

    return vt_status_operation_success;
}

enum VitaStatus vt_str_remove_all(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // remove all instances of substring
    while(vt_str_remove_first(s, z) == vt_status_operation_success);

    return vt_status_operation_success;
}

enum VitaStatus vt_str_remove_c(vt_str_t *const s, const char *const c) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(c != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // prepare
    size_t offset = 0;
    size_t last_index = 0;
    size_t *sLen = &s->len;
    size_t cLen = strlen(c);
    char *start = s->ptr;
    char *sdup = strdup(start);

    // remove all specitified characters
    bool copy = true;
    for(size_t i = 0; i < *sLen; i++) {
        for(size_t j = 0; j < cLen; j++) {
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
    *sLen -= offset;

    // update ptr data
    VT_DEBUG_FREE(start);
    sdup[*sLen] = '\0';
    s->ptr = sdup;
    
    return vt_status_operation_success;
}

enum VitaStatus vt_str_strip(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

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
    memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_strip_punct(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

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
    memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return vt_status_operation_success;
}

enum VitaStatus vt_str_strip_c(vt_str_t *const s, const char *const c) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(c != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // prepare
    bool stop = false;
    size_t offset = 0;
    size_t cLen = strlen(c);
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while(!stop) {
        for(size_t i = 0; i < cLen; i++) {
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
        for(size_t i = 0; i < cLen; i++) {
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
    memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return vt_status_operation_success;
}

const char *vt_str_find(const char *const z, const char *sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    return strstr(z, sub);
}

size_t vt_str_can_find(const vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // count the number of substring instances in strbuf
    size_t count = 0;
    const size_t zLen = strlen(z);
    const char *p = s->ptr;
    while((p = strstr(p + zLen, z)) != NULL) {
        count++;
    }

    return count;
}

vt_plist_t *vt_str_split(vt_plist_t *ps, const vt_str_t *const s, const char *const sep) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // check if s contains sep substring
    const size_t strInstances = vt_str_can_find(s, sep);
    if(!strInstances) {
        return NULL;
    }

    // create a vec_t instance
    vt_plist_t *p = NULL;
    if(ps == NULL) {
        p = vt_plist_create(strInstances + 1);
        if(p == NULL) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
            return NULL;
        }
    } else {
        p = ps;
        if(vt_plist_has_space(p) < strInstances + 1 && vt_plist_reserve(p, (strInstances + 1) - vt_plist_has_space(p)) != vt_status_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
            return p;
        }
    }
    
    // clear
    if(vt_plist_clear(p) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));

        // free resource upon failure
        if(ps == NULL) { 
            vt_plist_free(p); 
        }

        return NULL;
    }

    // splitting the raw C string
    const size_t sepLen = strlen(sep);
    const char *head = s->ptr;
    const char *current = head;
    while(true) {
        // find sep
        current = strstr(head, sep);

        // count copy length
        const size_t copyLen = strlen(head) - (current == NULL ? 0 : strlen(current));
        if(copyLen == 0) { // if head == current, move head by sepLen (if sep is in the begining)
            head += sepLen;
            continue;
        } else {
            // create a vt_str_t instance and copy the values
            vt_str_t *tempStr = vt_strn(copyLen);

            // set the value and push it to the list
            if(vt_str_set_n(tempStr, head, copyLen) != vt_status_operation_success || vt_plist_push(p, tempStr) != vt_status_operation_success) {
                VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));
                return p;
            }

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

vt_str_t *vt_str_split_between(const char *const z, const char *const zl, const char *const zr) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(zl != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(zr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // find zl - left substring
    const char *const lsub = vt_str_find(z, zl);
    if(lsub == NULL) {
        return NULL;
    }

    // find r - right substring
    const char *const rsub = vt_str_find(z, zr);
    if(rsub == NULL) {
        return NULL;
    }

    // check if lsub < rsub
    if(lsub < rsub) {
        ptrdiff_t sub_len = rsub - lsub - strlen(zl);
        vt_str_t *st = vt_strn_empty(sub_len);
        if(st == NULL) {
            return NULL;
        }

        // append sub
        if(vt_str_append_n(st, rsub - sub_len, sub_len) != vt_status_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));

            vt_str_free(st);
            st = NULL;

            return NULL;
        }

        return st;
    }

    return NULL;
}

vt_str_t *vt_str_join(vt_str_t *const s, const char *const sep, const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(p != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(p->ptr2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // create vt_str_t if needed
    vt_str_t *st = ((s == NULL) ? (vt_strn(VT_ARRAY_DEFAULT_INIT_ELEMENTS)) : (s));
    if(st == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return NULL;
    }

    // append the first part
    if(vt_str_append(st, vt_plist_get(p, 0)) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));

        // if s is NULL, free st (this is done to avoid freeing user's s instance)
        if(s == NULL) {
            vt_str_free(st);
        }

        return NULL;
    }

    // continue appending
    const size_t pLen = vt_plist_len(p);
    for(size_t i = 1; i < pLen; i++) {
        if(vt_str_append(st, sep) != vt_status_operation_success && vt_str_append(st, vt_plist_get(p, i)) != vt_status_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));
            
            // if s is NULL, free st (this is done to avoid freeing user's s instance)
            if(s == NULL) {
                vt_str_free(st);
            }

            return NULL;
        }
    }

    return st;
}

vt_str_t *vt_str_join_n(vt_str_t *const s, const char *const sep, const size_t n, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // create a new vt_str_t instance and append
    vt_str_t *st = ((s == NULL) ? (vt_strn(VT_ARRAY_DEFAULT_INIT_ELEMENTS)) : (s));
    if(st == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
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
        if(vt_str_append(st, z) != vt_status_operation_success) { 
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));

            // if s is NULL, free st (this is done to avoid freeing user's s instance)
            if(s == NULL) {
                vt_str_free(st);
            }

            return st; 
        }

        // do not append path separator at the very end
        if(i < n-1 && vt_str_append(st, sep) != vt_status_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));
            
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

vt_str_t *vt_str_pop_get_first(vt_str_t *sr, vt_str_t *const s, const char *const sep) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    const size_t sepLen = strlen(sep);
    if(!vt_str_len(s) || !sepLen) {
        return sr;
    }

    // check if s contains sep substring
    const char *const tempStr = strstr(s->ptr, sep);
    if(tempStr == NULL) {
        return sr;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = vt_str_len(s) - strlen(tempStr);
    if(!copyLen) {
        return sr;
    }

    // create vt_str_t instance
    vt_str_t *spop = ((sr == NULL) ? (vt_strn(copyLen)) : (sr));
    if(spop == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return sr;
    }
    vt_str_clear(spop);

    // if not enough space, reserve more
    if(vt_str_len(spop) < copyLen && vt_str_reserve(spop, copyLen - vt_str_len(spop)) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return spop;
    }

    // copy the string before the separator
    vt_str_set_n(spop, s->ptr, copyLen);

    // pop the part of the string with the separator
    vt_str_remove(s, 0, copyLen + sepLen);

    return spop;
}

vt_str_t *vt_str_pop_get_last(vt_str_t *sr, vt_str_t *const s, const char *const sep) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    const size_t sepLen = strlen(sep);
    if(!vt_str_len(s) || !sepLen) {
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

    // create vt_str_t instance
    vt_str_t *spop = ((sr == NULL) ? (vt_strn(copyLen)) : (sr));
    if(spop == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return sr;
    }

    // if not enough space, reserve more
    if(vt_str_len(spop) < copyLen && vt_str_reserve(spop, copyLen - vt_str_len(spop) + 1) != vt_status_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
        return spop;
    }

    // copy the string after the separator
    vt_str_set_n(spop, lastInstance + sepLen, copyLen);

    // pop the part of the string with the separator
    vt_str_remove(s, vt_str_len(s) - copyLen - sepLen, copyLen + sepLen);

    return spop;
}

bool vt_str_equals(const char *const z1, const char *const z2) {
    // check for invalid input
    VT_DEBUG_ASSERT(z1 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(z2 != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    const size_t z1Len = strlen(z1);
    if(z1Len > strlen(z2)) {
        return false;
    }

    return !strncmp(z1, z2, z1Len);
}

bool vt_str_starts_with(const char *const z, const char *const sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    const size_t subLen = strlen(sub);
    if(subLen > strlen(z)) {
        return false;
    }

    return !strncmp(z, sub, subLen);
}

bool vt_str_ends_with(const char *const z, const char *const sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    const size_t zLen = strlen(z);
    const size_t subLen = strlen(sub);
    if(subLen > zLen) {
        return false;
    }

    return !strncmp(z + zLen - subLen, sub, subLen);
}

void vt_str_apply(const vt_str_t *const s, void (*func)(char*, size_t)) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(func != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    const size_t len = vt_str_len(s);
    for(size_t i = 0; i < len; i++) {
        func(&((char*)s->ptr)[i], i);
    }
}

bool vt_str_is_numeric(const char *const z, const size_t max_len) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(max_len > 0, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // check if string is a number
    const size_t zLen = strnlen(z, max_len);
    for(size_t i = 0; i < zLen; i++) {
        if(!isdigit(z[i]) && z[i] != '.') {
            return false;
        }
    }

    return true;
}

void vt_str_capitalize(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    // check if string is a number
    char *const z = s->ptr;
    const size_t zLen = vt_str_len(s);
    for(size_t i = 0; i < zLen; i++) {
        z[i] = toupper(z[i]);
    }
}

int64_t vt_str_index_of(const vt_str_t *const s, const char z) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    const char *const ztmp = strchr(s->ptr, z);
    if(ztmp == NULL) {
        return -1;
    }

    return (int64_t)(ztmp - (char*)(s->ptr));
}

char *vt_str_slide_front(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    return (char*)vt_bat_slide_front(s);
}

char *vt_str_slide_back(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    return (char*)vt_bat_slide_back(s);
}

void vt_str_slide_reset(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));

    vt_bat_slide_reset(s);
}

// -------------------------- PRIVATE -------------------------- //

/** Prints a formatted string into a vt_str_t
    @param s vt_str_t instance
    @param fmt string print format
    @param args variable args list

    @returns `vt_str_t` instance upon success, NULL upon failure
*/
static vt_str_t *vt_str_vfmt(vt_str_t *s, const char *const fmt, va_list args) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_is_null));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_get_vita_error_str(vt_status_error_invalid_arguments));

    // format string
    va_list args2; va_copy(args2, args); 
    {
        // check if new memory needs to be allocated
        const int64_t len = vsnprintf(NULL, (size_t)0, fmt, args);
        if(len < 0) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_operation_failure));
            return NULL;
        }

        // check for space
        const size_t hasSpace = vt_str_has_space(s);
        if(hasSpace < (size_t)len && vt_str_reserve(s, (len - hasSpace)) != vt_status_operation_success) {
            VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_status_error_allocation));
            return NULL;
        }

        // print data to s
        vsprintf(s->ptr, fmt, args2);
    } 
    va_end(args2);

    return s;
}

