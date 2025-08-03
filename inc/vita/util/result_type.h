#ifndef VITA_UTIL_RESULT_TYPE_H
#define VITA_UTIL_RESULT_TYPE_H

/** RESULT TYPE MODULE
 * This module contains the result type structure for handling results and returning errors similar to Go.
*/

#include "vita/core/core.h"

/// Define result type macro
#define VITA_DEFINE_RESULT_TYPE(type, name) \
    struct name {                \
        type value;              \
        const char *const error; \
    }

// result ok
#define VT_RESULT_OK(rt, value) ((rt){.value=value})

// result error
#define VT_RESULT_ERROR(rt, error) ((rt){.error=error})

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

#endif // VITA_ITIL_RESULT_TYPE_H
