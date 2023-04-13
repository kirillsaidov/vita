#include "vita/core/conv.h"

int64_t vt_conv_str_to_i64(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if value is numeric
    if(!vt_str_is_numeric(z, VT_MAX_PRECISION)) {
        VT_DEBUG_PRINTF("%s: Must be digits only, not \"<%s>\".\n", vt_get_vita_error_str(vt_ve_error_type_conversion), z);
        return 0;
    }   

    // save value
    return strtoll(z, NULL, 10); // base = 10
}

uint64_t vt_conv_str_to_u64(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if value is numeric
    if(!vt_str_is_numeric(z, VT_MAX_PRECISION)) {
        VT_DEBUG_PRINTF("%s: Must be digits only, not \"<%s>\".\n", vt_get_vita_error_str(vt_ve_error_type_conversion), z);
        return 0;
    }

    // save value
    return strtoull(z, NULL, 10); // base = 10
}

double vt_conv_str_to_double(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if value is numeric
    if(!vt_str_is_numeric(z, VT_MAX_PRECISION)) {
        VT_DEBUG_PRINTF("%s: Must be digits only, not \"<%s>\".\n", vt_get_vita_error_str(vt_ve_error_type_conversion), z);
        return 0;
    }

    // save value
    return strtod(z, NULL);
}

real vt_conv_str_to_real(const char *const z) {
    // check for invalid input
    VT_DEBUG_ASSERT(z != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if value is numeric
    if(!vt_str_is_numeric(z, VT_MAX_PRECISION)) {
        VT_DEBUG_PRINTF("%s: Must be digits only, not \"<%s>\".\n", vt_get_vita_error_str(vt_ve_error_type_conversion), z);
        return 0;
    }

    // save value
    return strtold(z, NULL);
}

#define VT_INSTANTIATE_CONV_TO_STR(T, t, fmt)                                                       \
    void vt_conv_##t##_to_str(char *const buf, const size_t buf_len, const T val) {                 \
        VT_DEBUG_ASSERT(buf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments)); \
                                                                                                    \
        const size_t required_len = snprintf(NULL, 0, fmt, val);                                    \
        VT_ENFORCE(buf_len >= required_len + 1, "%s\n", "Buffer size is too small!");               \
                                                                                                    \
        const size_t ret_len = snprintf(buf, required_len, fmt, val);                               \
        VT_ENFORCE(ret_len == required_len, "%s\n", "Conversion failed!");                          \
    }
VT_INSTANTIATE_CONV_TO_STR(int64_t, int64, "%ld")
VT_INSTANTIATE_CONV_TO_STR(uint64_t, uint64, "%zu")
VT_INSTANTIATE_CONV_TO_STR(double, double, "%lf")
VT_INSTANTIATE_CONV_TO_STR(real, real, "%Lf")
#undef VT_INSTANTIATE_CONV_TO_STR





