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
    - vt_bat_head
    - vt_bat_len
    - vt_bat_capacity
    - vt_bat_has_space
    - vt_bat_elsize
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

// getting file name
#if defined(_WIN32) || defined(_WIN64)
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
#define VT_DEFAULT_INIT_ELEMENTS 10
#define VT_CONTAINER_GROWTH_RATE 2

// this is needed in order to properly expand macros if one macro is inserted into another
#define VT_i_PCAT_NX(x, y) x ## y           // preprocessor concatenation
#define VT_i_STR_EXPAND(x) #x               // expands macros to its value

#define VT_PCAT(x, y) VT_i_PCAT_NX(x, y)    // preprocessor concatenation
#define VT_STRING_OF(x) VT_i_STR_EXPAND(x)  // converts to string
#define VT_AS(type, x) ((type)(x))          // cast

// data types for internal usage
enum VitaDataType {
    // fixed size
    vt_dt_int8,     // int8_t
    vt_dt_uint8,    // uint8_t
    vt_dt_int16,    // int16_t
    vt_dt_uint16,   // uint16_t
    vt_dt_int32,    // int32_t
    vt_dt_uint32,   // uint32_t
    vt_dt_int64,    // int64_t
    vt_dt_uint64,   // uint64_t

    // floats
    vt_dt_float,    // float
    vt_dt_double,   // double
    vt_dt_real,     // long double

    // other
    vt_dt_bool,     // bool
    vt_dt_char,     // char
    vt_dt_cstr,     // char*

    // vita
    vt_dt_str,      // vt_str_t
    vt_dt_vec,      // vt_vec_t
    vt_dt_plist,    // vt_plist_t

    vt_dt_unknown,  // unknown data type
    vt_dt_count     // number of elements
};

// float, double, real (long double alias)
typedef long double real;

// removing elements from array
enum VitaRemoveStrategy {
    vt_rs_stable,  // keep ordering
    vt_rs_fast,    // ordering doesn't matter
    vt_rs_count    // number of elements
};

// define all vita errors
#define VT_i_GENERATE_VITA_ERRORS(apply) \
    apply(vt_ve_error_is_null)                      /* element wasn't initialized or is NULL */ \
    apply(vt_ve_error_allocation)                   /* failed to allocate or reallocate memory */ \
    apply(vt_ve_error_invalid_arguments)            /* invalid arguments supplied */ \
    apply(vt_ve_error_out_of_memory)                /* not enough memory/space, allocate more */ \
    apply(vt_ve_error_out_of_bounds_access)         /* accessing memory beyond allocated size */ \
    apply(vt_ve_error_incompatible_datatype)        /* working with different datatypes */ \
    apply(vt_ve_error_type_conversion)              /* failed to convert one type to another */ \
    apply(vt_ve_operation_element_not_found)        /* element was not found */ \
    apply(vt_ve_operation_failure)                  /* failed to perform an action */ \
    apply(vt_ve_operation_success)                  /* all good */ \
    apply(vt_ve_count)                              /* number of elements */

// generate vita errors enum
#define X(a) a,
enum VitaError {
    VT_i_GENERATE_VITA_ERRORS(X)
};
#undef X

// base container type for all primitives
struct VitaBaseArrayType {
    // data pointers
    union {
        void *ptr;
        void **ptr2;
    };

    // data information
    size_t len;         // container length
    size_t capacity;    // container capacity
    size_t elsize;      // container element size
    size_t slider_idx;  // container slider that adjusts where ptr points to
};

/* ------------- BASE CONTAINER TYPE ------------- */

/** Allocates memory for VitaBaseArrayType structure
    @returns `VitaBaseArrayType*` upon success, `NULL` otherwise
*/
extern struct VitaBaseArrayType *vt_bat_new(void);

/** Frees the VitaBaseArrayType instance
    @param bct VitaBaseArrayType pointer
*/
extern void vt_bat_free(struct VitaBaseArrayType *bct);

/** Returns VitaBaseArrayType's ptr head
    @param bct VitaBaseArrayType ptr instance
    @returns `NULL` upon failure
*/
extern void *vt_bat_head(const struct VitaBaseArrayType *const bct);

/** Returns VitaBaseArrayType's length    
    @param bct VitaBaseArrayType ptr
    @returns length
*/
extern size_t vt_bat_len(const struct VitaBaseArrayType *const bct);

/** Returns VitaBaseArrayType's capacity
    @param bct VitaBaseArrayType ptr
    @returns capacity
*/
extern size_t vt_bat_capacity(const struct VitaBaseArrayType *const bct);

/** Returns available space before new allocation is required
    @param bct VitaBaseArrayType ptr
    @returns free space (capacity - length)
*/
extern size_t vt_bat_has_space(const struct VitaBaseArrayType *const bct);

/** Returns VitaBaseArrayType's element size
    @param bct VitaBaseArrayType ptr
    @returns element size
*/
extern size_t vt_bat_elsize(const struct VitaBaseArrayType *const bct);

/** Slides through the container elements one by one
    @param bct VitaBaseArrayType ptr
    @returns container ptr head pointing to next element from the start

    @note returns `NULL` upon reaching the end
*/
extern void *vt_bat_slide_front(struct VitaBaseArrayType *const bct);

/** Slides through the container elements one by one
    @param bct VitaBaseArrayType ptr
    @returns container ptr head pointing to next element from the end

    @note returns `NULL` upon reaching the end
*/
extern void *vt_bat_slide_back(struct VitaBaseArrayType *const bct);

/** Resets the slider
    @param bct VitaBaseArrayType ptr
*/
extern void vt_bat_slide_reset(struct VitaBaseArrayType *const bct);

/** Maps a 2d index to 1d index
    @param row row index
    @param col col index
    @param ncols number of columns (horizontal width)
    @returns size_t 1d index
*/
extern size_t vt_index_2d_to_1d(const size_t row, const size_t col, const size_t ncols);

/** Maps a 1d index to 2d index
    @param row row index to save the value
    @param col col index to save the value
    @param idx 1d index to convert to 2d
    @param ncols number of columns (horizontal width)
*/
extern void vt_index_1d_to_2d(size_t *const row, size_t *const col, const size_t idx, const size_t ncols);

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
extern void *vt_malloc(const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Allocates memory and initiazes to zero
    @param bytes amount to allocate
    @param file `__SOURCE_FILENAME__`
    @param func `__func__`
    @param line `__LINE__`

    @returns ptr to allocated memory

    @note exits upon failure
*/
extern void *vt_calloc(const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Reallocates memory
    @param ptr pointer to memory address
    @param bytes amount to allocate
    @param file `__SOURCE_FILENAME__`
    @param func `__func__`
    @param line `__LINE__`

    @returns ptr to allocated memory

    @note exits upon failure
*/
extern void *vt_realloc(void *ptr, const size_t bytes, const char *const file, const char *const func, const int32_t line);

/** Frees memory
    @param bytes amount to allocate
    @returns ptr to allocated memory

    @note returns upon ptr being NULL
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
extern const char *vt_get_vita_error_str(const enum VitaError e);

#endif // VITA_CORE_H
