#include "vita/core/compiler.h"

vt_version_t vt_compiler_version_get(void) {
    vt_version_t v = {
        .major = VT_COMPILER_VERSION_MAJOR,
        .minor = VT_COMPILER_VERSION_MINOR,
        .patch = VT_COMPILER_VERSION_PATCH,
        .str = VT_COMPILER_VERSION
    };

    return v;
}
