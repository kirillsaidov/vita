#ifndef VITA_MAT_H
#define VITA_MAT_H

/** VITA_MAT MODULE (dynamic 2D array or a pointer list)
    - mat_new
    - mat_ctor
    - mat_dup
    - mat_dtor
    - mat_free
    - mat_create
    - mat_destroy
    - mat_rows
    - mat_cols
    - mat_size
    - mat_clear
    - mat_resize
    - mat_set
    - mat_get
    - mat_apply
    
    new:
    - mat_add
    - mat_mult
    - mat_scale
    - mat_transpose
    - mat_inverse
    - mat_round
    - mat_elmax
    - mat_elmin
    - mat_sum
*/

#include "../core/core.h"
#include "../util/debug.h"
#include "../container/vec.h"

// see core/core.h for definition
typedef struct VitaBaseContainerType mat_t;

/** Allocates memory for mat_t
    @returns `mat_t*` upon success, `NULL` otherwise
*/
extern mat_t *mat_new(void);

/** Creates a new dynamic zero-initialized array of length of 0 and capacity of n
    @param m mat_t instance
    @param rows number of rows
    @param cols number of cols
    @param elsize element size

    @returns enum VitaError
*/
extern enum VitaError mat_ctor(mat_t *const m, const size_t rows, const size_t cols, const size_t elsize);

/** Duplicates and returns a new dynamic 2D array
    @param m mat_t instance
    @returns mat_t* instance upon success, `NULL` otherwise
*/
extern mat_t *mat_dup(const mat_t *const m);

/** Destroys contents of mat_t
    @param m mat_t pointer
*/
extern void mat_dtor(mat_t *const m);

/** Frees the mat_t instance
    @param m mat_t pointer
*/
extern void mat_free(mat_t *m);

/** Allocates and constructs mat_t
    @param rows number of rows
    @param cols number of cols
    @param elsize element size

    @returns `mat_t*` upon success, `NULL` otherwise
*/
extern mat_t *mat_create(const size_t rows, const size_t cols, const size_t elsize);

/** Deallocates and destroys mat_t contents
    @param m mat_t pointer
*/
extern void mat_destroy(mat_t *m);

/** Allocates and constructs mat_t from an array
    @param ptr array 
    @param rows number of rows
    @param cols number of cols
    @param elsize element size

    @returns `mat_t*` upon success, `NULL` otherwise

    @note 
        If ptr == NULL, returns an empty `mat_t` instance
*/
extern mat_t *mat_from(const void *const ptr, const size_t rows, const size_t cols, const size_t elsize);
extern mat_t *mat_fromi8(const int8_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromu8(const uint8_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromi16(const int16_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromu16(const uint16_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromi32(const int32_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromu32(const uint32_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromi64(const int64_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromu64(const uint64_t *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromf(const float *const ptr, const size_t rows, const size_t cols);
extern mat_t *mat_fromd(const double *const ptr, const size_t rows, const size_t cols);

/** Returns mat_t rows, cols, number of elements (size)
    @param m mat_t instance
    @returns rows/cols/size
*/
extern size_t mat_rows(const mat_t *const m);
extern size_t mat_cols(const mat_t *const m);
extern size_t mat_size(const mat_t *const m);

/** Clears the mat_t (memset to 0)
    @param m mat_t instance
    @returns `true` upon success
*/
extern enum VitaError mat_clear(mat_t *const m);

/** Resizes mat_t rows and cols
    @param m mat_t instance
    @param rows number of rows
    @param cols number of cols

    @returns `true` upon success
*/
extern enum VitaError mat_resize(mat_t *const m, const size_t rows, const size_t cols);

/** Assigns a new value at an index
    @param m mat_t instance
    @param val value
    @param atRow row index
    @param atCol col index

    @returns `true` upon success
*/
extern enum VitaError mat_set(mat_t *const m, const void *val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_seti8(mat_t *const m, const int8_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setu8(mat_t *const m, const uint8_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_seti16(mat_t *const m, const int16_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setu16(mat_t *const m, const uint16_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_seti32(mat_t *const m, const int32_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setu32(mat_t *const m, const uint32_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_seti64(mat_t *const m, const int64_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setu64(mat_t *const m, const uint64_t val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setf(mat_t *const m, const float val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setd(mat_t *const m, const double val, const size_t atRow, const size_t atCol);

/** Returns value at index
    @param m mat_t instance
    @param atRow row index
    @param atCol col index

    @returns value (depends on data type)
*/
extern void *mat_get(const mat_t *const m, const size_t atRow, const size_t atCol);
extern int8_t mat_geti8(const mat_t *const m, const size_t atRow, const size_t atCol);
extern uint8_t mat_getu8(const mat_t *const m, const size_t atRow, const size_t atCol);
extern int16_t mat_geti16(const mat_t *const m, const size_t atRow, const size_t atCol);
extern uint16_t mat_getu16(const mat_t *const m, const size_t atRow, const size_t atCol);
extern int32_t mat_geti32(const mat_t *const m, const size_t atRow, const size_t atCol);
extern uint32_t mat_getu32(const mat_t *const m, const size_t atRow, const size_t atCol);
extern int64_t mat_geti64(const mat_t *const m, const size_t atRow, const size_t atCol);
extern uint64_t mat_getu64(const mat_t *const m, const size_t atRow, const size_t atCol);
extern float mat_getf(const mat_t *const m, const size_t atRow, const size_t atCol);
extern double mat_getd(const mat_t *const m, const size_t atRow, const size_t atCol);

/** Calls the specified function on each element
    @param m mat_t instance
    @param func function to execute action on each element: func(pointer, row index, col index)
*/
extern void mat_apply(const mat_t *const m, void (*func)(void*, size_t, size_t));

// extern enum VitaError mat_add(mat_t *const m1, const mat_t *const m2);
// extern enum VitaError mat_mult(mat_t *const m1, const mat_t *const m2);
// extern enum VitaError mat_scale(mat_t *const m, const double value, const int8_t axis);
// extern enum VitaError mat_transpose(void);
// extern enum VitaError mat_inverse(void);
// extern enum VitaError mat_round(const mat_t *const m, const int8_t axis);
// extern vec_t *mat_elmax(const mat_t *const m, const int8_t axis);
// extern vec_t *mat_elmin(const mat_t *const m, const int8_t axis);
// extern double mat_sum(const mat_t *const m, const int8_t axis);

#endif // VITA_MAT_H
