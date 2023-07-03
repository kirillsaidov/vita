#include "vita/core/conv.h"

static float vt_strtof(const char *z, char **zend, int base) { (void)base; return strtof(z, zend); }
static double vt_strtod(const char *z, char **zend, int base) { (void)base; return strtod(z, zend); }
static real vt_strtold(const char *z, char **zend, int base) { (void)base; return strtold(z, zend); }

#define VT_INSTANTIATE_CONV_FROM_STR(T, t, F)                                                       \
    T vt_conv_str_to_##t(const char *const z) {                                                     \
        VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));   \
                                                                                                    \
        if(!vt_str_is_numeric(z, VT_MAX_PRECISION)) {                                               \
            VT_DEBUG_PRINTF(                                                                        \
                "%s: Must be digits only, not \"<%s>\".\n",                                         \
                vt_status_to_str(VT_STATUS_ERROR_CONVERSION),                                 \
                z                                                                                   \
            );                                                                                      \
            return 0;                                                                               \
        }                                                                                           \
                                                                                                    \
        return (T)F(z, NULL, 10);                                                                   \
    }
VT_INSTANTIATE_CONV_FROM_STR(int8_t, i8, strtol)
VT_INSTANTIATE_CONV_FROM_STR(uint8_t, u8, strtoul)
VT_INSTANTIATE_CONV_FROM_STR(int16_t, i16, strtol)
VT_INSTANTIATE_CONV_FROM_STR(uint16_t, u16, strtoul)
VT_INSTANTIATE_CONV_FROM_STR(int32_t, i32, strtol)
VT_INSTANTIATE_CONV_FROM_STR(uint32_t, u32, strtoul)
VT_INSTANTIATE_CONV_FROM_STR(int64_t, i64, strtoll)
VT_INSTANTIATE_CONV_FROM_STR(uint64_t, u64, strtoull)
VT_INSTANTIATE_CONV_FROM_STR(float, f, vt_strtof)
VT_INSTANTIATE_CONV_FROM_STR(double, d, vt_strtod)
VT_INSTANTIATE_CONV_FROM_STR(real, r, vt_strtold)
#undef VT_INSTANTIATE_CONV_FROM_STR 

#define VT_INSTANTIATE_CONV_TO_STR(T, t, fmt)                                                       \
    void vt_conv_##t##_to_str(char *const buf, const size_t buf_len, const T val) {                 \
        VT_DEBUG_ASSERT(buf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS)); \
                                                                                                    \
        const size_t required_len = snprintf(NULL, 0, "%" fmt, val);                                \
        VT_ENFORCE(buf_len >= required_len + 1, "%s\n", "Buffer size is too small!");               \
                                                                                                    \
        const size_t ret_len = snprintf(buf, required_len, "%" fmt, val);                           \
        VT_ENFORCE(ret_len == required_len, "%s\n", "Conversion failed!");                          \
    }
VT_INSTANTIATE_CONV_TO_STR(int8_t, i8, PRId8)
VT_INSTANTIATE_CONV_TO_STR(uint8_t, u8, PRIu8)
VT_INSTANTIATE_CONV_TO_STR(int16_t, i16, PRId16)
VT_INSTANTIATE_CONV_TO_STR(uint16_t, u16, PRIu16)
VT_INSTANTIATE_CONV_TO_STR(int32_t, i32, PRId32)
VT_INSTANTIATE_CONV_TO_STR(uint32_t, u32, PRIu32)
VT_INSTANTIATE_CONV_TO_STR(int64_t, i64, PRId64)
VT_INSTANTIATE_CONV_TO_STR(uint64_t, u64, PRIu64)
VT_INSTANTIATE_CONV_TO_STR(float, f, "f")
VT_INSTANTIATE_CONV_TO_STR(double, d, "lf")
VT_INSTANTIATE_CONV_TO_STR(real, r, "Lf")
#undef VT_INSTANTIATE_CONV_TO_STR





