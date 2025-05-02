#include "vita/container/str.h"

static vt_str_t *vt_str_vfmt_set(vt_str_t *s, const char *const fmt, va_list args);
static vt_str_t *vt_str_vfmt_append(vt_str_t *s, const char *const fmt, va_list args);

vt_str_t vt_str_create_static(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create vt_str_t instance
    vt_str_t s = {
        .ptr = (void*)z,
        .len = strlen(z),
        .elsize = sizeof(char),
        .is_view = true,
    };

    return s;
}

vt_str_t *vt_str_create(const char *const z, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create str
    const size_t zLen = strlen(z);
    vt_str_t *s = vt_str_create_len(zLen, alloctr);

    // set z to str
    vt_str_set_n(s, z, zLen);

    return s;
}

vt_str_t *vt_str_create_n(const char *const z, const size_t n, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create str
    vt_str_t *s = vt_str_create_len(n, alloctr);

    // set z to str
    vt_str_set_n(s, z, n);

    return s;
}

vt_str_t *vt_str_create_len(const size_t n, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // allocate memory for a vt_str_t struct
    vt_str_t *s = vt_array_new(alloctr);
    *s = (vt_str_t) {
        .alloctr = alloctr,
        .ptr = alloctr ? VT_ALLOCATOR_ALLOC(alloctr, (n + 1) * sizeof(char)) : VT_CALLOC((n + 1) * sizeof(char)),
        .len = n,
        .capacity = n,
        .elsize = sizeof(char),
    };

    // default initiaze it to whitespace
    vt_memset(s->ptr, '?', s->len);

    return s;
}

vt_str_t *vt_str_create_capacity(const size_t n, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create and clear vt_str_t
    vt_str_t *s = vt_str_create_len(n, alloctr);
    vt_str_clear(s);

    return s;
}

void vt_str_destroy(vt_str_t *s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // if vt_str_t is view, skip
    if (s->is_view) {
        return;
    }

    // free the vt_str_t string and vt_str_t struct
    if (s->alloctr) {
        VT_ALLOCATOR_FREE(s->alloctr, s->ptr);
    } else {
        VT_FREE(s->ptr);
    }
    s->ptr = NULL;

    // reset to NULL
    vt_array_free(s);
    s = NULL;
}

vt_str_t *vt_str_dup(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    
    return vt_str_create_n(s->ptr, s->len, s->alloctr);
}

vt_str_t *vt_str_take_ownership(const char *const z, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // allocate memory for a vt_str_t struct
    vt_str_t *s = vt_array_new(alloctr);

    // init
    const size_t zLen = strlen(z);
    *s = (vt_str_t) {
        .alloctr = alloctr,
        .ptr = (void*)(z),
        .len = zLen,
        .capacity = zLen,
        .elsize = sizeof(char),
    };

    return s;
}

const char *vt_str_z(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    return (const char*)(s->ptr);
}

size_t vt_str_len(const vt_str_t *const s) {
    return vt_array_len(s);
}

size_t vt_str_capacity(const vt_str_t *const s) {
    return vt_array_capacity(s);
}

size_t vt_str_has_space(const vt_str_t *const s) {
    return vt_array_has_space(s);
}

bool vt_str_is_empty(const vt_str_t *const s) {
    return !vt_array_len(s);
}

size_t vt_str_validate_len(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // calculate valid length
    const size_t max_len = s->capacity ? s->capacity - 1 : 0;
    const size_t s_len = strnlen(s->ptr, max_len);

    // reset
    ((char*)s->ptr)[s_len] = '\0';
    s->len = s_len;

    return s_len;
}

void vt_str_shrink(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a read-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // if length and capacity are the same, exit the function
    if (s->len == s->capacity) {
        return;
    }

    // shrink the array capacity to length
    s->ptr = s->alloctr 
        ? VT_ALLOCATOR_REALLOC(s->alloctr, s->ptr, (s->len + 1) * s->elsize) 
        : VT_REALLOC(s->ptr, (s->len + 1) * s->elsize);

    // update
    s->capacity = s->len;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[s->capacity] = '\0';
}

void vt_str_clear(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a read-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // set C string to ""
    ((char*)s->ptr)[0] = '\0';

    // update length
    s->len = 0;
}

void vt_str_reserve(vt_str_t *const s, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a read-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // reserve memory for additional n elements
    s->ptr = s->alloctr 
        ? VT_ALLOCATOR_REALLOC(s->alloctr, s->ptr, (s->capacity + n + 1) * s->elsize) 
        : VT_REALLOC(s->ptr, (s->capacity + n + 1) * s->elsize);

    // update
    s->capacity += n;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[s->capacity] = '\0';
}

void vt_str_resize(vt_str_t *const s, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a read-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check length to avoid unnecessary reallocations
    // reserve more memory if needed
    if (vt_str_len(s) < n) {
        vt_str_reserve(s, n - vt_str_len(s));
    }

    // update
    s->len = n;
    s->capacity = n;

    // add '\0' terminator at the very end of vt_str_t
    ((char*)s->ptr)[n] = '\0';
}

void vt_str_set(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    vt_str_set_n(s, z, strlen(z));
}

enum VitaStatus vt_str_set_at(vt_str_t *const s, const char *z, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < s->len,
        "%s: Assigning at %zu, but vt_str_t length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        s->len
    );

    // ensure that z fits into vt_str_t
    const size_t zLen = strlen(z);
    VT_ENFORCE(
        zLen <= s->len - at,
        "%s: Supplied string length is %zu, but available space is %zu!\n",
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_MEMORY),
        zLen,
        s->len - at
    );

    // remove data from `at` till end
    vt_str_remove(s, at, s->len - at);

    // append new string
    vt_str_append_n(s, z, zLen);

    return VT_STATUS_OPERATION_SUCCESS;
}

enum VitaStatus vt_str_set_c(vt_str_t *const s, const char c, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(
        at < s->len,
        "%s: Assigning at %zu, but vt_str_t length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        s->len
    );
    
    // add the '\0' terminator
    ((char*)s->ptr)[at] = c;

    return VT_STATUS_OPERATION_SUCCESS;
}

void vt_str_set_n(vt_str_t *const s, const char *z, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // ensure that z fits into vt_str_t
    VT_ENFORCE(
        n <= s->len,
        "%s: Supplied string length is %zu, but available space is %zu!\n",
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_MEMORY),
        n,
        s->len
    );

    // copy z data to vt_str_t
    vt_memmove(s->ptr, z, (n * s->elsize));

    // add the '\0' terminator
    ((char*)s->ptr)[n] = '\0';

    // update values
    s->len = n;
}

void vt_str_append(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    vt_str_append_n(s, z, strlen(z));
}

enum VitaStatus vt_str_appendf(vt_str_t *const s, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // iterate over all arguments
    va_list args; 
    va_start(args, fmt); 
    if (vt_str_vfmt_append(s, fmt, args) == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_STATUS_OPERATION_FAILURE;
    }
    va_end(args);

    return VT_STATUS_OPERATION_SUCCESS;
}

void vt_str_append_n(vt_str_t *const s, const char *z, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check if new memory needs to be allocated
    if (vt_str_has_space(s) < n) {
        vt_str_reserve(s, (n - vt_str_has_space(s)));
    }

    // copy z to vt_str_t
    vt_memmove((char*)(s->ptr) + s->len * s->elsize, z, n * s->elsize);

    // set new length
    s->len += n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';
}

void vt_str_insert(vt_str_t *const s, const char *z, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < s->len,
        "%s: Inserts at %zu, but vt_str_t length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        s->len
    );
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check if new memory needs to be allocated
    const size_t zLen = strlen(z);
    const size_t hasSpace = vt_str_has_space(s);
    if (hasSpace < zLen) {
        vt_str_reserve(s, (zLen - hasSpace));
    }

    // shift the end of string from the specified index `at` to `at + zLen` in str
    vt_memmove((char*)(s->ptr) + (at + zLen) * s->elsize, (char*)(s->ptr) + at * s->elsize, (s->len - at) * s->elsize);

    // copy the str contents to str from the specified index
    vt_memmove((char*)(s->ptr) + at * s->elsize, z, zLen * s->elsize);

    // set new length
    s->len += zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';
}

enum VitaStatus vt_str_insertf(vt_str_t *const s, const size_t at, const char *const fmt, ...) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // iterate over all arguments
    va_list args; va_start(args, fmt); 
    {
        // check format length
        int64_t len = 0;
        va_list args2; va_copy(args2, args); 
        {
            len = vsnprintf(NULL, (size_t)0, fmt, args2);
        } 
        va_end(args2);

        // check for error
        if (len < 0) {
            VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            return VT_STATUS_OPERATION_FAILURE;
        }

        // if format length is small, use stack memory
        if (len < VT_STR_TMP_BUFFER_SIZE - 1) {
            char tmp_buf[VT_STR_TMP_BUFFER_SIZE] = {0};

            // print formatted string to tmp_buf
            vsprintf(tmp_buf, fmt, args);

            // insert tmp_buf to vt_str_t
            vt_str_insert(s, tmp_buf, at);
        } else {
            // append data to a temporary vt_str_t instance
            vt_str_t *tmp_str = vt_str_create_len(len, s->alloctr);
            if (vt_str_vfmt_set(tmp_str, fmt, args) == NULL) {
                VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
                return VT_STATUS_OPERATION_FAILURE;
            }

            // insert tmp_str
            vt_str_insert(s, vt_str_z(tmp_str), at);

            // free tmp_str
            vt_str_destroy(tmp_str);
        }
    } 
    va_end(args);

    return VT_STATUS_OPERATION_SUCCESS;
}

void vt_str_insert_n(vt_str_t *const s, const char *z, const size_t at, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check if new memory needs to be allocated
    const size_t hasSpace = vt_str_has_space(s);
    if (hasSpace < n) {
        vt_str_reserve(s, (n - hasSpace));
    }

    // move everyting between [at; at + n) by n
    vt_memmove((char*)(s->ptr) + (at + n) * s->elsize, (char*)(s->ptr) + at * s->elsize, (s->len - at) * s->elsize);

    // insert n elements of z between [at; at + n)
    vt_memmove((char*)(s->ptr) + at * s->elsize, z, n * s->elsize);

    // set new length
    s->len += n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';
}

void vt_str_insert_before(vt_str_t *const s, const char *const sub, const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // find the substring
    const int64_t idx = vt_str_index_find(s, sub);
    if (idx < 0) {
        return;
    }

    // insert z
    vt_str_insert(s, z, idx);
}

void vt_str_insert_after(vt_str_t *const s, const char *const sub, const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // find the substring
    const int64_t idx = vt_str_index_find(s, sub);
    if (idx < 0) {
        return;
    }

    // insert z
    vt_str_insert(s, z, idx + strlen(sub));
}

void vt_str_remove(vt_str_t *const s, const size_t from, size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        from < s->len,
        "%s: Start from %zu, but vt_str_t length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        from, 
        s->len
    );

    // check if we need to remove all chars until the end
    if (n > s->len - from) {
        n = s->len - from;
    }

    // shift the characters in string from index `from + n` to `from` in strbuf
    vt_memmove((char*)(s->ptr) + from * s->elsize, (char*)(s->ptr) + (from + n) * s->elsize, (s->len - (from + n)) * s->elsize);

    // set new length
    s->len -= n;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';
}

enum VitaStatus vt_str_remove_first(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // find a substring in strbuf; if substring wasn't found, return
    char* sub = strstr(s->ptr, z);
    if (sub == NULL) {
        return VT_STATUS_ERROR_ELEMENT_NOT_FOUND;
    }

    // find how many characters to copy from the end
    // it cannot be negative, since the substring will always be to the right of the begining
    const size_t zLen = strlen(z);
    const size_t diff = (size_t)(((char*)(s->ptr) + s->len * s->elsize) - (sub + zLen * s->elsize));

    // shift the characters to the left of the string by the substring length
    vt_memmove(sub, (sub + zLen * s->elsize), diff * s->elsize);

    // set new length
    s->len -= zLen;

    // add the '\0' terminator
    ((char*)s->ptr)[s->len] = '\0';

    return VT_STATUS_OPERATION_SUCCESS;
}

enum VitaStatus vt_str_remove_last(vt_str_t *s, const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // find the last instance of sep
    const size_t zLen = strlen(z);
    char *p = strstr(s->ptr, z);
    char *lastInstance = p;
    while ((p = strstr(p + zLen, z)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if (lastInstance == NULL) {
        return VT_STATUS_ERROR_ELEMENT_NOT_FOUND;
    }

    // find the position of last substring instance
    const ptrdiff_t last_instance_pos = (((char*)(s->ptr) + s->len * s->elsize) - lastInstance);

    // remove the last instance
    vt_str_remove(s, last_instance_pos, zLen);

    return VT_STATUS_OPERATION_SUCCESS;
}

enum VitaStatus vt_str_remove_all(vt_str_t *const s, const char *z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // remove all instances of substring
    while (vt_str_remove_first(s, z) == VT_STATUS_OPERATION_SUCCESS);

    return VT_STATUS_OPERATION_SUCCESS;
}

void vt_str_remove_c(vt_str_t *const s, const char *const c) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(c != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // prepare
    size_t offset = 0;
    size_t last_index = 0;
    size_t *sLen = &s->len;
    size_t cLen = strlen(c);
    char *start = s->ptr;

    // remove all specitified characters
    bool copy = true;
    for (size_t i = 0; i < *sLen; i++) {
        for (size_t j = 0; j < cLen; j++) {
            if (start[i] == c[j]) {
                offset++;

                // do not copy characters
                copy = false;
                break;
            }
        }
        
        // copy data
        if (copy) {
            last_index = i - offset;
            start[last_index] = start[i];
        } else {
            copy = true;
        }
    }
    
    // update length
    *sLen -= offset;
    start[*sLen] = '\0';
}

void vt_str_replace(vt_str_t *const s, const char *const sub, const char *const rsub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(rsub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check length and do nothing if empty
    const size_t subLen = strlen(sub);
    const size_t rsubLen = strlen(rsub);
    if (!subLen || !rsubLen) return;

    // find number of instances to be replaced
    const char *ptr = vt_str_z(s);
    while ((ptr = strstr(ptr, sub)) != NULL) {
        const ptrdiff_t at_idx = ptr - vt_str_z(s);

        // replace
        if (subLen == rsubLen) {
            vt_memcopy((char*)ptr, rsub, rsubLen);
        } else {
            vt_str_remove(s, at_idx, subLen);
            if (at_idx < (ptrdiff_t)vt_str_len(s)) vt_str_insert(s, rsub, at_idx);
            else vt_str_append(s, rsub);
        }

        // adjust the pointer:
        // since `s` can be resized during iteration, we cannot use `rsubLen`
        // and must use `vt_str_z(s)` to account for this
        ptr = vt_str_z(s) + at_idx + rsubLen;
    }
}

void vt_str_replace_first(vt_str_t *const s, const char *const sub, const char *const rsub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(rsub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check length and do nothing if empty
    const size_t subLen = strlen(sub);
    const size_t rsubLen = strlen(rsub);
    if (!subLen || !rsubLen) return;

    // find number of instances to be replaced
    const char *const ptr = vt_str_find(s, sub);
    if (ptr) {
        const ptrdiff_t at_idx = ptr - vt_str_z(s);

        // replace
        if (subLen == rsubLen) {
            vt_memcopy((char*)s->ptr + at_idx * s->elsize, rsub, rsubLen);
        } else {
            vt_str_remove(s, at_idx, subLen);
            if (at_idx < (ptrdiff_t)vt_str_len(s)) vt_str_insert(s, rsub, at_idx);
            else vt_str_append(s, rsub);
        }
    }
}

void vt_str_replace_last(vt_str_t *const s, const char *const sub, const char *const rsub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(rsub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_array_is_view(s), "%s: Cannot modify a viewable-only object!\n", vt_status_to_str(VT_STATUS_ERROR_IS_VIEW));

    // check length and do nothing if empty
    const size_t subLen = strlen(sub);
    const size_t rsubLen = strlen(rsub);
    if (!subLen || !rsubLen) return;

    // find the last instance of sep
    const char *ptr = strstr(s->ptr, sub);
    const char *lastInstance = ptr;
    while ((ptr = strstr(ptr + subLen, sub))) {
        lastInstance = ptr;
    }

    // find number of instances to be replaced
    if (lastInstance) {
        const ptrdiff_t at_idx = lastInstance - vt_str_z(s);

        // replace
        if (subLen == rsubLen) {
            vt_memcopy((char*)s->ptr + at_idx * s->elsize, rsub, rsubLen);
        } else {
            vt_str_remove(s, at_idx, subLen);
            if (at_idx < (ptrdiff_t)vt_str_len(s)) vt_str_insert(s, rsub, at_idx);
            else vt_str_append(s, rsub);
        }
    }
}

void vt_str_replace_c(vt_str_t *const s, const char *const c, const char *const r) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(c != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(r != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check length
    const size_t rLen = strlen(r);
    const size_t cLen = strlen(c);
    VT_ENFORCE(rLen <= cLen, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // do nothing if empty
    if (!rLen || !cLen) return;

    // iterate over the string
    char *const ptr = s->ptr;
    const size_t sLen = vt_str_len(s);
    VT_FOREACH(i, 0, sLen) {
        VT_FOREACH(j, 0, cLen) {
            if (ptr[i] == c[j]) {
                ptr[i] = j < rLen ? r[j] : r[rLen - 1];
                break;
            }
        }
    }
}

void vt_str_strip(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // prepare
    size_t offset = 0;
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while (isspace(*curr)) {
        offset++;
        curr = p + offset;
    }

    // update length
    *len -= offset;

    // strip tailing whitespace and control symbols
    curr = p + (*len + offset - 1);
    while (isspace(*curr)) {
        (*len)--;
        curr = p + (*len + offset - 1);
    }

    // move string to the begining
    vt_memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';
}

void vt_str_strip_punct(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // prepare
    size_t offset = 0;
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while (isspace(*curr) || ispunct(*curr)) {
        offset++;
        curr = p + offset;
    }

    // update length
    *len -= offset;

    // strip tailing whitespace and control symbols
    curr = p + (*len + offset - 1);
    while (isspace(*curr) || ispunct(*curr)) {
        (*len)--;
        curr = p + (*len + offset - 1);
    }

    // move string to the begining
    vt_memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';
}

void vt_str_strip_c(vt_str_t *const s, const char *const c) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(c != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // prepare
    bool stop = false;
    size_t offset = 0;
    size_t cLen = strlen(c);
    size_t *len = &s->len;
    const char *p = s->ptr;
    const char *curr = p + offset;

    // strip leading whitespace and control symbols
    while (!stop) {
        for (size_t i = 0; i < cLen; i++) {
            if (*curr == c[i]) {
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
    while (!stop) {
        for (size_t i = 0; i < cLen; i++) {
            if (*curr == c[i]) {
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
    vt_memmove(s->ptr, (char*)(s->ptr) + offset, *len);

    // update end
    ((char*)(s->ptr))[*len] = '\0';
}

const char *vt_str_find(const vt_str_t *const s, const char *sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(strlen(sub) <= vt_str_len(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return strstr(vt_str_z(s), sub);
}

size_t vt_str_can_find(const vt_str_t *const s, const char *sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // substring (needle) cannot be longer than z (haystack)
    const size_t subLen = strlen(sub);
    VT_ENFORCE(subLen <= vt_str_len(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    size_t count = 0;
    const char *p = vt_str_z(s);
    while ((p = strstr(p, sub)) != NULL) {
        count++;
        p += subLen;
    }

    return count;
}

vt_plist_t *vt_str_split(vt_plist_t *ps, const vt_str_t *const s, const char *const sep) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if s contains sep substring
    const size_t strInstances = vt_str_can_find(s, sep);
    if (!strInstances) {
        return NULL;
    }

    // create a vec_t instance
    vt_plist_t *p = NULL;
    if (ps == NULL) {
        p = vt_plist_create(strInstances + 1, NULL);
        if (p == NULL) {
            VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));
            return NULL;
        }
    } else {
        p = ps;
        if (vt_plist_has_space(p) < strInstances + 1) {
            vt_plist_reserve(p, (strInstances + 1) - vt_plist_has_space(p));
        }
    }
    
    // clear
    vt_plist_clear(p);

    // splitting the raw C string
    const size_t sepLen = strlen(sep);
    const char *head = s->ptr;
    const char *current = head;
    while (true) {
        // find sep
        current = strstr(head, sep);

        // count copy length
        const size_t copyLen = strlen(head) - (current == NULL ? 0 : strlen(current));
        if (copyLen == 0) { // if head == current, move head by sepLen (if sep is in the begining)
            head += sepLen;
            continue;
        } else {
            // create a vt_str_t instance and copy the values
            vt_str_t *tempStr = vt_str_create_len(copyLen, p->alloctr ? p->alloctr : NULL);

            // set the value and push it to the list
            vt_str_set_n(tempStr, head, copyLen);
            vt_plist_push_back(p, tempStr);
            
            // update head
            head = current + sepLen;
        }

        // break from loop when no more sep is found or we are at the end of string
        if (current == NULL || current[sepLen] == '\0') {
            break;
        }
    }

    return p;
}

vt_str_t *vt_str_split_between(vt_str_t *const s, const char *const z, const char *const zl, const char *const zr) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(zl != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(zr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // find zl - left substring
    const char *const lsub = strstr(z, zl);
    if (lsub == NULL) {
        return NULL;
    }

    // find r - right substring
    const char *const rsub = strstr(z, zr);
    if (rsub == NULL) {
        return NULL;
    }

    // prepare
    vt_str_t *st = (s == NULL) 
        ? vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL)
        : s;
    vt_str_clear(st);

    // check if lsub < rsub
    if (lsub < rsub) {
        ptrdiff_t sub_len = rsub - lsub - strlen(zl);
        vt_str_reserve(st, sub_len);

        // append sub
        vt_str_append_n(st, rsub - sub_len, sub_len);

        return st;
    }

    return NULL;
}

vt_str_t *vt_str_join_list(vt_str_t *const s, const char *const sep, const vt_plist_t *const p) {
    // check for invalid input
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vt_array_is_valid_object(p), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // create vt_str_t if needed
    vt_str_t *st = (s == NULL)
        ? vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL) 
        : s;

    // append the first part
    vt_str_append(st, vt_plist_get(p, 0));

    // continue appending
    const size_t pLen = vt_plist_len(p);
    for (size_t i = 1; i < pLen; i++) {
        vt_str_append(st, sep);
        vt_str_append(st, vt_plist_get(p, i));
    }

    return st;
}

vt_str_t *vt_str_join_array(vt_str_t *const s, const char *const sep, const char *array[], const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(array != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create vt_str_t if needed
    vt_str_t *st = (s == NULL)
        ? vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL) 
        : s;

    // append the first part
    vt_str_append(st, array[0]);

    // continue appending
    for (size_t i = 1; i < n; i++) {
        vt_str_append(st, sep);
        vt_str_append(st, array[i]);
    }

    return st;
}

vt_str_t *vt_str_pop_get_first(vt_str_t *sr, vt_str_t *const s, const char *const sep) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check sep len is valid
    const size_t sepLen = strlen(sep);
    if (!vt_str_len(s) || !sepLen) {
        return sr;
    }

    // check if s contains sep substring
    const char *const tempStr = strstr(s->ptr, sep);
    if (tempStr == NULL) {
        return sr;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = vt_str_len(s) - strlen(tempStr);
    if (!copyLen) {
        return sr;
    }

    // create vt_str_t instance
    vt_str_t *spop = (sr == NULL) 
        ? vt_str_create_len(copyLen, NULL) 
        : sr;

    // if not enough space, reserve more
    if (vt_str_len(spop) < copyLen) {
        vt_str_reserve(spop, copyLen - vt_str_len(spop));
    }

    // clear
    vt_str_clear(spop);

    // copy the string before the separator
    vt_str_append_n(spop, s->ptr, copyLen);

    // pop the part of the string with the separator
    vt_str_remove(s, 0, copyLen + sepLen);

    return spop;
}

vt_str_t *vt_str_pop_get_last(vt_str_t *sr, vt_str_t *const s, const char *const sep) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sep != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check sep len is valid
    const size_t sepLen = strlen(sep);
    if (!vt_str_len(s) || !sepLen) {
        return sr;
    }

    // check if s contains sep substring
    const char *const tempStr = strstr(s->ptr, sep);
    if (tempStr == NULL) {
        return sr;
    }

    // find the last instance of sep
    const char *p = strstr(s->ptr, sep);
    const char *lastInstance = p;
    while ((p = strstr(p + sepLen, sep)) != NULL) {
        lastInstance = p;
    }

    // if not found, return
    if (lastInstance == NULL) {
        return sr;
    }

    // if the copy length of the substring is zero, there is nothing to copy
    const size_t copyLen = strlen(lastInstance) - sepLen;
    if (!copyLen) {
        return sr;
    }

    // create vt_str_t instance
    vt_str_t *spop = (sr == NULL) 
        ? vt_str_create_len(copyLen, NULL) 
        : sr;

    // if not enough space, reserve more
    if (vt_str_len(spop) < copyLen) {
        vt_str_reserve(spop, copyLen - vt_str_len(spop) + 1);
    }

    // clear
    vt_str_clear(spop);

    // copy the string after the separator
    vt_str_append_n(spop, lastInstance + sepLen, copyLen);

    // pop the part of the string with the separator
    vt_str_remove(s, vt_str_len(s) - copyLen - sepLen, copyLen + sepLen);

    return spop;
}

bool vt_str_equals_z(const char *const z1, const char *const z2) {
    // check for invalid input
    VT_DEBUG_ASSERT(z1 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(z2 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const size_t len = strlen(z1);
    if (len != strlen(z2)) {
        return false;
    }

    return !strncmp(z1, z2, len);
}

bool vt_str_equals_n(const char *const z1, const char *const z2, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(z1 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(z2 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(n <= strlen(z1), "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(n <= strlen(z2), "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return !strncmp(z1, z2, n);
}

bool vt_str_equals(const vt_str_t *const s1, const vt_str_t *const s2) {
    // check for invalid input
    VT_DEBUG_ASSERT(s1 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(s2 != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const size_t len = vt_str_len(s1);
    if (len != vt_str_len(s2)) {
        return false;
    }

    return !strncmp(vt_str_z(s1), vt_str_z(s2), len);
}

bool vt_str_starts_with(const vt_str_t *const s, const char *const sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const size_t subLen = strlen(sub);
    if (subLen > vt_str_len(s)) {
        return false;
    }

    return !strncmp(vt_str_z(s), sub, subLen);
}

bool vt_str_ends_with(const vt_str_t *const s, const char *const sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const size_t sLen = vt_str_len(s);
    const size_t subLen = strlen(sub);
    if (subLen > sLen) {
        return false;
    }

    return !strncmp(vt_str_z(s) + sLen - subLen, sub, subLen);
}

void vt_str_apply(const vt_str_t *const s, void (*func)(char*, size_t)) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(func != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const size_t len = vt_str_len(s);
    for (size_t i = 0; i < len; i++) {
        func(&((char*)s->ptr)[i], i);
    }
}

bool vt_str_is_numeric(const vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // check if string is a number
    const char *const z = vt_str_z(s);
    const size_t len = vt_str_len(s);
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(z[i]) && z[i] != '.') {
            return false;
        }
    }

    return true;
}

bool vt_str_is_numeric_z(const char *const z, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // check if string is a number
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(z[i]) && z[i] != '.') {
            return false;
        }
    }

    return true;
}

void vt_str_to_uppercase(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // to uppercase
    char *const z = s->ptr;
    const size_t zLen = vt_str_len(s);
    for (size_t i = 0; i < zLen; i++) {
        z[i] = toupper(z[i]);
    }
}

void vt_str_to_lowercase(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // to lowercase
    char *const z = s->ptr;
    const size_t zLen = vt_str_len(s);
    for (size_t i = 0; i < zLen; i++) {
        z[i] = tolower(z[i]);
    }
}

int64_t vt_str_index_of(const vt_str_t *const s, const char z) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    const char *const ztmp = strchr(vt_str_z(s), z);
    if (ztmp == NULL) {
        return -1;
    }

    return (int64_t)(ztmp - vt_str_z(s));
}

int64_t vt_str_index_find(const vt_str_t *const s, const char *sub) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(sub != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // find substring
    const char *sub_start = vt_str_find(s, sub);
    if (sub_start == NULL) {
        return -1;
    }

    return (int64_t)(sub_start - vt_str_z(s));
}

char *vt_str_slide_front(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    return (char*)vt_array_slide_front(s);
}

char *vt_str_slide_back(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    return (char*)vt_array_slide_back(s);
}

void vt_str_slide_reset(vt_str_t *const s) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    vt_array_slide_reset(s);
}

// -------------------------- PRIVATE -------------------------- //

/** Sets a formatted string into a vt_str_t
    @param s vt_str_t instance
    @param fmt string print format
    @param args variable args list

    @returns `vt_str_t` instance upon success, NULL upon failure
*/
static vt_str_t *vt_str_vfmt_set(vt_str_t *s, const char *const fmt, va_list args) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // format string
    va_list args2; va_copy(args2, args); 
    {
        // check format length
        const int64_t len = vsnprintf(NULL, (size_t)0, fmt, args) + 1;
        if (len < 0) {
            VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            va_end(args2);
            return NULL;
        }

        // check for space
        const size_t hasSpace = vt_str_has_space(s);
        if (hasSpace < (size_t)len) {
            vt_str_reserve(s, (len - hasSpace));
        }

        // print data to s
        // vsprintf((char*)s->ptr + s->len * s->elsize, fmt, args2); // TODO: remove
        vsnprintf(s->ptr, len+1, fmt, args2);

        // update
        s->len = len;
        ((char*)s->ptr)[s->len] = '\0';
    }
    va_end(args2);

    return s;
}

/** Appends a formatted string into a vt_str_t
    @param s vt_str_t instance
    @param fmt string print format
    @param args variable args list

    @returns `vt_str_t` instance upon success, NULL upon failure
*/
static vt_str_t *vt_str_vfmt_append(vt_str_t *s, const char *const fmt, va_list args) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(s), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // format string
    va_list args2; va_copy(args2, args); 
    {
        // check format length
        const int64_t len = vsnprintf(NULL, (size_t)0, fmt, args);
        if (len < 0) {
            VT_DEBUG_PRINTF("%s\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            va_end(args2);
            return NULL;
        }

        // check for space
        const size_t hasSpace = vt_str_has_space(s);
        if (hasSpace < (size_t)len) {
            vt_str_reserve(s, (len - hasSpace));
        }

        // print data to s
        // vsprintf((char*)s->ptr + s->len * s->elsize, fmt, args2); // TODO: remove
        vsnprintf((char*)s->ptr + s->len * s->elsize, len+1, fmt, args2);

        // update
        s->len += len;
        ((char*)s->ptr)[s->len] = '\0';
    } 
    va_end(args2);

    return s;
}

