#ifndef VITA_ALGORITHM_SEARCH_H
#define VITA_ALGORITHM_SEARCH_H

/** SEARCH MODULE
    - //
*/

// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>

/** 
Linear search algorithm

Params:
    arr = array 
    len = array length
    elsize = array element size
    val = value to search for
    compare = compare function (pass NULL to use the memcmp comparison)

Returns: `index` if value was found, and -1 if value was not found

Notes:
    Pass NULL instead of compare function to use the memcmp comparison.
    Compare function must return a 1 on true 0 on false.
*/
// int64_t search_linear(const char *const arr, const size_t len, const size_t elsize, const void *val, int8_t (*compare)(const void *a, const void *b));

/**
Binary search algorithm

Params:
    const void* arr
    const size_t len
    const size_t elsize
    const void* val
    bool (*compare)(const void* a, const void* b)

Returns: `index` if value was found, and -1 if value was not found

Notes:
    Pass NULL instead of compare function to use the memcmp comparison.
    Compare function must return: 
         0 => if a == b
        -1 => if a < b
*/
// int64_t search_binary(void* arr, const size_t len, const size_t elsize, const void* val, int8_t (*compare)(const void* a, const void* b));

extern void search_test(void);

#endif // VITA_ALGORITHM_SEARCH_H
