#ifndef VITA_VERSION_H
#define VITA_VERSION_H

/** VITA_CORE MODULE
    - //
*/

#include "core.h"

// defines
#define VITA_VERSION_MAJOR 0
#define VITA_VERSION_MINOR 3
#define VITA_VERSION_PATCH 0
#define VITA_VERSION STRINGOF(PCAT(PCAT(PCAT(PCAT(VITA_VERSION_MAJOR, .), VITA_VERSION_MINOR), .), VITA_VERSION_PATCH))

typedef struct {
    uint8_t major, minor, patch;
    const char *str;
} version_t;

/** Query Vita version
    @returns version_t struct containing major, minor, patch and full version str
*/
extern version_t get_vita_version(void);

#endif // VITA_VERSION_H
