#ifndef VITA_UTIL_RESULT_TYPE_H
#define VITA_UTIL_RESULT_TYPE_H

/** RESULT TYPE MODULE
 * This module contains the result type structure for handling results and returning errors similar to Go.
*/

#include "vita/core/core.h"

/// Result type with code, value and error fields
#define VITA_DEFINE_RESULT_TYPE(type, name) \
    struct name {                \
        int code;                \
        type value;              \
        const char *error;       \
    }

/// Return result ok.
#define VT_RESULT_OK(rt, v) ((rt){.value=v})

/// Return error message (with default code=-1)
#define VT_RESULT_ERROR(rt, e) ((rt){.code=-1, .error=e})

// result error message with custom code
#define VT_RESULT_ERRORC(rt, c, e) ((rt){.code=c, .error=e})

// define specific result types
VITA_DEFINE_RESULT_TYPE(int8_t, VitaResultI8);
VITA_DEFINE_RESULT_TYPE(uint8_t, VitaResultUI8);
VITA_DEFINE_RESULT_TYPE(int16_t, VitaResultI16);
VITA_DEFINE_RESULT_TYPE(uint16_t, VitaResultUI16);
VITA_DEFINE_RESULT_TYPE(int32_t, VitaResultI32);
VITA_DEFINE_RESULT_TYPE(uint32_t, VitaResultUI32);
VITA_DEFINE_RESULT_TYPE(int64_t, VitaResultI64);
VITA_DEFINE_RESULT_TYPE(uint64_t, VitaResultUI64);
VITA_DEFINE_RESULT_TYPE(float, VitaResultF);
VITA_DEFINE_RESULT_TYPE(double, VitaResultD);
VITA_DEFINE_RESULT_TYPE(real, VitaResultR);
VITA_DEFINE_RESULT_TYPE(bool, VitaResultBool);
VITA_DEFINE_RESULT_TYPE(char, VitaResultChar);
VITA_DEFINE_RESULT_TYPE(char*, VitaResultCstr);
VITA_DEFINE_RESULT_TYPE(void*, VitaResultVoidPtr);

#endif // VITA_ITIL_RESULT_TYPE_H
