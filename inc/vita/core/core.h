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

#if defined(_WIN32) || defined(_WIN64)
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

// constants
#define VITA_VERSION "0.2"
#define DEFAULT_INIT_ELEMENTS 10
#define CONTAINER_GROWTH_RATE 2

// useful macros
#define STRINGOF(x) #x
#define AS(type, x) ((type)(x))

// types for argopt
enum DataType {
    dt_int,     // integer
    dt_float,   // floating point numbers
    dt_bool,    // boolean values
    dt_char,    // chars
    dt_str,     // str_t type (its not a raw C string!)
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
#define ___GENERATE_VITA_ERRORS(apply) \
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
    ___GENERATE_VITA_ERRORS(X)
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

/**
Returns BaseContainerType's ptr head

Params:
    bct = BaseContainerType ptr instance

Returns: `NULL` upon failure
*/
extern void *bct_head(const struct BaseContainerType *const bct);

/**
Returns BaseContainerType's length

Params:
    bct = BaseContainerType ptr

Returns: length
*/
extern size_t bct_len(const struct BaseContainerType *const bct);

/**
Returns BaseContainerType's capacity

Params:
    bct = BaseContainerType ptr

Returns: capacity
*/
extern size_t bct_capacity(const struct BaseContainerType *const bct);

/**
Returns BaseContainerType's rows

Params:
    bct = BaseContainerType ptr

Returns: rows
*/
extern size_t bct_rows(const struct BaseContainerType *const bct);

/**
Returns BaseContainerType's cols

Params:
    bct = BaseContainerType ptr

Returns: cols
*/
extern size_t bct_cols(const struct BaseContainerType *const bct);

/**
Returns BaseContainerType's element size

Params:
    bct = BaseContainerType ptr

Returns: element size
*/
extern size_t bct_elsize(const struct BaseContainerType *const bct);

/* ------------- OTHER FUNCTIONALITY ------------- */

/**
Generic swap

Params:
    a = first value
    b = second value
    elsize = element size

Returns: `true` upon success
*/
extern bool gswap(void* a, void* b, const size_t elsize);

/**
Sets a timestamp "year-month-day hour-minute-seconds" to timebuf

Params:
    timebuf = to store timestamp data with len 21 chars
*/
extern void get_current_timestamp(char *timebuf, const size_t len);

/**
Returns a vita error string from vita error code

Params:
    e = vita error code

Returns: c string upon success, `NULL` otherwise
*/
extern const char *const get_vita_error_str(const enum VitaError e);

#endif // VITA_CORE_H
