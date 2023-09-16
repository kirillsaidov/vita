#include "vita/container/span.h"

vt_span_t vt_span_from(void *ptr, const size_t length, const size_t elsize) {
    // check for invalid input
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(length > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return (vt_span_t) {
        .instance.ptr = ptr,
        .instance.len = length,
        .instance.elsize = elsize,
    };
}

vt_span_t vt_span_from_to(void *ptr, const size_t from_idx, const size_t to_idx, const size_t elsize) {
    // check for invalid input
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(from_idx > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(to_idx > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(elsize > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return (vt_span_t) {
        .instance.ptr = (char*)ptr + from_idx * elsize,
        .instance.len = to_idx - from_idx,
        .instance.elsize = elsize,
    };
}

size_t vt_span_len(const vt_span_t span) {
    return span.instance.len;
}

void *vt_span_get(const vt_span_t span, const size_t at) {
    VT_DEBUG_ASSERT(
        at < span.instance.len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        span.instance.len
    );

    return vt_array_get(&span.instance, at);
}

#define VT_INSTANTIATE_SPAN_GET(T, t)                         \
    T vt_span_get##t(const vt_span_t span, const size_t at) { \
        return *(T*)(vt_span_get(span, at));                  \
    }
VT_INSTANTIATE_SPAN_GET(int8_t, i8)
VT_INSTANTIATE_SPAN_GET(uint8_t, u8)
VT_INSTANTIATE_SPAN_GET(int16_t, i16)
VT_INSTANTIATE_SPAN_GET(uint16_t, u16)
VT_INSTANTIATE_SPAN_GET(int32_t, i32)
VT_INSTANTIATE_SPAN_GET(uint32_t, u32)
VT_INSTANTIATE_SPAN_GET(int64_t, i64)
VT_INSTANTIATE_SPAN_GET(uint64_t, u64)
VT_INSTANTIATE_SPAN_GET(float, f)
VT_INSTANTIATE_SPAN_GET(double, d)
VT_INSTANTIATE_SPAN_GET(real, r)
#undef VT_INSTANTIATE_SPAN_GET

void vt_span_set(vt_span_t span, const void *const val, const size_t at) {
    // check for invalid input
    VT_DEBUG_ASSERT(val != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(
        at < span.instance.len,
        "%s: Out of bounds memory access at %zu, but length is %zu!\n", 
        vt_status_to_str(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS), 
        at, 
        span.instance.len
    );

    vt_array_set(&span.instance, val, at);
}

#define VT_INSTANTIATE_SPAN_SET(T, t)                                   \
    void vt_span_set##t(vt_span_t span, const T val, const size_t at) { \
        vt_span_set(span, &val, at);                                    \
    }
VT_INSTANTIATE_SPAN_SET(int8_t, i8)
VT_INSTANTIATE_SPAN_SET(uint8_t, u8)
VT_INSTANTIATE_SPAN_SET(int16_t, i16)
VT_INSTANTIATE_SPAN_SET(uint16_t, u16)
VT_INSTANTIATE_SPAN_SET(int32_t, i32)
VT_INSTANTIATE_SPAN_SET(uint32_t, u32)
VT_INSTANTIATE_SPAN_SET(int64_t, i64)
VT_INSTANTIATE_SPAN_SET(uint64_t, u64)
VT_INSTANTIATE_SPAN_SET(float, f)
VT_INSTANTIATE_SPAN_SET(double, d)
VT_INSTANTIATE_SPAN_SET(real, r)
#undef VT_INSTANTIATE_SPAN_SET
