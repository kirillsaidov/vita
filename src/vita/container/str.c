#include "vita/container/str.h"

const str_t str_make_on_stack(const char *const cs) {
    str_t s = {
        .ptr = (void*)cs,
        .len = strlen(cs),
        .elsize = sizeof(char)
    };

    return s;
}

str_t *str(const char *cs) {
    if(cs == NULL) {
        cs = "";
    }

    // create str
    str_t *s = strn(strlen(cs));
    if(s == NULL) {
        return NULL;
    }

    // set cs to str
    if(str_set(s, cs) != ve_operation_success) {
        str_free(s);
        return NULL;
    }

    return s;
}

str_t *strn(const size_t n) {
    // allocate memory for a str_t struct
    str_t *s = malloc(sizeof(str_t));

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

str_t *str_take_ownership(const char *const cs) {
    if(cs == NULL) {
        return NULL;
    }

    // allocate memory for a str_t struct
    str_t *s = malloc(sizeof(str_t));

    // check if s was allocated
    if(s == NULL) {
        return NULL;
    }

    // str_t init
    const size_t csLen = strlen(cs);
    *s = (str_t) {
        .ptr = (void*)(cs),
        .len = csLen,
        .capacity = csLen,
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

enum VitaError str_set(str_t *const s, const char *cs) {
    return str_set_n(s, cs, strlen(cs));
}

enum VitaError str_set_n(str_t *const s, const char *cs, const size_t n) {
    // error checking
    if(s == NULL || cs == NULL) {
        return ve_error_is_null;
    }

    // check if it has enough space
    if(s->capacity < n) {
        return ve_error_out_of_bounds_access;
    }

    // copy cs data to str_t
    memcpy(s->ptr, cs, (n * s->elsize));

    // add the '\0' terminator
    ((char*)s->ptr)[n] = '\0';

    // update values
    s->len = n;

    return ve_operation_success;
}

enum VitaError str_append(str_t *const s, const char *cs) {
    return str_append_n(s, cs, strlen(cs));
}

enum VitaError str_append_n(str_t *const s, const char *cs, const size_t n) {
    if(s == NULL || cs == NULL) {
        return ve_error_is_null;
    }

    // check if new memory needs to be allocated
    if(str_has_space(s) <= n && str_reserve(s, (n - str_has_space(s))) != ve_operation_success) {
        return ve_error_allocation;
    }

    // copy cs to str_t
    memcpy((s->ptr + s->len * s->elsize), cs, (n * s->elsize));

    // set new length
    s->len += n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

enum VitaError str_insert(str_t *const s, const char *cs, const size_t at) {
    if(s == NULL || cs == NULL) {
        return ve_error_is_null;
    }

    // check if we have out of bounds access
    if(s->len <= at) {
        return ve_error_out_of_bounds_access;
    }

    // check if new memory needs to be allocated
    const size_t csLen = strlen(cs);
    if(str_has_space(s) <= csLen && str_reserve(s, (csLen - str_has_space(s))) != ve_operation_success) {
        return ve_error_allocation;
    }

    // shift the end of string from the specified index `at` to `at + csLen` in str
    memmove((s->ptr + (at + csLen) * s->elsize), (s->ptr + at * s->elsize), ((s->len - at) * s->elsize));

    // copy the str contents to str from the specified index
    memcpy((s->ptr + at * s->elsize), cs, (csLen * s->elsize));

    // set new length
    s->len += csLen;

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

enum VitaError str_remove_str(str_t *const s, const char *cs) {
    if(s == NULL || cs == NULL) {
        return ve_error_is_null;
    }

    // find a substring in strbuf
    const size_t csLen = strlen(cs);
    void* sub = strstr(s->ptr, cs);
    if(sub == NULL) {
        return ve_operation_failure;
    }

    // find how many characters to copy from the end
    const size_t diff = (size_t)((s->ptr + s->len * s->elsize) - (sub + csLen * s->elsize));

    // shift the characters to the left of the string by the substring length
    memmove(sub, (sub + csLen * s->elsize), diff);

    // set new length
    s->len -= csLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return ve_operation_success;
}

size_t str_can_find(const str_t *const s, const char *cs) {
    if(s == NULL || cs == NULL || !str_len(s)) {
        return 0;
    }

    // count the number of substring instances in strbuf
    size_t count = 0;
    const size_t csLen = strlen(cs);
    const char *p = s->ptr;
    while((p = strstr(p + csLen * sizeof(char), cs)) != NULL) {
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
            head += sepLen * s->elsize;
            continue;
        } else {
            // create a str_t instance and copy the values
            str_t *tempStr = strn(copyLen);
            str_set_n(tempStr, head, copyLen);
            plist_push(p, tempStr);

            // update head
            head = current + sepLen * s->elsize;
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
        return NULL;
    }

    // check if s contains sep substring
    const char *const tempStr = strstr(s->ptr, sep);
    if(tempStr == NULL) {
        return NULL;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = str_len(s) - strlen(tempStr);
    if(!copyLen) {
        return NULL;
    }

    // copy the string before the separator
    str_t *spop = ((sr == NULL) ? (strn(copyLen)) : (sr));
    if(spop == NULL) {
        return NULL;
    }
    str_set_n(spop, s->ptr, copyLen);

    // pop the part of the string with the separator
    str_remove(s, 0, copyLen + strlen(sep));

    return spop;
}

str_t *str_pop_get_last(str_t *sr, str_t *const s, const char *const sep) {
    const size_t sepLen = strlen(sep);
    if(s == NULL || sep == NULL || !str_len(s) || !sepLen) {
        return NULL;
    }

    // find the last instance of sep
    const char *p = strstr(s->ptr, sep);
    const char *lastInstance = p;
    while((p = strstr(p + sepLen * s->elsize, sep)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if(lastInstance == NULL) {
        return NULL;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = strlen(lastInstance) - sepLen;
    if(!copyLen) {
        return NULL;
    }

    // create str_t instance
    str_t *spop = ((sr == NULL) ? (strn(copyLen)) : (sr));
    if(spop == NULL) {
        return NULL;
    }

    // copy the string before the separator
    str_set_n(spop, lastInstance + sepLen, copyLen);

    // pop the part of the string with the separator
    str_remove(s, str_len(s) - copyLen - sepLen, copyLen + sepLen);

    return spop;
}

bool str_equals(const char *const cs1, const char *const cs2) {
    const size_t cs1Len = strlen(cs1);
    if(cs1Len > strlen(cs2)) {
        return false;
    }

    return (!strncmp(cs1, cs2, cs1Len));
}

bool str_starts_with(const char *const cs, const char *const cs_sub) {
    const size_t subLen = strlen(cs_sub);
    if(subLen > strlen(cs)) {
        return false;
    }

    return (!strncmp(cs, cs_sub, subLen));
}

bool str_ends_with(const char *const cs, const char *const cs_sub) {
    const size_t csLen = strlen(cs);
    const size_t subLen = strlen(cs_sub);
    if(subLen > csLen) {
        return false;
    }

    return (!strncmp(cs + csLen - subLen, cs_sub, subLen));
}
