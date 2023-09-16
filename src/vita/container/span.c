#include "vita/container/span.h"

vt_span_t vt_span_from(void *ptr, const size_t length, const size_t elsize) {
    return (vt_span_t) {
        .instance.ptr = ptr,
        .instance.len = length,
        .instance.elsize = elsize,
    };
}

size_t vt_span_len(const vt_span_t span) {
    return span.instance.len;
}

void *vt_span_get(const vt_span_t span, const size_t at);
void vt_span_set(vt_span_t *const span, const void *const val, const size_t at);

#define VT_INSTANTIATE_SPAN_SET(T, t)                                          \
    void vt_span_set##t(vt_span_t *const span, const T val, const size_t at) { \
        vt_span_set(span, &val, at);                                           \
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
