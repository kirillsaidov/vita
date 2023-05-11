#include "vita/container/vec.h"

vt_vec_t *vt_vec_new(void) {
    return vt_bct_new();
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
    VT_DEBUG_FREE(v->ptr);

    // reset to zero
    memset(v, 0, sizeof(vt_vec_t));
}

void vt_vec_free(vt_vec_t *v) {
    vt_bct_free(v);
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

#define VT_INSTANTIATE_VEC_FROM(T, t)                               \
    vt_vec_t *vt_vec_from##t(const T *const ptr, const size_t n) {  \
        return vt_vec_from(ptr, n, sizeof(T));                      \
    }
VT_INSTANTIATE_VEC_FROM(int8_t, i8)
VT_INSTANTIATE_VEC_FROM(uint8_t, u8)
VT_INSTANTIATE_VEC_FROM(int16_t, i16)
VT_INSTANTIATE_VEC_FROM(uint16_t, u16)
VT_INSTANTIATE_VEC_FROM(int32_t, i32)
VT_INSTANTIATE_VEC_FROM(uint32_t, u32)
VT_INSTANTIATE_VEC_FROM(int64_t, i64)
VT_INSTANTIATE_VEC_FROM(uint64_t, u64)
VT_INSTANTIATE_VEC_FROM(float, f)
VT_INSTANTIATE_VEC_FROM(double, d)
VT_INSTANTIATE_VEC_FROM(real, r)
#undef VT_INSTANTIATE_VEC_FROM

size_t vt_vec_len(const vt_vec_t *const v) {
    return vt_bct_len(v);
}

size_t vt_vec_capacity(const vt_vec_t *const v) {
    return vt_bct_capacity(v);
}

size_t vt_vec_has_space(const vt_vec_t *const v) {
    return vt_bct_has_space(v);
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
        return vt_ve_operation_failure;
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

#define VT_INSTANTIATE_VEC_PUSH(T, t)                               \
    enum VitaError vt_vec_push##t(vt_vec_t *const v, const T val) { \
        return vt_vec_push(v, &val);                                \
    }
VT_INSTANTIATE_VEC_PUSH(int8_t, i8)
VT_INSTANTIATE_VEC_PUSH(uint8_t, u8)
VT_INSTANTIATE_VEC_PUSH(int16_t, i16)
VT_INSTANTIATE_VEC_PUSH(uint16_t, u16)
VT_INSTANTIATE_VEC_PUSH(int32_t, i32)
VT_INSTANTIATE_VEC_PUSH(uint32_t, u32)
VT_INSTANTIATE_VEC_PUSH(int64_t, i64)
VT_INSTANTIATE_VEC_PUSH(uint64_t, u64)
VT_INSTANTIATE_VEC_PUSH(float, f)
VT_INSTANTIATE_VEC_PUSH(double, d)
VT_INSTANTIATE_VEC_PUSH(real, r)
#undef VT_INSTANTIATE_VEC_PUSH

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

#define VT_INSTANTIATE_VEC_POP_GET(T, t)        \
    T vt_vec_pop_get##t(vt_vec_t *const v) {    \
        return *(T*)vt_vec_pop_get(v);          \
    }
VT_INSTANTIATE_VEC_POP_GET(int8_t, i8)
VT_INSTANTIATE_VEC_POP_GET(uint8_t, u8)
VT_INSTANTIATE_VEC_POP_GET(int16_t, i16)
VT_INSTANTIATE_VEC_POP_GET(uint16_t, u16)
VT_INSTANTIATE_VEC_POP_GET(int32_t, i32)
VT_INSTANTIATE_VEC_POP_GET(uint32_t, u32)
VT_INSTANTIATE_VEC_POP_GET(int64_t, i64)
VT_INSTANTIATE_VEC_POP_GET(uint64_t, u64)
VT_INSTANTIATE_VEC_POP_GET(float, f)
VT_INSTANTIATE_VEC_POP_GET(double, d)
VT_INSTANTIATE_VEC_POP_GET(real, r)
#undef VT_INSTANTIATE_VEC_POP_GET

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

    // copy val data to vt_str_t
    memcpy(((char*)(v->ptr) + at * v->elsize), val, v->elsize);

    return vt_ve_operation_success;
}

#define VT_INSTANTIATE_VEC_SET(T, t)                                                \
    enum VitaError vt_vec_set##t(vt_vec_t *const v, const T val, const size_t at) { \
        return vt_vec_set(v, &val, at);                                             \
    }
VT_INSTANTIATE_VEC_SET(int8_t, i8)
VT_INSTANTIATE_VEC_SET(uint8_t, u8)
VT_INSTANTIATE_VEC_SET(int16_t, i16)
VT_INSTANTIATE_VEC_SET(uint16_t, u16)
VT_INSTANTIATE_VEC_SET(int32_t, i32)
VT_INSTANTIATE_VEC_SET(uint32_t, u32)
VT_INSTANTIATE_VEC_SET(int64_t, i64)
VT_INSTANTIATE_VEC_SET(uint64_t, u64)
VT_INSTANTIATE_VEC_SET(float, f)
VT_INSTANTIATE_VEC_SET(double, d)
VT_INSTANTIATE_VEC_SET(real, r)
#undef VT_INSTANTIATE_VEC_SET

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

#define VT_INSTANTIATE_VEC_GET(T, t)                            \
    T vt_vec_get##t(const vt_vec_t *const v, const size_t at) { \
        return *(T*)(vt_vec_get(v, at));                        \
    }
VT_INSTANTIATE_VEC_GET(int8_t, i8)
VT_INSTANTIATE_VEC_GET(uint8_t, u8)
VT_INSTANTIATE_VEC_GET(int16_t, i16)
VT_INSTANTIATE_VEC_GET(uint16_t, u16)
VT_INSTANTIATE_VEC_GET(int32_t, i32)
VT_INSTANTIATE_VEC_GET(uint32_t, u32)
VT_INSTANTIATE_VEC_GET(int64_t, i64)
VT_INSTANTIATE_VEC_GET(uint64_t, u64)
VT_INSTANTIATE_VEC_GET(float, f)
VT_INSTANTIATE_VEC_GET(double, d)
VT_INSTANTIATE_VEC_GET(real, r)
#undef VT_INSTANTIATE_VEC_GET

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

#define VT_INSTANTIATE_VEC_INSERT(T, t)                                                 \
    enum VitaError vt_vec_insert##t(vt_vec_t *const v, const T val, const size_t at) {  \
        return vt_vec_insert(v, &val, at);                                              \
    }
VT_INSTANTIATE_VEC_INSERT(int8_t, i8)
VT_INSTANTIATE_VEC_INSERT(uint8_t, u8)
VT_INSTANTIATE_VEC_INSERT(int16_t, i16)
VT_INSTANTIATE_VEC_INSERT(uint16_t, u16)
VT_INSTANTIATE_VEC_INSERT(int32_t, i32)
VT_INSTANTIATE_VEC_INSERT(uint32_t, u32)
VT_INSTANTIATE_VEC_INSERT(int64_t, i64)
VT_INSTANTIATE_VEC_INSERT(uint64_t, u64)
VT_INSTANTIATE_VEC_INSERT(float, f)
VT_INSTANTIATE_VEC_INSERT(double, d)
VT_INSTANTIATE_VEC_INSERT(real, r)
#undef VT_INSTANTIATE_VEC_INSERT

enum VitaError vt_vec_remove(vt_vec_t *const v, const size_t at, const enum VitaRemoveStrategy rs) {
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
    if(rs == vt_rs_stable) {
        memmove((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (at + 1) * v->elsize, (v->len - at) * v->elsize);
    } else {
        vt_gswap((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (v->len - 1) * v->elsize, v->elsize);
    }

    // set new length
    v->len--;

    return vt_ve_operation_success;
}

int64_t vt_vec_can_find(const vt_vec_t *const v, const void *const val) {
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

#define VT_INSTANTIATE_VEC_CONTAINS(T, t)                               \
    int64_t vt_vec_can_find##t(const vt_vec_t *const v, const T val) {  \
        return vt_vec_can_find(v, &val);                                \
    }
VT_INSTANTIATE_VEC_CONTAINS(int8_t, i8)
VT_INSTANTIATE_VEC_CONTAINS(uint8_t, u8)
VT_INSTANTIATE_VEC_CONTAINS(int16_t, i16)
VT_INSTANTIATE_VEC_CONTAINS(uint16_t, u16)
VT_INSTANTIATE_VEC_CONTAINS(int32_t, i32)
VT_INSTANTIATE_VEC_CONTAINS(uint32_t, u32)
VT_INSTANTIATE_VEC_CONTAINS(int64_t, i64)
VT_INSTANTIATE_VEC_CONTAINS(uint64_t, u64)
VT_INSTANTIATE_VEC_CONTAINS(float, f)
VT_INSTANTIATE_VEC_CONTAINS(double, d)
VT_INSTANTIATE_VEC_CONTAINS(real, r)
#undef VT_INSTANTIATE_VEC_CONTAINS

void *vt_vec_slide_front(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return vt_bct_slide_front(v);
}

void *vt_vec_slide_back(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return vt_bct_slide_back(v);
}

void vt_vec_slide_reset(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(v != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(v->ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    
    vt_bct_slide_reset(v);
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
