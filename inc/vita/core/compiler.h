#ifndef VITA_CORE_COMPILER_H
#define VITA_CORE_COMPILER_H

/** COMPILER MODULE
    - vt_compiler_get_version
*/

#include "version.h"

#if defined(__clang__)
    #define VT_COMPILER_NAME "clang"
    #define VT_COMPILER_VENDOR "LLVM Clang"
    #define VT_COMPILER_VERSION_MAJOR __clang_major__
    #define VT_COMPILER_VERSION_MINOR __clang_minor__
    #define VT_COMPILER_VERSION_PATCH __clang_patchlevel__
#elif defined(__GNUC__) || defined(__GNUG__)
    #define VT_COMPILER_NAME "gcc"
    #define VT_COMPILER_VENDOR "GCC"
    #define VT_COMPILER_VERSION_MAJOR __GNUC__
    #define VT_COMPILER_VERSION_MINOR __GNUC_MINOR__
    #define VT_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#else
    #define VT_COMPILER_NAME "UNKNOWN"
    #define VT_COMPILER_VENDOR "UNKNOWN"
    #define VT_COMPILER_VERSION_MAJOR 0
    #define VT_COMPILER_VERSION_MINOR 0
    #define VT_COMPILER_VERSION_PATCH 0
#endif

#define VT_COMPILER_VERSION VT_STRING_OF(VT_PCAT(VT_PCAT(VT_PCAT(VT_PCAT(VT_COMPILER_VERSION_MAJOR, .), VT_COMPILER_VERSION_MINOR), .), VT_COMPILER_VERSION_PATCH))

/** Query compiler version (currently supports GCC and CLANG only)
    @returns vt_version_t struct containing major, minor, patch and full version str
*/
extern vt_version_t vt_compiler_get_version(void);

#endif // VITA_CORE_COMPILER_H


