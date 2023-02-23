#include "vita/algorithm/comparison.h"

/// instantiate vt_cmp_min functions
#define VT_INSTANTIATE_CMP_MIN(T, t)        \
    T vt_cmp_min##t(const T x, const T y) { \
        return x < y ? x : y;               \
    }
VT_INSTANTIATE_CMP_MIN(int8_t, i8)
VT_INSTANTIATE_CMP_MIN(uint8_t, u8)
VT_INSTANTIATE_CMP_MIN(int16_t, i16)
VT_INSTANTIATE_CMP_MIN(uint16_t, u16)
VT_INSTANTIATE_CMP_MIN(int32_t, i32)
VT_INSTANTIATE_CMP_MIN(uint32_t, u32)
VT_INSTANTIATE_CMP_MIN(int64_t, i64)
VT_INSTANTIATE_CMP_MIN(uint64_t, u64)
VT_INSTANTIATE_CMP_MIN(float, f)
VT_INSTANTIATE_CMP_MIN(double, d)
VT_INSTANTIATE_CMP_MIN(real, r)
#undef VT_INSTANTIATE_CMP_MIN

/// instantiate vt_cmp_max functions
#define VT_INSTANTIATE_CMP_MAX(T, t)        \
    T vt_cmp_max##t(const T x, const T y) { \
        return x > y ? x : y;               \
    }
VT_INSTANTIATE_CMP_MAX(int8_t, i8)
VT_INSTANTIATE_CMP_MAX(uint8_t, u8)
VT_INSTANTIATE_CMP_MAX(int16_t, i16)
VT_INSTANTIATE_CMP_MAX(uint16_t, u16)
VT_INSTANTIATE_CMP_MAX(int32_t, i32)
VT_INSTANTIATE_CMP_MAX(uint32_t, u32)
VT_INSTANTIATE_CMP_MAX(int64_t, i64)
VT_INSTANTIATE_CMP_MAX(uint64_t, u64)
VT_INSTANTIATE_CMP_MAX(float, f)
VT_INSTANTIATE_CMP_MAX(double, d)
VT_INSTANTIATE_CMP_MAX(real, r)
#undef VT_INSTANTIATE_CMP_MAX

