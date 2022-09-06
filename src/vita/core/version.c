#include "vita/core/version.h"

version_t get_vita_version(void) {
    version_t v = {
        .major = VITA_VERSION_MAJOR,
        .minor = VITA_VERSION_MINOR,
        .patch = VITA_VERSION_PATCH,
        .str = VITA_VERSION
    };

    return v;
}