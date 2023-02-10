#ifndef VITA_CORE_H
#define VITA_CORE_H

/** VITA_CORE MODULE
 * Macros:
    - VT_PCAT
    - VT_STRINGOF
    - VT_AS

 * Functions:
    - vt_bct_head
    - vt_bct_len
    - vt_bct_capacity
    - vt_bct_elsize
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
#include <time.h>

// getting file name
#if defined(__clang__)
    #define __SOURCE_FILENAME__ __FILE_NAME__
#elif defined(__GNUC__) || defined(__GNUG__)
    #define __SOURCE_FILENAME__ __BASE_FILE__
#else
    #if defined(_WIN32) || defined(_WIN64)
        #define __SOURCE_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
    #else
        #define __SOURCE_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
    #endif
#endif

// useful macros
#define VT_DEFAULT_INIT_ELEMENTS 10
#define VT_CONTAINER_GROWTH_RATE 2

// this is needed in order to properly expand macros if one macro is inserted into another
#define VT_i_PCAT_NX(x, y) x ## y           // preprocessor concatenation
#define VT_i_STR_EXPAND(x) #x               // expands macros to its value

#define VT_PCAT(x, y) VT_i_PCAT_NX(x, y)    // preprocessor concatenation
#define VT_STRINGOF(x) VT_i_STR_EXPAND(x)   // converts to string
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
    vt_dt_str,      // str_t
    vt_dt_vec,      // vec_t
    vt_dt_plist,    // plist_t

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
struct VitaBaseContainerType {
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

/** Allocates memory for VitaBaseContainerType structure
    @returns `VitaBaseContainerType*` upon success, `NULL` otherwise
*/
extern struct VitaBaseContainerType *vt_bct_new(void);

/** Frees the VitaBaseContainerType instance
    @param bct VitaBaseContainerType pointer
*/
extern void vt_bct_free(struct VitaBaseContainerType *bct);

/** Returns BaseContainerType's ptr head
    @param bct BaseContainerType ptr instance
    @returns `NULL` upon failure
*/
extern void *vt_bct_head(const struct VitaBaseContainerType *const bct);

/** Returns BaseContainerType's length    
    @param bct BaseContainerType ptr
    @returns length
*/
extern size_t vt_bct_len(const struct VitaBaseContainerType *const bct);

/** Returns BaseContainerType's capacity
    @param bct BaseContainerType ptr
    @returns capacity
*/
extern size_t vt_bct_capacity(const struct VitaBaseContainerType *const bct);

/** Returns BaseContainerType's element size
    @param bct BaseContainerType ptr
    @returns element size
*/
extern size_t vt_bct_elsize(const struct VitaBaseContainerType *const bct);

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

/* ------------- OTHER FUNCTIONALITY ------------- */

/** Generic swap
    @param a first value
    @param b second value
    @param elsize element size

    @returns `true` upon success
*/
extern bool vt_gswap(void* a, void* b, const size_t elsize);

/** Sets a timestamp "year-month-day hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be >= 21 chars! Otherwise, it returns doing nothing.
*/
extern void vt_get_current_timestamp(char *timebuf, const size_t len);

/** Returns a vita error string from vita error code
    @param e vita error code
    @returns c string upon success, `NULL` otherwise
*/
extern const char *vt_get_vita_error_str(const enum VitaError e);

#endif // VITA_CORE_H
