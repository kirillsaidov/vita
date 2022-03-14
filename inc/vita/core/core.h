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

#define DEFAULT_INIT_ELEMENTS 10
#define CONTAINER_GROWTH_RATE 2

// removing elements from array
enum RemoveStrategy {
    rs_stable,  // keep ordering
    rs_fast,    // ordering doesn't matter
    rs_count    // number of elements
};

// container operations errors
enum VitaError {
    ve_error_none,                      // nothing's happened yet
    ve_error_is_null,                   // element wasn't initialized or is NULL
    ve_error_allocation,                // failed to allocate or reallocate memory
    ve_error_incompatible_datatype,     // working with different datatypes
    ve_error_out_of_bounds_access,      // accessing memory beyond allocated size
    ve_operation_failure,               // failed to perform an action
    ve_operation_success,               // all good
    ve_count                            // number of elements
};

// array struct wrapper
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

#endif // VITA_CORE_H