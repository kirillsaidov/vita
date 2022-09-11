#ifndef VITA_CORE_H
#define VITA_CORE_H

/** VITA_CORE MODULE
 * Macros:
    - STRINGOF
    - AS

 * Functions:
    - bct_head
    - bct_len
    - bct_capacity
    - bct_rows
    - bct_cols
    - bct_elsize
    - gswap
    - get_current_timestamp
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
#define DEFAULT_INIT_ELEMENTS 10
#define CONTAINER_GROWTH_RATE 2

#define i_PCAT_NX(x, y) x ## y        // preprocessor concatenation
#define i_STR_EXPAND(x) #x            // expands macros to its value

#define PCAT(x, y) i_PCAT_NX(x, y)    // preprocessor concatenation
#define STRINGOF(x) i_STR_EXPAND(x)   // converts to string
#define AS(type, x) ((type)(x))       // cast

// data types for internal usage
enum DataType {
    // system specific
    dt_byte,    // will be treated the same as dt_int8
    dt_ubyte,   // will be treated the same as dt_uint8
    dt_short,   // will be treated the same as dt_int16
    dt_ushort,  // will be treated the same as dt_uint16
    dt_int,     // will be treated the same as dt_int32
    dt_long,    // will be treated the same as dt_int64
    dt_uint,    // will be treated the same as dt_uint32
    dt_ulong,   // will be treated the same as dt_uint64

    // fixed size
    dt_int8,    // int8_t
    dt_uint8,   // uint8_t
    dt_int16,   // int16_t
    dt_uint16,  // uint16_t
    dt_int32,   // int32_t
    dt_uint32,  // uint32_t
    dt_int64,   // int64_t
    dt_uint64,  // uint64_t

    // floats
    dt_float,   // float
    dt_double,  // double

    // other
    dt_bool,    // bool
    dt_char,    // char
    dt_str,     // str_t
    dt_cstr,    // char*
    dt_unknown, // unknown data type
    dt_count    // number of elements
};

// removing elements from array
enum RemoveStrategy {
    rs_stable,  // keep ordering
    rs_fast,    // ordering doesn't matter
    rs_count    // number of elements
};

// define vita errors
#define i_GENERATE_VITA_ERRORS(apply) \
    apply(ve_error_is_null)                     /* element wasn't initialized or is NULL */ \
    apply(ve_error_allocation)                  /* failed to allocate or reallocate memory */ \
    apply(ve_error_incompatible_datatype)       /* working with different datatypes */ \
    apply(ve_error_out_of_bounds_access)        /* accessing memory beyond allocated size */ \
    apply(ve_error_invalid_size)                /* invalid container element size */ \
    apply(ve_operation_failure)                 /* failed to perform an action */ \
    apply(ve_operation_success)                 /* all good */ \
    apply(ve_operation_element_not_found)       /* element was not found */ \
    apply(ve_count)                             /* number of elements */

// generate vita errors enum
#define X(a) a,
enum VitaError {
    i_GENERATE_VITA_ERRORS(X)
};
#undef X

// base container type for all primitives
struct BaseContainerType {
    union {
        void *ptr;
        void **ptr2;
    };

    union {
        struct {
            size_t len;
            size_t capacity;
        };

        struct {
            size_t rows;
            size_t cols;
        };
    };

    size_t elsize;
};

/* ------------- BASE CONTAINER TYPE ------------- */

/** Returns BaseContainerType's ptr head
    @param bct BaseContainerType ptr instance
    @returns `NULL` upon failure
*/
extern void *bct_head(const struct BaseContainerType *const bct);

/** Returns BaseContainerType's length
    @param bct BaseContainerType ptr
    @returns length
*/
extern size_t bct_len(const struct BaseContainerType *const bct);

/** Returns BaseContainerType's capacity
    @param bct BaseContainerType ptr
    @returns capacity
*/
extern size_t bct_capacity(const struct BaseContainerType *const bct);

/** Returns BaseContainerType's rows
    @param bct BaseContainerType ptr
    @returns rows
*/
extern size_t bct_rows(const struct BaseContainerType *const bct);

/** Returns BaseContainerType's cols
    @param bct BaseContainerType ptr
    @returns cols
*/
extern size_t bct_cols(const struct BaseContainerType *const bct);

/** Returns BaseContainerType's element size
    @param bct BaseContainerType ptr
    @returns element size
*/
extern size_t bct_elsize(const struct BaseContainerType *const bct);

/** Maps a 2d index to 1d index
    @param row row index
    @param col col index
    @param ncols number of columns (horizontal width)
    @returns size_t 1d index
*/
extern size_t index_2d_to_1d(const size_t row, const size_t col, const size_t ncols);

/** Maps a 1d index to 2d index
    @param row row index to save the value
    @param col col index to save the value
    @param idx 1d index to convert to 2d
    @param ncols number of columns (horizontal width)
*/
extern void index_1d_to_2d(size_t *const row, size_t *const col, const size_t idx, const size_t ncols);

/* ------------- OTHER FUNCTIONALITY ------------- */

/** Generic swap
    @param a first value
    @param b second value
    @param elsize element size

    @returns `true` upon success
*/
extern bool gswap(void* a, void* b, const size_t elsize);

/** Sets a timestamp "year-month-day hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data with len 21 chars
*/
extern void get_current_timestamp(char *timebuf, const size_t len);

/** Returns a vita error string from vita error code
    @param e vita error code
    @returns c string upon success, `NULL` otherwise
*/
extern const char *get_vita_error_str(const enum VitaError e);

#endif // VITA_CORE_H
