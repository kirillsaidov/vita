#ifndef VITA_CORE_H
#define VITA_CORE_H

/** CORE MODULE
 * This module is a collection of all common definitions and code needed by the rest of the library.

 * Macros:
    - VT_MALLOC
    - VT_CALLOC
    - VT_REALLOC
    - VT_FREE
    - VT_PCAT
    - VT_STRING_OF
    - VT_AS

 * Functions:
    - vt_malloc
    - vt_calloc
    - vt_realloc
    - vt_free
    - vt_gswap
    - vt_status_to_str
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

// OS specific setup
#if defined(_WIN32) || defined(_WIN64)
    #define __SOURCE_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
    #define strncpy(d, s, n) strncpy_s(d, n, s, n)
#else
    #define __SOURCE_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

// memory management macros
#define VT_MALLOC(bytes) vt_malloc(bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_CALLOC(bytes) vt_calloc(bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_REALLOC(ptr, bytes) vt_realloc(ptr, bytes, __SOURCE_FILENAME__, __func__, __LINE__)
#define VT_FREE(ptr) vt_free(ptr)

// constants
#define VT_ARRAY_DEFAULT_INIT_ELEMENTS 16
#define VT_ARRAY_DEFAULT_GROWTH_RATE 2

// this is needed in order to properly expand macros if one macro is inserted into another
#define VT_i_PCAT_NX(x, y) x ## y           // preprocessor concatenation
#define VT_i_STR_EXPAND(x) #x               // expands macros to its value

// useful macros
#define VT_PCAT(x, y) VT_i_PCAT_NX(x, y)    // preprocessor concatenation
#define VT_STRING_OF(x) VT_i_STR_EXPAND(x)  // converts to string
#define VT_AS(type, x) ((type)(x))          // cast

// foreach
#define VT_FOREACH(iter, from, to) for (size_t iter = from; iter < to; iter++)

// foreach reverse
#define VT_FOREACH_R(iter, from, to) for (size_t iter = to; iter > from; iter--)

// foreach with step
#define VT_FOREACH_STEP(iter, from, to, step) for (size_t iter = from; iter < to; iter += step)

// data types for internal usage
enum VitaTypeInfo {
    // fixed size
    VT_TYPE_INT8,     // int8_t
    VT_TYPE_UINT8,    // uint8_t
    VT_TYPE_INT16,    // int16_t
    VT_TYPE_UINT16,   // uint16_t
    VT_TYPE_INT32,    // int32_t
    VT_TYPE_UINT32,   // uint32_t
    VT_TYPE_INT64,    // int64_t
    VT_TYPE_UINT64,   // uint64_t

    // floats
    VT_TYPE_FLOAT,    // float
    VT_TYPE_DOUBLE,   // double
    VT_TYPE_REAL,     // long double

    // other
    VT_TYPE_BOOL,     // bool
    VT_TYPE_CHAR,     // char
    VT_TYPE_CSTR,     // char*

    // vita
    VT_TYPE_STR,      // vt_str_t
    VT_TYPE_VEC,      // vt_vec_t
    VT_TYPE_PLIST,    // vt_plist_t

    VT_TYPE_UNKNOWN,  // unknown data type
    VT_TYPE_COUNT     // number of elements
};

// float, double, real
typedef long double real;

// removing elements from array
enum VitaRemoveStrategy {
    VT_REMOVE_STRATEGY_STABLE,  // keep ordering
    VT_REMOVE_STRATEGY_FAST,    // ordering doesn't matter
    VT_REMOVE_STRATEGY_COUNT    // number of elements
};

// define all vita errors
#define VT_i_GENERATE_VITA_STATUS(apply) \
    apply(VT_STATUS_ERROR_IS_NULL)                  /* element wasn't initialized or is NULL */ \
    apply(VT_STATUS_ERROR_ALLOCATION)               /* failed to allocate or reallocate memory */ \
    apply(VT_STATUS_ERROR_INVALID_ARGUMENTS)        /* invalid arguments supplied */ \
    apply(VT_STATUS_ERROR_OUT_OF_MEMORY)            /* not enough memory/space, allocate more */ \
    apply(VT_STATUS_ERROR_OUT_OF_BOUNDS_ACCESS)     /* accessing memory beyond allocated size */ \
    apply(VT_STATUS_ERROR_INCOMPATIBLE_DATATYPE)    /* working with different datatypes */ \
    apply(VT_STATUS_ERROR_CONVERSION)               /* failed to convert one type to another */ \
    apply(VT_STATUS_ERROR_ELEMENT_NOT_FOUND)        /* element was not found */ \
    apply(VT_STATUS_OPERATION_FAILURE)              /* failed to perform an action */ \
    apply(VT_STATUS_OPERATION_SUCCESS)              /* all good */ \
    apply(VT_STATUS_COUNT)                          /* number of elements */

// generate vita errors enum
#define X(a) a,
enum VitaStatus {
    VT_i_GENERATE_VITA_STATUS(X)
};
#undef X

/* -------------- MEMORY MANAGEMENT -------------- */
/**
 * This module is meant as a safe replacement for stdc alloc functions that can never fail. 
 * In theory, memory should never fail on your computer. And if it does fail, then there is
 * probably an issue with your device. 
*/

/** Allocates memory
    @param bytes amount to allocate
    @param file `__SOURCE_FILENAME__`
    @param func `__func__`
    @param line `__LINE__`

    @returns ptr to allocated memory

    @note exits upon failure
*/
extern void *vt_malloc(const size_t bytes, const char *const file, const char *const func, const size_t line);

/** Allocates memory and initiazes to zero
    @param bytes amount to allocate
    @param file `__SOURCE_FILENAME__`
    @param func `__func__`
    @param line `__LINE__`

    @returns ptr to allocated memory

    @note exits upon failure
*/
extern void *vt_calloc(const size_t bytes, const char *const file, const char *const func, const size_t line);

/** Reallocates memory
    @param ptr pointer to memory address
    @param bytes amount to allocate
    @param file `__SOURCE_FILENAME__`
    @param func `__func__`
    @param line `__LINE__`

    @returns ptr to allocated memory

    @note exits upon failure
*/
extern void *vt_realloc(void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line);

/** Frees memory
    @param ptr pointer to memory
*/
extern void vt_free(void *ptr);

/** Copies data from source to destination memory buffer
    @param dest pointer to destination memory address
    @param src pointer to source memory address
    @returns dest: a pointer to the destination

    @note exits upon failure
    @note if `dest == NULL`, then destination is allocated with size of `bytes`
*/
extern void *vt_memmove(void *dest, const void *const src, const size_t bytes);

/* ------------- OTHER FUNCTIONALITY ------------- */

/** Generic swap
    @param a first value
    @param b second value
    @param elsize element size

    @returns `true` upon success
*/
extern bool vt_gswap(void* a, void* b, const size_t elsize);

/** Returns a vita error string from vita error code
    @param e vita error code
    @returns C string upon success, `NULL` otherwise
*/
extern const char *vt_status_to_str(const enum VitaStatus e);

#endif // VITA_CORE_H
