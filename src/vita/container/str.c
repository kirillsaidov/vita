#include "vita/container/str.h"

// static functions for internal usage
static vt_str_t *vt_str_vfmt(vt_str_t *s, const char *const fmt, va_list args);

vt_str_t vt_str_make_on_stack(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

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
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // create str
    vt_str_t *s = vt_strn(strlen(z));
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // set z to str
    if(vt_str_set(s, z) != ve_operation_success) {
        VT_DEBUG_PRINTF("Failed to copy \"%s\" to vt_str_t!", z);
        vt_str_free(s);
        return NULL;
    }

    return s;
}

vt_str_t *vt_str_fmt(vt_str_t *s, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate s if s == NULL
    if(s == NULL && (s = vt_strn(DEFAULT_INIT_ELEMENTS)) == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return s;
    }

    // clear vt_str_t
    if(vt_str_clear(s) != vt_ve_operation_success) {
        VT_DEBUG_PRINTF("Failed to clear out the string!\n");
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
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate memory for a vt_str_t struct
    vt_str_t *s = VT_DEBUG_CALLOC(sizeof(vt_str_t));

    // check if s was allocated
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
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
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        VT_DEBUG_FREE(s);
        return NULL;
    }

    // copy str data to vt_str_t
    memset(s->ptr, '\0', (n + 1) * s->elsize);

    return s;
}

vt_str_t *vt_strn_empty(const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // create and clear vt_str_t
    vt_str_t *s = vt_strn(n);
    vt_str_clear(s);

    return s;
}

vt_str_t *vt_str_dup(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return vt_str(s->ptr);
}

vt_str_t *vt_str_take_ownership(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate memory for a vt_str_t struct
    vt_str_t *s = VT_DEBUG_CALLOC(sizeof(vt_str_t));

    // check if s was allocated
    if(s == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
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
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // free the vt_str_t string and vt_str_t struct
    VT_DEBUG_FREE(s->ptr);
    VT_DEBUG_FREE(s);

    // reset to NULL
    s = NULL;
}

const char *vt_zvt_str(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return (const char*)(s->ptr);
}

size_t vt_str_len(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return s->len;
}

size_t vt_str_capacity(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return s->capacity;
}

size_t vt_str_has_space(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return (s->capacity - s->len);
}

bool vt_str_is_empty(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(s->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return !(s->len);
}

enum VitaError vt_str_shrink(vt_str_t *const s) {
    if(s == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        return ve_error_is_null;
    }

    // if length and capacity are the same, exit the function
    if(s->len == s->capacity) {
        return ve_operation_success;
    }

    // shrink the array capacity to length
    void *newptr = VT_DEBUG_REALLOC(s->ptr, (s->len + 1) * s->elsize);
    if(newptr == NULL) {
        VT_DEBUG_ASSERT(newptr != NULL, "Failed to reallocate memory for vt_str_t!");
        return ve_error_allocation;
    }

    // update values
    s->ptr = newptr;
    s->capacity = s->len;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[s->capacity] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_clear(vt_str_t *const s) {
    if(s == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        return ve_error_is_null;
    }

    // set C string to ""
    ((char*)s->ptr)[0] = '\0';

    // update length
    s->len = 0;

    return ve_operation_success;
}

enum VitaError vt_str_reserve(vt_str_t *const s, const size_t n) {
    if(s == NULL || !n) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(n, "Invalied reserve amount supplied: %zu!", n);
        return ve_error_is_null;
    }

    // reserve memory for additional n elements
    void *newptr = VT_DEBUG_REALLOC(s->ptr, (s->capacity + n + 1) * s->elsize);
    if(newptr == NULL) {
        VT_DEBUG_ASSERT(newptr != NULL, "Failed to reallocate memory for vt_str_t!");
        return ve_error_allocation;
    }

    // update values
    s->ptr = newptr;
    s->capacity += n;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[s->capacity] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_set(vt_str_t *const s, const char *z) {
    return vt_str_set_n(s, z, strlen(z));
}

enum VitaError vt_str_set_n(vt_str_t *const s, const char *z, const size_t n) {
    // error checking
    if(s == NULL || z == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        return ve_error_is_null;
    }

    // check if it has enough space
    if(s->capacity < n) {
        VT_DEBUG_ASSERT(n < s->capacity, "Unable to set the string. String length is %zu, but vt_str_t length is %zu!", n, s->len);
        return ve_error_out_of_bounds_access;
    }

    // copy z data to vt_str_t
    memmove(s->ptr, z, (n * s->elsize));

    // add the '\0' terminator
    ((char*)s->ptr)[n] = '\0';

    // update values
    s->len = n;

    return ve_operation_success;
}

enum VitaError vt_str_append(vt_str_t *const s, const char *z) {
    return vt_str_append_n(s, z, strlen(z));
}

enum VitaError vt_str_appendf(vt_str_t *const s, const char *const fmt, ...) {
    if(s == NULL || fmt == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(fmt != NULL, "Formatting supplied is NULL!");
        return ve_error_is_null;
    }

    // iterate over all arguments
    va_list args; va_start(args, fmt);
    vt_str_vfmt(s, fmt, args);
    va_end(args);

    return ve_operation_success;
}

enum VitaError vt_str_append_n(vt_str_t *const s, const char *z, const size_t n) {
    if(s == NULL || z == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        return ve_error_is_null;
    }

    // check if new memory needs to be allocated
    if(vt_str_has_space(s) < n && vt_str_reserve(s, (n - vt_str_has_space(s))) != ve_operation_success) {
        VT_DEBUG_ASSERT(0, "Failed to reserve memory for vt_str_t!");
        return ve_error_allocation;
    }

    // copy z to vt_str_t
    memmove((char*)(s->ptr) + s->len * s->elsize, z, n * s->elsize);

    // set new length
    s->len += n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_insert(vt_str_t *const s, const char *z, const size_t at) {
    if(s == NULL || z == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        return ve_error_is_null;
    }

    // check if we have out of bounds access
    if(s->len <= at) {
        VT_DEBUG_ASSERT(at < s->len, "Unable to insert the string at %zu, because vt_str_t length is %zu!", at, s->len);
        return ve_error_out_of_bounds_access;
    }

    // check if new memory needs to be allocated
    const size_t zLen = strlen(z);
    if(vt_str_has_space(s) < zLen && vt_str_reserve(s, (zLen - vt_str_has_space(s))) != ve_operation_success) {
        VT_DEBUG_ASSERT(0, "Failed to reserve memory for vt_str_t!");
        return ve_error_allocation;
    }

    // shift the end of string from the specified index `at` to `at + zLen` in str
    memmove((char*)(s->ptr) + (at + zLen) * s->elsize, (char*)(s->ptr) + at * s->elsize, (s->len - at) * s->elsize);

    // copy the str contents to str from the specified index
    memmove((char*)(s->ptr) + at * s->elsize, z, zLen * s->elsize);

    // set new length
    s->len += zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_remove(vt_str_t *const s, const size_t from, size_t n) {
    if(s == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        return ve_error_is_null;
    }

    // check if we have out of bounds access
    if(s->len <= from) {
        VT_DEBUG_ASSERT(from < s->len, "Unable to remove the string from %zu, because vt_str_t length is %zu!", from, s->len);
        return ve_error_out_of_bounds_access;
    }

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

    return ve_operation_success;
}

enum VitaError vt_str_remove_first(vt_str_t *const s, const char *z) {
    const size_t zLen = strlen(z);
    if(s == NULL || z == NULL || !zLen) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        VT_DEBUG_ASSERT(zLen, "Supplied string length is 0!");
        return ve_error_is_null;
    }

    // find a substring in strbuf; if substring wasn't found, return
    char* sub = strvt_str(s->ptr, z);
    if(sub == NULL) {
        return ve_operation_element_not_found;
    }

    // find how many characters to copy from the end
    const size_t diff = (size_t)(((char*)(s->ptr) + s->len * s->elsize) - (sub + zLen * s->elsize));

    // shift the characters to the left of the string by the substring length
    memmove(sub, (sub + zLen * s->elsize), diff * s->elsize);

    // set new length
    s->len -= zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_remove_last(vt_str_t *s, const char *const z) {
    const size_t zLen = strlen(z);
    if(s == NULL || z == NULL || !zLen) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        VT_DEBUG_ASSERT(zLen, "Supplied string length is 0!");
        return ve_error_is_null;
    }

    // find the last instance of sep
    char *p = strvt_str(s->ptr, z);
    char *lastInstance = p;
    while((p = strvt_str(p + zLen, z)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if(lastInstance == NULL) {
        return ve_operation_success;
    }

    // find how many characters to shrink the string
    const size_t diff = (size_t)(((char*)(s->ptr) + s->len * s->elsize) - lastInstance);

    // set z to zero where it begins
    lastInstance[0]= '\0';

    // update vt_str_t
    s->len -= diff - 1;

    return ve_operation_success;
}

enum VitaError vt_str_remove_all(vt_str_t *const s, const char *z) {
    if(s == NULL || z == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        return ve_error_is_null;
    }

    // remove all instances of substring
    while(vt_str_remove_first(s, z) == ve_operation_success);

    return ve_operation_success;
}

enum VitaError vt_str_remove_c(vt_str_t *const s, const char *const c) {
    if (s == NULL || c == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(c != NULL, "Tokens supplied string is NULL!");
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
    VT_DEBUG_FREE(start);
    sdup[*slen] = '\0';
    s->ptr = sdup;
    
    return ve_operation_success;
}

enum VitaError vt_str_strip(vt_str_t *const s) {
    if(s == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
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
    memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_strip_punct(vt_str_t *const s) {
    if(s == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
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
    memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return ve_operation_success;
}

enum VitaError vt_str_strip_c(vt_str_t *const s, const char *const c) {
    if(s == NULL || c == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(c != NULL, "Tokens supplied string is NULL!");
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
    memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';

    return ve_operation_success;
}

const char *vt_str_find(const char *const z, const char *zsub) {
    if(z == NULL || zsub == NULL) {
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        VT_DEBUG_ASSERT(zsub != NULL, "Substring supplied is NULL!");
        return NULL;
    }

    return strvt_str(z, zsub);
}

size_t vt_str_can_find(const vt_str_t *const s, const char *z) {
    if(s == NULL || z == NULL || !vt_str_len(s)) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(z != NULL, "Substring supplied is NULL!");
        return 0;
    }

    // count the number of substring instances in strbuf
    size_t count = 0;
    const size_t zLen = strlen(z);
    const char *p = s->ptr;
    while((p = strvt_str(p + zLen, z)) != NULL) {
        count++;
    }

    return count;
}

plist_t *vt_str_split(plist_t *ps, const vt_str_t *const s, const char *const sep) {
    if(s == NULL || sep == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(sep != NULL, "Separator string supplied is NULL!");
        return NULL;
    }

    // check if s contains sep substring
    const size_t strInstances = vt_str_can_find(s, sep);
    if(!strInstances) {
        return NULL;
    }

    // create a vec_t instance
    plist_t *p = NULL;
    if(ps == NULL) {
        p = plist_create(strInstances + 1);
        if(p == NULL) {
            VT_DEBUG_ASSERT(p != NULL, "Failed to allocate plist_t instance!");
            return NULL;
        }
    } else {
        p = ps;
        if(plist_has_space(p) < strInstances + 1 && plist_reserve(p, (strInstances + 1) - plist_has_space(p)) != ve_operation_success) {
            VT_DEBUG_ASSERT(0, "Failed to reserve more memory for vt_str_t!");
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
        current = strvt_str(head, sep);

        // count copy length
        size_t copyLen = strlen(head) - (current == NULL ? 0 : strlen(current));
        if(copyLen == 0) { // if head == current, move head by sepLen (if sep is in the begining)
            head += sepLen;
            continue;
        } else {
            // create a vt_str_t instance and copy the values
            vt_str_t *tempStr = vt_strn(copyLen);
            vt_str_set_n(tempStr, head, copyLen);
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

// CONTINUE FROM HERE
vt_str_t *vt_str_pop_get_first(vt_str_t *sr, vt_str_t *const s, const char *const sep) {
    if(s == NULL || sep == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(sep != NULL, "Separator string supplied is NULL!");
        return sr;
    }

    if(!vt_str_len(s) || !strlen(sep)) {
        return sr;
    }

    // check if s contains sep substring
    const char *const tempStr = strvt_str(s->ptr, sep);
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
        VT_DEBUG_ASSERT(spop != NULL, "Failed to allocate vt_str_t instance!");
        return sr;
    }
    vt_str_clear(spop);

    // if not enough space, reserve more
    if(vt_str_len(spop) < copyLen) {
        vt_str_reserve(spop, copyLen - vt_str_len(spop));
    }

    // // copy the string before the separator
    vt_str_set_n(spop, s->ptr, copyLen);

    // pop the part of the string with the separator
    vt_str_remove(s, 0, copyLen + strlen(sep));

    return spop;
}

vt_str_t *vt_str_pop_get_last(vt_str_t *sr, vt_str_t *const s, const char *const sep) {
    const size_t sepLen = strlen(sep);
    if(s == NULL || sep == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(sep != NULL, "Separator string supplied is NULL!");
        return sr;
    }

    if(!vt_str_len(s) || !sepLen) {
        return sr;
    }

    // find the last instance of sep
    const char *p = strvt_str(s->ptr, sep);
    const char *lastInstance = p;
    while((p = strvt_str(p + sepLen, sep)) != NULL) {
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
        return sr;
    }

    // if not enough space, reserve more
    if(vt_str_len(spop) < copyLen) {
        vt_str_reserve(spop, copyLen - vt_str_len(spop) + 1);
    }

    // copy the string after the separator
    vt_str_set_n(spop, lastInstance + sepLen, copyLen);

    // pop the part of the string with the separator
    vt_str_remove(s, vt_str_len(s) - copyLen - sepLen, copyLen + sepLen);

    return spop;
}

bool vt_str_equals(const char *const z1, const char *const z2) {
    const size_t z1Len = strlen(z1);
    if(z1Len > strlen(z2)) {
        return false;
    }

    return (!vt_strncmp(z1, z2, z1Len));
}

bool vt_str_starts_with(const char *const z, const char *const sub) {
    const size_t subLen = strlen(sub);
    if(subLen > strlen(z)) {
        return false;
    }

    return (!vt_strncmp(z, sub, subLen));
}

bool vt_str_ends_with(const char *const z, const char *const sub) {
    const size_t zLen = strlen(z);
    const size_t subLen = strlen(sub);
    if(subLen > zLen) {
        return false;
    }

    return (!vt_strncmp(z + zLen - subLen, sub, subLen));
}

void vt_str_apply(const vt_str_t *const s, void (*func)(char*, size_t)) {
    if(s == NULL || func == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(func != NULL, "Supplied func is NULL!");
        return;
    }

    const size_t len = vt_str_len(s);
    for(size_t i = 0; i < len; i++) {
        func(&((char*)s->ptr)[i], i);
    }
}

bool vt_str_is_numeric(const char *const z, const size_t max_len) {
    if(z == NULL || !max_len) {
        VT_DEBUG_ASSERT(z != NULL, "String supplied is NULL!");
        VT_DEBUG_ASSERT(max_len, "max_len cannot be 0!");
        return false;
    }

    // check if string is a number
    const size_t zLen = vt_strnlen(z, max_len);
    for(size_t i = 0; i < zLen; i++) {
        if(!isdigit(z[i]) && z[i] != '.') {
            return false;
        }
    }

    return true;
}

void vt_str_capitalize(vt_str_t *const s) {
    if(s == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        return;
    }

    // check if string is a number
    char *const z = s->ptr;
    const size_t zLen = vt_str_len(s);
    for(size_t i = 0; i < zLen; i++) {
        z[i] = toupper(z[i]);
    }
}

int64_t vt_str_index_of(const vt_str_t *const s, const char z) {
    const char *const ztmp = strchr(s->ptr, z);
    if(ztmp == NULL) {
        return -1;
    }

    return (int64_t)(ztmp - (char*)(s->ptr));
}

// -------------------------- PRIVATE -------------------------- //

static vt_str_t *vt_str_vfmt(vt_str_t *s, const char *const fmt, va_list args) {
    if(s == NULL || fmt == NULL) {
        VT_DEBUG_ASSERT(s != NULL, "vt_str_t instance was not initialized!");
        VT_DEBUG_ASSERT(fmt != NULL, "Formatting supplied is NULL!");
        return s;
    }

    // format string
    va_list args2; va_copy(args2, args); 
    {
        // check if new memory needs to be allocated
        const int64_t len = vsnprintf(NULL, (size_t)0, fmt, args);
        if(len < 0) {
            VT_DEBUG_ASSERT(0, "Encoding error occured!");
            return NULL;
        }

        // check for space
        if(vt_str_has_space(s) < (size_t)len && vt_str_reserve(s, (len - vt_str_has_space(s))) != ve_operation_success) {
            VT_DEBUG_ASSERT(0, "Failed to reserve more memory for vt_str_t!");
            return s;
        }

        // print data to s
        vsprintf(s->ptr, fmt, args2);
    } 
    va_end(args2);

    return s;
}

