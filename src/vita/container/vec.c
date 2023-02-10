#include "vita/container/vec.h"

vt_vec_t *vt_vec_new(void) {
    vt_vec_t *v = VT_DEBUG_CALLOC(sizeof(vt_vec_t));
    if(v == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    return v;
}

enum VitaError vt_vec_ctor(vt_vec_t *const v, const size_t n, const size_t elsize) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // vt_vec_t init
    *v = (vt_vec_t) {
        .ptr = VT_DEBUG_CALLOC(n * elsize),
        .len = 0,
        .capacity = n,
        .elsize = elsize,
    };

    // checking if v->ptr was allocated
    if(v->ptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    return vt_ve_operation_success;
}

vt_vec_t *vt_vec_dup(const vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // allocate a new vt_vec_t instance
    vt_vec_t *vdup = vt_vec_new();
    if(vdup == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // construct vdup and resize it
    if(vt_vec_ctor(vdup, v->capacity, v->elsize) != vt_ve_operation_success || vt_vec_resize(vdup, v->len) != vt_ve_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));        
        vt_vec_free(vdup);
        return NULL;
    }

    // copy values
    memcpy(vdup->ptr, v->ptr, v->len * v->elsize);

    return vdup;
}

void vt_vec_dtor(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // free vt_vec_t contents
    DEBUG_FREE(v->ptr);

    // default-init
    *v = (vt_vec_t) {0};
}

void vt_vec_free(vt_vec_t *v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // free the vt_vec_t
    DEBUG_FREE(v);

    // reset to NULL
    v = NULL;
}

vt_vec_t *vt_vec_create(const size_t n, const size_t elsize) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate vt_vec_t
    vt_vec_t *v = vt_vec_new();
    if(v == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // construct the instance
    if(vt_vec_ctor(v, n, elsize) != vt_ve_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_operation_failure));
        vt_vec_free(v);
        return NULL;
    }

    return v;
}

void vt_vec_destroy(vt_vec_t *v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    vt_vec_dtor(v);
    vt_vec_free(v);
}

vt_vec_t *vt_vec_from(const void *const ptr, const size_t n, const size_t elsize) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    vt_vec_t *v = vt_vec_create(n, elsize);
    if(v == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // if nothing to copy return an empty vt_vec_t instance
    if(ptr == NULL) {
        return v;
    }

    // copy values
    memmove(v->ptr, ptr, n * elsize);
    v->len = n;

    return v;
}

vt_vec_t *vt_vec_fromi8(const int8_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(int8_t));
}

vt_vec_t *vt_vec_fromu8(const uint8_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(uint8_t));
}

vt_vec_t *vt_vec_fromi16(const int16_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(int16_t));
}

vt_vec_t *vt_vec_fromu16(const uint16_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(uint16_t));
}

vt_vec_t *vt_vec_fromi32(const int32_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(int32_t));
}

vt_vec_t *vt_vec_fromu32(const uint32_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(uint32_t));
}

vt_vec_t *vt_vec_fromi64(const int64_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(int64_t));
}

vt_vec_t *vt_vec_fromu64(const uint64_t *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(uint64_t));
}

vt_vec_t *vt_vec_fromf(const float *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(float));
}

vt_vec_t *vt_vec_fromd(const double *const ptr, const size_t n) {
    return vt_vec_from(ptr, n, sizeof(double));
}

size_t vt_vec_len(const vt_vec_t *const v) {
    return vt_bct_len(v);
}

size_t vt_vec_capacity(const vt_vec_t *const v) {
    return vt_bct_capacity(v);
}

size_t vt_vec_has_space(const vt_vec_t *const v) {
    return vt_bct_capacity(v) - vt_bct_len(v);
}

bool vt_vec_is_empty(const vt_vec_t *const v) {
    return !vt_bct_len(v);
}

enum VitaError vt_vec_shrink(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // if length and capacity are the same, exit the function
    if(v->len == v->capacity) {
        return vt_ve_operation_success;
    }

    // shrink the array capacity to length
    void *newptr = VT_DEBUG_REALLOC(v->ptr, v->len * v->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // update values
    v->ptr = newptr;
    v->capacity = v->len;

    return vt_ve_operation_success;
}

enum VitaError vt_vec_clear(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // update length
    v->len = 0;

    return vt_ve_operation_success;
}

enum VitaError vt_vec_reserve(vt_vec_t *const v, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // if n == 0, do nothing
    if(!n) {
        return vt_ve_operation_success;
    }

    // reserve memory for additional n elements
    void *newptr = VT_DEBUG_REALLOC(v->ptr, (v->capacity + n) * v->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // update values
    v->ptr = newptr;
    v->capacity += n;

    return vt_ve_operation_success;
}

enum VitaError vt_vec_resize(vt_vec_t *const v, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // cannot resize to 0, it's not an error, thus, do nothing
    if(n == 0) {
        return ve_operation_failure;
    }

    if(n == v->capacity) {
        v->len = v->capacity;
        return vt_ve_operation_success;
    }

    // resize vt_vec_t
    void *newptr = VT_DEBUG_REALLOC(v->ptr, n * v->elsize);
    if(newptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // update values
    v->ptr = newptr;
    v->len = v->capacity = n;

    return vt_ve_operation_success;
}

enum VitaError vt_vec_push(vt_vec_t *const v, const void *const val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if new memory needs to be allocated
    if(!vt_vec_has_space(v) && vt_vec_reserve(v, v->capacity * VT_CONTAINER_GROWTH_RATE) != vt_ve_operation_success) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // copy val to vt_vec_t
    memcpy(((char*)(v->ptr) + v->len++ * v->elsize), val, v->elsize);

    return vt_ve_operation_success;
}

enum VitaError vt_vec_pushi8(vt_vec_t *const v, const int8_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushu8(vt_vec_t *const v, const uint8_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushi16(vt_vec_t *const v, const int16_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushu16(vt_vec_t *const v, const uint16_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushi32(vt_vec_t *const v, const int32_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushu32(vt_vec_t *const v, const uint32_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushi64(vt_vec_t *const v, const int64_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushu64(vt_vec_t *const v, const uint64_t val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushf(vt_vec_t *const v, const float val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pushd(vt_vec_t *const v, const double val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));

    return vt_vec_push(v, &val);
}

enum VitaError vt_vec_pop(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // pop the last element
    if(v->len > 0) {
        v->len--;
    }

    return vt_ve_operation_success;
}

void *vt_vec_pop_get(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // pop the last element
    if(v->len > 0) {
        return ((char*)(v->ptr) + --v->len * v->elsize);
    }

    return v->ptr;
}

enum VitaError vt_vec_set(vt_vec_t *const v, const void *const val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    // copy val data to str_t
    memcpy(((char*)(v->ptr) + at * v->elsize), val, v->elsize);

    return vt_ve_operation_success;
}

enum VitaError vt_vec_seti8(vt_vec_t *const v, const int8_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_setu8(vt_vec_t *const v, const uint8_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_seti16(vt_vec_t *const v, const int16_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_setu16(vt_vec_t *const v, const uint16_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_seti32(vt_vec_t *const v, const int32_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_setu32(vt_vec_t *const v, const uint32_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_seti64(vt_vec_t *const v, const int64_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_setu64(vt_vec_t *const v, const uint64_t val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_setf(vt_vec_t *const v, const float val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

enum VitaError vt_vec_setd(vt_vec_t *const v, const double val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(v->elsize == sizeof(val), "%s\n", vt_get_vita_error_str(vt_ve_error_incompatible_datatype));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return vt_vec_set(v, &val, at);
}

void *vt_vec_get(const vt_vec_t *const v, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    return ((char*)(v->ptr) + at * v->elsize);
}

int8_t vt_vec_geti8(const vt_vec_t *const v, const size_t at) {
    return *(int8_t*)(vt_vec_get(v, at));
}

uint8_t vt_vec_getu8(const vt_vec_t *const v, const size_t at) {
    return *(uint8_t*)(vt_vec_get(v, at));
}

int16_t vt_vec_geti16(const vt_vec_t *const v, const size_t at) {
    return *(int16_t*)(vt_vec_get(v, at));
}

uint16_t vt_vec_getu16(const vt_vec_t *const v, const size_t at) {
    return *(uint16_t*)(vt_vec_get(v, at));
}

int32_t vt_vec_geti32(const vt_vec_t *const v, const size_t at) {
    return *(int32_t*)(vt_vec_get(v, at));
}

uint32_t vt_vec_getu32(const vt_vec_t *const v, const size_t at) {
    return *(uint32_t*)(vt_vec_get(v, at));
}

int64_t vt_vec_geti64(const vt_vec_t *const v, const size_t at) {
    return *(int64_t*)(vt_vec_get(v, at));
}

uint64_t vt_vec_getu64(const vt_vec_t *const v, const size_t at) {
    return *(uint64_t*)(vt_vec_get(v, at));
}

float vt_vec_getf(const vt_vec_t *const v, const size_t at) {
    return *(float*)(vt_vec_get(v, at));
}

double vt_vec_getd(const vt_vec_t *const v, const size_t at) {
    return *(double*)(vt_vec_get(v, at));
}

enum VitaError vt_vec_insert(vt_vec_t *const v, const void *const val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    // check if new memory needs to be allocated
    if(!vt_vec_has_space(v) && !vt_vec_reserve(v, v->capacity * VT_CONTAINER_GROWTH_RATE)) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return vt_ve_error_allocation;
    }

    // shift values by one value to the end of the vt_vec_t
    memmove(((char*)(v->ptr) + (at + 1) * v->elsize), ((char*)(v->ptr) + at * v->elsize), ((v->len - at) * v->elsize));

    // copy the str contents to str from the specified index
    memcpy(((char*)(v->ptr) + at * v->elsize), val, v->elsize);

    // set new length
    v->len++;

    return vt_ve_operation_success;
}

enum VitaError vt_vec_remove(vt_vec_t *const v, const size_t at, const enum RemoveStrategy rs) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_get_vita_error_str(vt_ve_error_out_of_bounds_access), 
        at, 
        v->len
    );

    // check remove strategy
    if(rs == rs_stable) {
        memmove((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (at + 1) * v->elsize, (v->len - at) * v->elsize);
    } else {
        gswap((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (v->len - 1) * v->elsize, v->elsize);
    }

    // set new length
    v->len--;

    return vt_ve_operation_success;
}

int64_t vt_vec_contains(const vt_vec_t *const v, const void *const val) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    size_t i = 0;
    for (char *iter = v->ptr; iter != (char*)(v->ptr) + v->len * v->elsize; iter += v->elsize, i++) {
        if(memcmp(iter, val, v->elsize) == 0) {
            return i;
        }
    }

    return -1;
}

void vt_vec_apply(const vt_vec_t *const v, void (*func)(void*, size_t)) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(func != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    size_t i = 0;
    for(char *iter = v->ptr; iter != (char*)(v->ptr) + v->len * v->elsize; iter += v->elsize, i++) {
        func(iter, i);
    }
}
