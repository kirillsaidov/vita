#ifndef VITA_CONTAINER_COMMON_H
#define VITA_CONTAINER_COMMON_H

/** COMMON MODULE
    - vt_array_head
    - vt_array_len
    - vt_array_capacity
    - vt_array_has_space
    - vt_array_elsize
    - vt_array_has_alloctr
    - vt_array_set
    - vt_array_get
    - vt_array_slide_front
    - vt_array_slide_back
    - vt_array_slide_reset
    - vt_index_2d_to_1d
    - vt_index_1d_to_2d
    - vt_index_3d_to_1d
    - vt_index_1d_to_3d        
*/

#include "../core/core.h"
#include "../util/debug.h"
#include "../allocator/mallocator.h"

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

/** Allocates memory for VitaBaseArrayType structure
    @param alloctr allocator instance
    @returns `VitaBaseArrayType*` upon success, `NULL` otherwise

    @note if `NULL` is specified, then vita calloc/realloc/free is used
*/
extern struct VitaBaseArrayType *vt_array_new(struct VitaBaseAllocatorType *const alloctr);

/** Frees the VitaBaseArrayType instance
    @param vbat VitaBaseArrayType instance
*/
extern void vt_array_free(struct VitaBaseArrayType *vbat);

/** Returns VitaBaseArrayType's ptr head
    @param vbat VitaBaseArrayType instance
    @returns `NULL` upon failure
*/
extern void *vt_array_head(const struct VitaBaseArrayType *const vbat);

/** Returns VitaBaseArrayType's length    
    @param vbat VitaBaseArrayType instance
    @returns length
*/
extern size_t vt_array_len(const struct VitaBaseArrayType *const vbat);

/** Returns VitaBaseArrayType's capacity
    @param vbat VitaBaseArrayType instance
    @returns capacity
*/
extern size_t vt_array_capacity(const struct VitaBaseArrayType *const vbat);

/** Returns available space before new allocation is required
    @param vbat VitaBaseArrayType instance
    @returns free space (capacity - length)
*/
extern size_t vt_array_has_space(const struct VitaBaseArrayType *const vbat);

/** Returns VitaBaseArrayType's element size
    @param vbat VitaBaseArrayType instance
    @returns element size
*/
extern size_t vt_array_elsize(const struct VitaBaseArrayType *const vbat);

/** Checks if VitaBaseArrayType has a valid allocator instance
    @returns ditto
*/
extern bool vt_array_has_alloctr(const struct VitaBaseArrayType *const vbat);

/** Returns value at index
    @param vbat VitaBaseArrayType instance
    @param at index
    @returns void*
*/
extern void *vt_array_get(const struct VitaBaseArrayType *const vbat, const size_t at);

/** Assigns a new value at an index
    @param vbat VitaBaseArrayType instance
    @param val value
    @param at index to set the value
*/
extern void vt_array_set(const struct VitaBaseArrayType *const vbat, const void *const val, const size_t at);

/** Slides through the container elements one by one starting from the begining
    @param vbat VitaBaseArrayType instance
    @returns container ptr head pointing to next element from the start

    @note returns `NULL` upon reaching the end
*/
extern void *vt_array_slide_front(struct VitaBaseArrayType *const vbat);

/** Slides through the container elements one by one starting from the end
    @param vbat VitaBaseArrayType instance
    @returns container ptr head pointing to next element from the end

    @note returns `NULL` upon reaching the begining
*/
extern void *vt_array_slide_back(struct VitaBaseArrayType *const vbat);

/** Resets the slider
    @param vbat VitaBaseArrayType instance
*/
extern void vt_array_slide_reset(struct VitaBaseArrayType *const vbat);

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

/** Maps a 3d index to 1d index
    @param row row index
    @param col col index
    @param depth depth index
    @param nrows number of rows (vertical height)
    @param ncols number of columns (horizontal width)
    @returns size_t 1d index
*/
extern size_t vt_index_3d_to_1d(const size_t row, const size_t col, const size_t depth, const size_t nrows, const size_t ncols);

/** Maps a 1d index to 2d index
    @param row row index to save the value
    @param col col index to save the value
    @param depth depth index to save the value
    @param idx 1d index to convert to 3d
    @param nrows number of rows (vertical height)
    @param ncols number of columns (horizontal width)
*/
extern void vt_index_1d_to_3d(size_t *const row, size_t *const col, size_t *const depth, const size_t idx, const size_t nrows, const size_t ncols);

#endif // VITA_CONTAINER_COMMON_H
