#ifndef VITA_MALLOCATOR_H
#define VITA_MALLOCATOR_H

/** VITA_MEMORY MODULE 
 * This module is meant as a safe replacement for stdc alloc functions that can never fail. 
 * In theory, memory should never fail on your computer. And if it does fail, then there is
 * probably an issue with your device. 
    - vt_malloc
    - vt_calloc
    - vt_realloc
    - vt_free
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

#endif // VITA_MALLOCATOR_H
