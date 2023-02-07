#include "vita/core/version.h"

vt_version_t vt_get_version(void) {
    vt_version_t v = {
        .major = VT_VITA_VERSION_MAJOR,
        .minor = VT_VITA_VERSION_MINOR,
        .patch = VT_VITA_VERSION_PATCH,
        .str = VT_VITA_VERSION
    };

    return v;
}


