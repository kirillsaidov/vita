# Get compiler information

This chapter explains how to get compiler information. For more details refer to [`core/compiler.h`](../../inc/vita/core/compiler.h).

## Contents
1. [Check for available compiler](page9.md#check-for-available-compiler)
2. [Get compiler version](page9.md#get-compiler-version)

### Check for available compiler
```c
if (VT_COMPILER_ID == VT_COMPILER_ID_CLANG) {
    // ...
} else if (VT_COMPILER_ID == VT_COMPILER_ID_GCC) {
    // ...
} else {
    // ...
}
```

### Get compiler version
```c
const vt_version_vt v = vt_compiler_get_version();

// print compiler information
printf("Compiler id: %d\n", VT_COMPILER_ID);         // 0
printf("Compiler name: %s\n", VT_COMPILER_NAME);     // gcc
printf("Compiler vendor: %s\n", VT_COMPILER_VENDOR); // GCC

// compiler version
printf("Compiler version: %s\n", v.str); // 13.0.1
printf("Compiler major: %d\n", v.major); // 13
printf("Compiler minor: %d\n", v.minor); // 0
printf("Compiler patch: %d\n", v.patch); // 1
```

**[ [Back](page8.md) | [Contents](VITA.md) ]**
