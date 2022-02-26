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
    - mat_foreach
*/

#include "../core/core.h"

// see core/core.h for definition
typedef struct BaseArrayType mat_t;

/**
Allocates memory for mat_t

Returns: `mat_t*` upon success, `NULL` otherwise
*/
extern mat_t *mat_new(void);

/**
Creates a new dynamic zero-initialized array of length of 0 and capacity of n

Params:
    m = mat_t instance
    rows = number of rows
    cols = number of cols
    elsize = element size

Returns: `mat_t*` upon success, `NULL` otherwise
*/
extern enum VitaError mat_ctor(mat_t *const m, const size_t rows, const size_t cols, const size_t elsize);

/**
Duplicates and returns a new dynamic 2D array

Params:
    m = mat_t instance

Returns: mat_t* instance upon success, `NULL` otherwise
*/
extern mat_t *mat_dup(const mat_t *const m);

/**
Destroys contents of mat_t

Params:
    m = mat_t pointer
*/
extern void mat_dtor(mat_t *const m);

/**
Frees the mat_t instance

Params:
    m = mat_t pointer
*/
extern void mat_free(mat_t *m);

/**
Allocates and constructs mat_t

Params:
    n = number of elements
    elsize = element size

Returns: `mat_t*` upon success, `NULL` otherwise
*/
extern mat_t *mat_create(const size_t rows, const size_t cols, const size_t elsize);

/**
Deallocates and destroys mat_t contents

Params:
    m = mat_t pointer
*/
extern void mat_destroy(mat_t *m);

/**
Returns mat_t rows, cols, number of elements (size)

Params:
    m = mat_t instance

Returns: rows/cols/size
*/
extern size_t mat_rows(const mat_t *const m);
extern size_t mat_cols(const mat_t *const m);
extern size_t mat_size(const mat_t *const m);

/**
Clears the mat_t (memset to 0)

Params:
    m = mat_t instance

Returns: `true` upon success
*/
extern enum VitaError mat_clear(mat_t *const m);

/**
Resizes mat_t rows and cols

Params:
    m = mat_t instance
    rows = number of rows
    cols = number of cols

Returns: `true` upon success
*/
extern enum VitaError mat_resize(mat_t *const m, const size_t rows, const size_t cols);

/**
Assigns a new value at an index

Params:
    m = mat_t instance
    val = value
    atRow = row index
    atCol = col index

Returns: `true` upon success
*/
extern enum VitaError mat_set(mat_t *const m, const void *val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_seti32(mat_t *const m, const int val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_seti64(mat_t *const m, const long val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setf(mat_t *const m, const float val, const size_t atRow, const size_t atCol);
extern enum VitaError mat_setd(mat_t *const m, const double val, const size_t atRow, const size_t atCol);

/**
Returns value at index

Params:
    m = mat_t instance
    atRow = row index
    atCol = col index

Returns: value (depends on data type)
*/
extern void* mat_get(const mat_t *const m, const size_t atRow, const size_t atCol);
extern int32_t mat_geti32(const mat_t *const m, const size_t atRow, const size_t atCol);
extern int64_t mat_geti64(const mat_t *const m, const size_t atRow, const size_t atCol);
extern float mat_getf(const mat_t *const m, const size_t atRow, const size_t atCol);
extern double mat_getd(const mat_t *const m, const size_t atRow, const size_t atCol);

/**
Calls the specified function on each element

Params:
    m = mat_t instance
    func = function to execute action on each element: func(pointer, row index, col index)
*/
extern void mat_foreach(const mat_t *const m, void (*func)(void*, size_t, size_t));


#endif // VITA_MAT_H
