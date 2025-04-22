#include "vita/container/vec.h"

vt_vec_t *vt_vec_create(const size_t n, const size_t elsize, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // allocate a new vt_vec_t instance
    vt_vec_t *v = vt_array_new(alloctr);
    *v = (vt_vec_t) {
        .alloctr = alloctr,
        .ptr = alloctr ? VT_ALLOCATOR_ALLOC(alloctr, n * elsize) : VT_CALLOC(n * elsize),
        .len = 0,
        .capacity = n,
        .elsize = elsize,
    };

    return v;
}

vt_vec_t *vt_vec_create_from(const size_t n, const size_t elsize, const void *vals, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(vals != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // create vector instance
    vt_vec_t *v = vt_vec_create(n, elsize, alloctr);
    v->len = n;

    // copy values to vector
    vt_memmove(v->ptr, vals, n * elsize);

    return v;
}

#define VT_PROTOTYPE_VEC_CREATE_FROM(T, t)                                                                          \
    vt_vec_t *vt_vec_create_from_##t(const size_t n, const T vals[], struct VitaBaseAllocatorType *const alloctr) { \
        return vt_vec_create_from(n, sizeof(T), vals, alloctr);                                                     \
    }
VT_PROTOTYPE_VEC_CREATE_FROM(int8_t, i8)
VT_PROTOTYPE_VEC_CREATE_FROM(uint8_t, u8)
VT_PROTOTYPE_VEC_CREATE_FROM(int16_t, i16)
VT_PROTOTYPE_VEC_CREATE_FROM(uint16_t, u16)
VT_PROTOTYPE_VEC_CREATE_FROM(int32_t, i32)
VT_PROTOTYPE_VEC_CREATE_FROM(uint32_t, u32)
VT_PROTOTYPE_VEC_CREATE_FROM(int64_t, i64)
VT_PROTOTYPE_VEC_CREATE_FROM(uint64_t, u64)
VT_PROTOTYPE_VEC_CREATE_FROM(float, f)
VT_PROTOTYPE_VEC_CREATE_FROM(double, d)
VT_PROTOTYPE_VEC_CREATE_FROM(real, r)
#undef VT_PROTOTYPE_VEC_CREATE_FROM

void vt_vec_destroy(vt_vec_t *v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // free vt_vec_t contents
    if (v->alloctr) {
        VT_ALLOCATOR_FREE(v->alloctr, v->ptr);
    } else {
        VT_FREE(v->ptr);
    }
    v->ptr = NULL; // reset to zero


    // free vt_vec_t instance itself
    vt_array_free(v);
    v = NULL;
}

vt_vec_t *vt_vec_dup(const vt_vec_t *const v, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // allocate a new vt_vec_t instance
    vt_vec_t *vdup = vt_vec_create(v->len, v->elsize, alloctr);
    vdup->len = v->len;

    // copy values
    memcpy(vdup->ptr, v->ptr, v->len * v->elsize);

    return vdup;
}

size_t vt_vec_len(const vt_vec_t *const v) {
    return vt_array_len(v);
}

size_t vt_vec_capacity(const vt_vec_t *const v) {
    return vt_array_capacity(v);
}

size_t vt_vec_has_space(const vt_vec_t *const v) {
    return vt_array_has_space(v);
}

bool vt_vec_is_empty(const vt_vec_t *const v) {
    return !vt_array_len(v);
}

void vt_vec_shrink(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // if length and capacity are the same, exit the function
    if (v->len == v->capacity) {
        return;
    }

    // shrink the array capacity to length
    v->ptr = v->alloctr 
        ? VT_ALLOCATOR_REALLOC(v->alloctr, v->ptr, v->len * v->elsize) 
        : VT_REALLOC(v->ptr, v->len * v->elsize);

    // update
    v->capacity = v->len;
}

void vt_vec_clear(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // update length
    v->len = 0;
}

void vt_vec_reserve(vt_vec_t *const v, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // reserve memory for additional n elements
    v->ptr = v->alloctr 
        ? VT_ALLOCATOR_REALLOC(v->alloctr, v->ptr, (v->capacity + n) * v->elsize) 
        : VT_REALLOC(v->ptr, (v->capacity + n) * v->elsize);

    // update
    v->capacity += n;
}

void vt_vec_resize(vt_vec_t *const v, const size_t n) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(n > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    if (n == v->capacity) {
        v->len = v->capacity;
        return;
    }

    // resize vt_vec_t
    v->ptr = v->alloctr 
        ? VT_ALLOCATOR_REALLOC(v->alloctr, v->ptr, n * v->elsize) 
        : VT_REALLOC(v->ptr, n * v->elsize);

    // update
    v->len = v->capacity = n;
}

void vt_vec_push_front(vt_vec_t *const v, const void *const val) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // push value
    if (vt_vec_len(v)) vt_vec_insert(v, val, 0);
    else vt_vec_push_back(v, val);
}

#define VT_INSTANTIATE_VEC_PUSH_FRONT(T, t)                     \
    void vt_vec_push_front_##t(vt_vec_t *const v, const T val) {\
        vt_vec_push_front(v, &val);                             \
    }
VT_INSTANTIATE_VEC_PUSH_FRONT(int8_t, i8)
VT_INSTANTIATE_VEC_PUSH_FRONT(uint8_t, u8)
VT_INSTANTIATE_VEC_PUSH_FRONT(int16_t, i16)
VT_INSTANTIATE_VEC_PUSH_FRONT(uint16_t, u16)
VT_INSTANTIATE_VEC_PUSH_FRONT(int32_t, i32)
VT_INSTANTIATE_VEC_PUSH_FRONT(uint32_t, u32)
VT_INSTANTIATE_VEC_PUSH_FRONT(int64_t, i64)
VT_INSTANTIATE_VEC_PUSH_FRONT(uint64_t, u64)
VT_INSTANTIATE_VEC_PUSH_FRONT(float, f)
VT_INSTANTIATE_VEC_PUSH_FRONT(double, d)
VT_INSTANTIATE_VEC_PUSH_FRONT(real, r)
#undef VT_INSTANTIATE_VEC_PUSH_FRONT

void vt_vec_push_back(vt_vec_t *const v, const void *const val) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // check if new memory needs to be allocated
    if (!vt_vec_has_space(v)) {
        vt_vec_reserve(v, v->capacity * VT_ARRAY_DEFAULT_GROWTH_RATE);
    }

    // copy val to vt_vec_t
    memcpy(((char*)(v->ptr) + v->len++ * v->elsize), val, v->elsize);
}

#define VT_INSTANTIATE_VEC_PUSH_BACK(T, t)                     \
    void vt_vec_push_back_##t(vt_vec_t *const v, const T val) {\
        vt_vec_push_back(v, &val);                             \
    }
VT_INSTANTIATE_VEC_PUSH_BACK(int8_t, i8)
VT_INSTANTIATE_VEC_PUSH_BACK(uint8_t, u8)
VT_INSTANTIATE_VEC_PUSH_BACK(int16_t, i16)
VT_INSTANTIATE_VEC_PUSH_BACK(uint16_t, u16)
VT_INSTANTIATE_VEC_PUSH_BACK(int32_t, i32)
VT_INSTANTIATE_VEC_PUSH_BACK(uint32_t, u32)
VT_INSTANTIATE_VEC_PUSH_BACK(int64_t, i64)
VT_INSTANTIATE_VEC_PUSH_BACK(uint64_t, u64)
VT_INSTANTIATE_VEC_PUSH_BACK(float, f)
VT_INSTANTIATE_VEC_PUSH_BACK(double, d)
VT_INSTANTIATE_VEC_PUSH_BACK(real, r)
#undef VT_INSTANTIATE_VEC_PUSH_BACK

void vt_vec_pop(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // pop the last element
    if (v->len > 0) {
        v->len--;
    }
}

void *vt_vec_pop_get(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    // pop the last element
    if (v->len > 0) {
        return ((char*)(v->ptr) + --v->len * v->elsize);
    }

    return NULL;
}

#define VT_INSTANTIATE_VEC_POP_GET(T, t)        \
    T vt_vec_pop_get_##t(vt_vec_t *const v) {   \
        void *ptr = vt_vec_pop_get(v);          \
        return ptr ? *(T*)ptr : 0;              \
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

void vt_vec_set(vt_vec_t *const v, const void *const val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        v->len
    );

    // copy val data to vt_str_t
    vt_array_set(v, val, at);
}

#define VT_INSTANTIATE_VEC_SET(T, t)                                      \
    void vt_vec_set_##t(vt_vec_t *const v, const T val, const size_t at) {\
        vt_vec_set(v, &val, at);                                          \
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
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        v->len
    );

    return vt_array_get(v, at);
}

#define VT_INSTANTIATE_VEC_GET(T, t)                            \
    T vt_vec_get_##t(const vt_vec_t *const v, const size_t at) {\
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

void vt_vec_insert(vt_vec_t *const v, const void *const val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        v->len
    );

    // check if new memory needs to be allocated
    if (!vt_vec_has_space(v)) {
        vt_vec_reserve(v, v->capacity * VT_ARRAY_DEFAULT_GROWTH_RATE);
    }

    // shift values by one value to the end of the vt_vec_t
    vt_memmove(((char*)(v->ptr) + (at + 1) * v->elsize), ((char*)(v->ptr) + at * v->elsize), ((v->len - at) * v->elsize));

    // copy the str contents to str from the specified index
    memcpy(((char*)(v->ptr) + at * v->elsize), val, v->elsize);

    // set new length
    v->len++;
}

#define VT_INSTANTIATE_VEC_INSERT(T, t)                                       \
    void vt_vec_insert_##t(vt_vec_t *const v, const T val, const size_t at) { \
        vt_vec_insert(v, &val, at);                                           \
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

void vt_vec_remove(vt_vec_t *const v, const size_t at, const enum VitaRemoveStrategy rs) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(rs < VT_REMOVE_STRATEGY_COUNT, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < v->len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        v->len
    );

    // check remove strategy
    if (rs == VT_REMOVE_STRATEGY_STABLE) {
        vt_memmove((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (at + 1) * v->elsize, (v->len - at - 1) * v->elsize);
    } else {
        vt_gswap((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (v->len - 1) * v->elsize, v->elsize);
    }

    // set new length
    v->len--;
}

int64_t vt_vec_can_find(const vt_vec_t *const v, const void *const val) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    size_t i = 0;
    for (char *iter = v->ptr; iter != (char*)(v->ptr) + v->len * v->elsize; iter += v->elsize, i++) {
        if (memcmp(iter, val, v->elsize) == 0) {
            return i;
        }
    }

    return -1;
}

#define VT_INSTANTIATE_VEC_CAN_FIND(T, t)                               \
    int64_t vt_vec_can_find_##t(const vt_vec_t *const v, const T val) { \
        return vt_vec_can_find(v, &val);                                \
    }
VT_INSTANTIATE_VEC_CAN_FIND(int8_t, i8)
VT_INSTANTIATE_VEC_CAN_FIND(uint8_t, u8)
VT_INSTANTIATE_VEC_CAN_FIND(int16_t, i16)
VT_INSTANTIATE_VEC_CAN_FIND(uint16_t, u16)
VT_INSTANTIATE_VEC_CAN_FIND(int32_t, i32)
VT_INSTANTIATE_VEC_CAN_FIND(uint32_t, u32)
VT_INSTANTIATE_VEC_CAN_FIND(int64_t, i64)
VT_INSTANTIATE_VEC_CAN_FIND(uint64_t, u64)
VT_INSTANTIATE_VEC_CAN_FIND(float, f)
VT_INSTANTIATE_VEC_CAN_FIND(double, d)
VT_INSTANTIATE_VEC_CAN_FIND(real, r)
#undef VT_INSTANTIATE_VEC_CAN_FIND

void *vt_vec_slide_front(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    return vt_array_slide_front(v);
}

void *vt_vec_slide_back(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));

    return vt_array_slide_back(v);
}

void vt_vec_slide_reset(vt_vec_t *const v) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    
    vt_array_slide_reset(v);
}


void vt_vec_apply(const vt_vec_t *const v, void (*func)(void*, size_t)) {
    // check for invalid input
    VT_DEBUG_ASSERT(vt_array_is_valid_object(v), "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_INVALID_OBJECT));
    VT_DEBUG_ASSERT(func != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    size_t i = 0;
    for (char *iter = v->ptr; iter != (char*)(v->ptr) + v->len * v->elsize; iter += v->elsize, i++) {
        func(iter, i);
    }
}
