#include "vita/container/vec.h"

vec_t *vec_new(void) {
    vec_t *v = DEBUG_CALLOC(sizeof(vec_t));
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "Failed to allocate vec_t instance!");
        return NULL;
    }

    return v;
}

enum VitaError vec_ctor(vec_t *const v, const size_t n, const size_t elsize) {
    // check if v was allocated
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not allocated!");
        return ve_error_is_null;
    }

    // vec_t init
    *v = (vec_t) {
        .ptr = DEBUG_CALLOC(n * elsize),
        .len = 0,
        .capacity = n,
        .elsize = elsize,
    };

    // checking if v->ptr was allocated
    if(v->ptr == NULL) {
        DEBUG_ASSERT(v->ptr != NULL, "Unable to construct vec_t instance!");
        return ve_error_allocation;
    }

    return ve_operation_success;
}

vec_t *vec_dup(const vec_t *const v) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t was not initialized!");
        return NULL;
    }

    // allocate a new vec_t instance
    vec_t *vdup = vec_new();
    if(vdup == NULL) {
        DEBUG_ASSERT(vdup != NULL, "Failed to allocate a copy of vec_t instance!");
        return NULL;
    }

    // construct vdup and resize it
    if(vec_ctor(vdup, v->capacity, v->elsize) != ve_operation_success || vec_resize(vdup, v->len) != ve_operation_success) {
        DEBUG_ASSERT(0, "Failed to copy vec_t data!");
        
        vec_free(vdup);
        return NULL;
    }

    // copy values
    memcpy(vdup->ptr, v->ptr, v->len * v->elsize);

    return vdup;
}

void vec_dtor(vec_t *const v) {
    // if NULL, exit
    if(v == NULL) {
        return;
    }

    // free vec_t contents
    DEBUG_FREE(v->ptr);

    // default-init
    *v = (vec_t) {0};
}

void vec_free(vec_t *v) {
    // if NULL, exit
    if(v == NULL) {
        return;
    }

    // free the vec_t
    DEBUG_FREE(v);

    // reset to NULL
    v = NULL;
}

vec_t *vec_create(const size_t n, const size_t elsize) {
    vec_t *v = vec_new();
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "Unable to allocate vec_t instance!");
        return NULL;
    }

    if(vec_ctor(v, n, elsize) != ve_operation_success) {
        DEBUG_ASSERT(0, "Failed to construct vec_t instance!");

        vec_free(v);
        return NULL;
    }

    return v;
}

void vec_destroy(vec_t *v) {
    vec_dtor(v);
    vec_free(v);
}

vec_t *vec_from(const void *const ptr, const size_t n, const size_t elsize) {
    vec_t *v = vec_create(n, elsize);
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "Failed to create vec_t instance!");
        return NULL;
    }

    // if nothing to copy return an empty vec_t instance
    if(ptr == NULL) {
        return v;
    }

    // copy values
    memmove(v->ptr, ptr, n * elsize);
    v->len = n;

    return v;
}

vec_t *vec_fromi8(const int8_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(int8_t));
}

vec_t *vec_fromu8(const uint8_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(uint8_t));
}

vec_t *vec_fromi16(const int16_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(int16_t));
}

vec_t *vec_fromu16(const uint16_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(uint16_t));
}

vec_t *vec_fromi32(const int32_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(int32_t));
}

vec_t *vec_fromu32(const uint32_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(uint32_t));
}

vec_t *vec_fromi64(const int64_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(int64_t));
}

vec_t *vec_fromu64(const uint64_t *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(uint64_t));
}

vec_t *vec_fromf(const float *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(float));
}

vec_t *vec_fromd(const double *const ptr, const size_t n) {
    return vec_from(ptr, n, sizeof(double));
}

size_t vec_len(const vec_t *const v) {
    DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
    return (v == NULL) ? 0 : v->len;
}

size_t vec_capacity(const vec_t *const v) {
    DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
    return (v == NULL) ? 0 : v->capacity;
}

size_t vec_has_space(const vec_t *const v) {
    DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
    return (v == NULL) ? 0 : (v->capacity - v->len);
}

bool vec_is_empty(const vec_t *const v) {
    DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
    return (v == NULL) ? false : !(v->len);
}

enum VitaError vec_shrink(vec_t *const v) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return ve_error_is_null;
    }

    // if length and capacity are the same, exit the function
    if(v->len == v->capacity) {
        return ve_operation_success;
    }

    // shrink the array capacity to length
    void *newptr = DEBUG_REALLOC(v->ptr, v->len * v->elsize);
    if(newptr == NULL) {
        DEBUG_ASSERT(newptr != NULL, "Failed to reallocate memory!");
        return ve_error_allocation;
    }

    // update values
    v->ptr = newptr;
    v->capacity = v->len;

    return ve_operation_success;
}

enum VitaError vec_clear(vec_t *const v) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return ve_error_is_null;
    }

    // update length
    v->len = 0;

    return ve_operation_success;
}

enum VitaError vec_reserve(vec_t *const v, const size_t n) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return ve_error_is_null;
    }

    // if n = 0, do nothing
    if(!n) {
        return ve_operation_success;
    }

    // reserve memory for additional n elements
    void *newptr = DEBUG_REALLOC(v->ptr, (v->capacity + n) * v->elsize);
    if(newptr == NULL) {
        DEBUG_ASSERT(newptr != NULL, "Failed to reallocate memory!");
        return ve_error_allocation;
    }

    // update values
    v->ptr = newptr;
    v->capacity += n;

    return ve_operation_success;
}

enum VitaError vec_resize(vec_t *const v, const size_t n) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return ve_error_is_null;
    }

    // cannot resize to 0, it's not an error, thus, do nothing
    if(n == 0) {
        return ve_operation_failure;
    }

    if(n == v->capacity) {
        v->len = v->capacity;
        return ve_operation_success;
    }

    // resize vec_t
    void *newptr = DEBUG_REALLOC(v->ptr, n * v->elsize);
    if(newptr == NULL) {
        DEBUG_ASSERT(newptr != NULL, "Failed to reallocate memory!");
        return ve_error_allocation;
    }

    // update values
    v->ptr = newptr;
    v->len = v->capacity = n;

    return ve_operation_success;
}

enum VitaError vec_push(vec_t *const v, const void *const val) {
    if(v == NULL || val == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        DEBUG_ASSERT(val != NULL, "Value supplied is NULL!");
        return ve_error_is_null;
    }

    // check if new memory needs to be allocated
    if(!vec_has_space(v) && vec_reserve(v, v->capacity * CONTAINER_GROWTH_RATE) != ve_operation_success) {
        DEBUG_ASSERT(0, "Failed to reserve more memory!");
        return ve_error_allocation;
    }

    // copy val to vec_t
    memcpy(((char*)(v->ptr) + v->len++ * v->elsize), val, v->elsize);

    return ve_operation_success;
}

enum VitaError vec_pushi8(vec_t *const v, const int8_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushu8(vec_t *const v, const uint8_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushi16(vec_t *const v, const int16_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushu16(vec_t *const v, const uint16_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushi32(vec_t *const v, const int32_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushu32(vec_t *const v, const uint32_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushi64(vec_t *const v, const int64_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushu64(vec_t *const v, const uint64_t val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushf(vec_t *const v, const float val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pushd(vec_t *const v, const double val) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_push(v, &val);
}

enum VitaError vec_pop(vec_t *const v) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return ve_error_is_null;
    }

    // pop the last element
    if(v->len > 0) {
        v->len--;
    }

    return ve_operation_success;
}

void *vec_pop_get(vec_t *const v) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return NULL;
    }

    // pop the last element
    if(v->len > 0) {
        return ((char*)(v->ptr) + --v->len * v->elsize);
    }

    return v->ptr;
}

enum VitaError vec_set(vec_t *const v, const void *const val, const size_t at) {
    if(v == NULL || val == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        DEBUG_ASSERT(val != NULL, "Value supplied is NULL!");
        return ve_error_is_null;
    }

    if(!(at < v->len)) {
        DEBUG_ASSERT(at < v->len, "Out of bounds access at %zu, but length is %zu!", at, v->len);
        return ve_error_out_of_bounds_access;
    }

    // copy val data to str_t
    memcpy(((char*)(v->ptr) + at * v->elsize), val, v->elsize);

    return ve_operation_success;
}

enum VitaError vec_seti8(vec_t *const v, const int8_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_setu8(vec_t *const v, const uint8_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_seti16(vec_t *const v, const int16_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_setu16(vec_t *const v, const uint16_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_seti32(vec_t *const v, const int32_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_setu32(vec_t *const v, const uint32_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_seti64(vec_t *const v, const int64_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_setu64(vec_t *const v, const uint64_t val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_setf(vec_t *const v, const float val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

enum VitaError vec_setd(vec_t *const v, const double val, const size_t at) {
    if(v->elsize != sizeof(val)) {
        DEBUG_ASSERT(v->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return vec_set(v, &val, at);
}

void *vec_get(const vec_t *const v, const size_t at) {
    if(v == NULL || !(at < v->len)) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        DEBUG_ASSERT(at < v->len, "Out of bounds access at %zu, but length is %zu!", at, v->len);
        return NULL;
    }

    return ((char*)(v->ptr) + at * v->elsize);
}

int8_t vec_geti8(const vec_t *const v, const size_t at) {
    return *(int8_t*)(vec_get(v, at));
}

uint8_t vec_getu8(const vec_t *const v, const size_t at) {
    return *(uint8_t*)(vec_get(v, at));
}

int16_t vec_geti16(const vec_t *const v, const size_t at) {
    return *(int16_t*)(vec_get(v, at));
}

uint16_t vec_getu16(const vec_t *const v, const size_t at) {
    return *(uint16_t*)(vec_get(v, at));
}

int32_t vec_geti32(const vec_t *const v, const size_t at) {
    return *(int32_t*)(vec_get(v, at));
}

uint32_t vec_getu32(const vec_t *const v, const size_t at) {
    return *(uint32_t*)(vec_get(v, at));
}

int64_t vec_geti64(const vec_t *const v, const size_t at) {
    return *(int64_t*)(vec_get(v, at));
}

uint64_t vec_getu64(const vec_t *const v, const size_t at) {
    return *(uint64_t*)(vec_get(v, at));
}

float vec_getf(const vec_t *const v, const size_t at) {
    return *(float*)(vec_get(v, at));
}

double vec_getd(const vec_t *const v, const size_t at) {
    return *(double*)(vec_get(v, at));
}

enum VitaError vec_insert(vec_t *const v, const void *const val, const size_t at) {
    if(v == NULL || val == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        DEBUG_ASSERT(val != NULL, "Value supplied is NULL!");
        return ve_error_is_null;
    }

    if(!(at < v->len)) {
        DEBUG_ASSERT(at < v->len, "Cannot insert value at index %zu, because length is %zu!", at, v->len);
        return ve_error_out_of_bounds_access;
    }

    // check if new memory needs to be allocated
    if(!vec_has_space(v) && !vec_reserve(v, v->capacity * CONTAINER_GROWTH_RATE)) {
        DEBUG_ASSERT(0, "Failed to reserve more memory!");
        return ve_error_allocation;
    }

    // shift values by one value to the end of the vec_t
    memmove(((char*)(v->ptr) + (at + 1) * v->elsize), ((char*)(v->ptr) + at * v->elsize), ((v->len - at) * v->elsize));

    // copy the str contents to str from the specified index
    memcpy(((char*)(v->ptr) + at * v->elsize), val, v->elsize);

    // set new length
    v->len++;

    return ve_operation_success;
}

enum VitaError vec_remove(vec_t *const v, const size_t at, const enum RemoveStrategy rs) {
    if(v == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return ve_error_is_null;
    }

    if(!(at < v->len)) {
        DEBUG_ASSERT(at < v->len, "Cannot remove value at index %zu, because length is %zu!", at, v->len);
        return ve_error_out_of_bounds_access;
    }

    // check remove strategy
    if(rs == rs_stable) {
        memmove((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (at + 1) * v->elsize, (v->len - at) * v->elsize);
    } else {
        gswap((char*)(v->ptr) + at * v->elsize, (char*)(v->ptr) + (v->len - 1) * v->elsize, v->elsize);
    }

    // set new length
    v->len--;

    return ve_operation_success;
}

int64_t vec_contains(const vec_t *const v, const void *const val) {
    if(v == NULL || val == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        return -1;
    }

    size_t i = 0;
    for (char *iter = v->ptr; iter != (char*)(v->ptr) + v->len * v->elsize; iter += v->elsize, i++) {
        if(memcmp(iter, val, v->elsize) == 0) {
            return i;
        }
    }

    return -1;
}

void vec_apply(const vec_t *const v, void (*func)(void*, size_t)) {
    if(v == NULL || func == NULL) {
        DEBUG_ASSERT(v != NULL, "vec_t instance was not initialized!");
        DEBUG_ASSERT(func != NULL, "func supplied is NULL!");
        return;
    }

    size_t i = 0;
    for(char *iter = v->ptr; iter != (char*)(v->ptr) + v->len * v->elsize; iter += v->elsize, i++) {
        func(iter, i);
    }
}
