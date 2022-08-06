#ifndef VITA_CORE_H
#define VITA_CORE_H

/** VITA_CORE MODULE
    - is_null
    - gswap
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define VITA_VERSION 0.2
#define DEFAULT_INIT_ELEMENTS 10
#define CONTAINER_GROWTH_RATE 2

// types for argopt
enum DataType {
    dt_int,     // integer
    dt_float,   // floating point numbers
    dt_bool,    // boolean values
    dt_char,    // chars
    dt_str,     // str_t type (its not a raw C string!)
    dt_count    // number of elements
};

// removing elements from array
enum RemoveStrategy {
    rs_stable,  // keep ordering
    rs_fast,    // ordering doesn't matter
    rs_count    // number of elements
};

// container operations errors
enum VitaError {
    ve_error_is_null,                   // element wasn't initialized or is NULL
    ve_error_allocation,                // failed to allocate or reallocate memory
    ve_error_incompatible_datatype,     // working with different datatypes
    ve_error_out_of_bounds_access,      // accessing memory beyond allocated size
    ve_operation_failure,               // failed to perform an action
    ve_operation_success,               // all good
    ve_count                            // number of elements
};

// array struct wrapper: base container type
struct BaseArrayType {
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
Returns a string for enum DataType value

Params:
    dt = enum DataType

Returns: a raw C string
*/
extern const char *const dt_to_str(const enum DataType dt);

#endif // VITA_CORE_H
