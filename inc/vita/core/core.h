#ifndef VITA_CORE_H
#define VITA_CORE_H

/** VITA_CORE MODULE
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
    - vt_get_current_timestamp
    - vt_get_vita_error_str
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
#include <time.h>

// OS specific setup
#if defined(_WIN32) || defined(_WIN64)
    #define strncpy(d, s, n) strncpy_s(d, n, s, n)
    #define __SOURCE_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
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
#define VT_FOREACH(iter, from, to) for(size_t iter = from; iter < to; iter++)

// data types for internal usage
enum VitaTypeInfo {
    // fixed size
    vt_type_int8,     // int8_t
    vt_type_uint8,    // uint8_t
    vt_type_int16,    // int16_t
    vt_type_uint16,   // uint16_t
    vt_type_int32,    // int32_t
    vt_type_uint32,   // uint32_t
    vt_type_int64,    // int64_t
    vt_type_uint64,   // uint64_t

    // floats
    vt_type_float,    // float
    vt_type_double,   // double
    vt_type_real,     // long double

    // other
    vt_type_bool,     // bool
    vt_type_char,     // char
    vt_type_cstr,     // char*

    // vita
    vt_type_str,      // vt_str_t
    vt_type_vec,      // vt_vec_t
    vt_type_plist,    // vt_plist_t

    vt_type_unknown,  // unknown data type
    vt_type_count     // number of elements
};

// float, double, real (long double alias)
typedef long double real;

// removing elements from array
enum VitaRemoveStrategy {
    vt_remove_stategy_stable,  // keep ordering
    vt_remove_stategy_fast,    // ordering doesn't matter
    vt_remove_stategy_count    // number of elements
};

// define all vita errors
#define VT_i_GENERATE_VITA_STATUS(apply) \
    apply(vt_status_error_is_null)                  /* element wasn't initialized or is NULL */ \
    apply(vt_status_error_allocation)               /* failed to allocate or reallocate memory */ \
    apply(vt_status_error_invalid_arguments)        /* invalid arguments supplied */ \
    apply(vt_status_error_out_of_memory)            /* not enough memory/space, allocate more */ \
    apply(vt_status_error_out_of_bounds_access)     /* accessing memory beyond allocated size */ \
    apply(vt_status_error_incompatible_datatype)    /* working with different datatypes */ \
    apply(vt_status_error_conversion)               /* failed to convert one type to another */ \
    apply(vt_status_error_element_not_found)        /* element was not found */ \
    apply(vt_status_operation_failure)              /* failed to perform an action */ \
    apply(vt_status_operation_success)              /* all good */ \
    apply(vt_status_count)                          /* number of elements */

// generate vita errors enum
#define X(a) a,
enum VitaStatus {
    VT_i_GENERATE_VITA_STATUS(X)
};
#undef X

// see allocator/mallocator.h
struct VitaBaseAllocatorType;

// base array type for all array-like primitives
struct VitaBaseArrayType {
    // data pointers
    union {
        void *ptr;
        void **ptr2;
    };

    // allocator: if `NULL`, then calloc/realloc/free is used
    struct VitaBaseAllocatorType *alloctr;

    // data information
    size_t len;         // container length
    size_t capacity;    // container capacity
    size_t elsize;      // container element size
    size_t slider_idx;  // container slider that adjusts where ptr points to
};

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
extern const char *vt_get_vita_error_str(const enum VitaStatus e);

#endif // VITA_CORE_H
