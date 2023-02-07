#ifndef VITA_VERSION_H
#define VITA_VERSION_H

/** VITA_CORE MODULE
    - vt_get_version
*/

#include "core.h"

// defines
#define VT_VITA_VERSION_MAJOR 0
#define VT_VITA_VERSION_MINOR 3
#define VT_VITA_VERSION_PATCH 1
#define VT_VITA_VERSION VT_STRINGOF(VT_PCAT(VT_PCAT(VT_PCAT(VT_PCAT(VT_VITA_VERSION_MAJOR, .), VT_VITA_VERSION_MINOR), .), VT_VITA_VERSION_PATCH))

typedef struct {
    uint8_t major, minor, patch;
    const char *str;
} vt_version_t;

/** Query Vita version
    @returns version_t struct containing major, minor, patch and full version str
*/
extern vt_version_t vt_get_version(void);

#endif // VITA_VERSION_H
