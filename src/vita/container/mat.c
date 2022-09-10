#include "vita/container/mat.h"

mat_t *mat_new(void) {
    mat_t *m = DEBUG_CALLOC(sizeof(mat_t));
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "Failed to allocate mat_t instance!");
        return NULL;
    }

    return m;
}

enum VitaError mat_ctor(mat_t *const m, const size_t rows, const size_t cols, const size_t elsize) {
    if(m == NULL) {
        DEBUG_PRINT("%s", "mat_t instance was not allocated!");
        return ve_error_is_null;
    }

    if(!rows || !cols) {
        DEBUG_PRINT("%s", "invalid mat_t size supplied (rows, cols) = (%zu, %zu)!", rows, cols);
        return ve_error_invalid_size;
    }

    // mat_t init
    *m = (mat_t) {
        .ptr = DEBUG_CALLOC(rows * cols * sizeof(char*)),
        .rows = rows,
        .cols = cols,
        .elsize = elsize,
    };

    // error checking
    if(m->ptr == NULL) {
        DEBUG_PRINT("%s", "Unable to construct mat_t instance!");
        return ve_error_allocation;
    }

    return ve_operation_success;
}

mat_t *mat_dup(const mat_t *const m) {
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        return NULL;
    }

    // allocate a new mat_t instance
    mat_t *mdup = mat_new();
    if(mdup == NULL) {
        DEBUG_PRINT("%s", "Failed to allocate memory for mat_t before copy!");
        return NULL;
    }

    // construct mat_t instance
    if(mat_ctor(mdup, m->rows, m->cols, m->elsize) != ve_operation_success) {
        mat_free(mdup);
        return NULL;
    }

    // copy values
    memcpy(mdup->ptr, m->ptr, m->rows * m->cols * m->elsize);

    return mdup;
}

void mat_dtor(mat_t *const m) {
    // if NULL, exit
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        return;
    }

    // free mat_t contents
    DEBUG_FREE(m->ptr);

    // default-init
    *m = (mat_t) {0};
}

void mat_free(mat_t *m) {
    if(m == NULL) {
        return;
    }

    DEBUG_FREE(m);
}

mat_t *mat_create(const size_t rows, const size_t cols, const size_t elsize) {
    mat_t *m = mat_new(); // allocate mem for mat_t
    if(m == NULL) {
        DEBUG_PRINT("%s", "Unable to allocate memory for mat_t instance!");
        return NULL;
    }

    // construct mat_t
    if(mat_ctor(m, rows, cols, elsize) != ve_operation_success) {
        DEBUG_ASSERT(0, "Failed to construct mat_t instance!");

        mat_free(m);
        return NULL;
    }

    return m;
}

void mat_destroy(mat_t *m) {
    mat_dtor(m);
    mat_free(m);
}

mat_t *mat_from(const void *const ptr, const size_t rows, const size_t cols, const size_t elsize) {
    mat_t *m = mat_create(rows, cols, elsize);
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "Failed to create mat_t instance!");
        return NULL;
    }

    // if nothing to copy return an empty mat_t instance
    if(ptr == NULL) {
        return m;
    }

    // copy values
    memmove(m->ptr, ptr, rows * cols * elsize);
    m->rows = rows;
    m->cols = cols;

    return m;
}

mat_t *mat_fromi32(const int32_t *const ptr, const size_t rows, const size_t cols) {
    return mat_from(ptr, rows, cols, sizeof(int32_t));
}

mat_t *mat_fromi64(const int64_t *const ptr, const size_t rows, const size_t cols) {
    return mat_from(ptr, rows, cols, sizeof(int64_t));
}

mat_t *mat_fromf(const float *const ptr, const size_t rows, const size_t cols) {
    return mat_from(ptr, rows, cols, sizeof(float));
}

mat_t *mat_fromd(const double *const ptr, const size_t rows, const size_t cols) {
    return mat_from(ptr, rows, cols, sizeof(double));
}

size_t mat_rows(const mat_t *const m) {
    DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
    return (m == NULL) ? 0 : m->rows;
}

size_t mat_cols(const mat_t *const m) {
    DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
    return (m == NULL) ? 0 : m->cols;
}

size_t mat_size(const mat_t *const m) {
    DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
    return (m == NULL) ? 0 : (m->rows * m->cols);
}

enum VitaError mat_clear(mat_t *const m) {
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        return ve_error_is_null;
    }

    // set values to 0
    memset(m->ptr, 0, m->rows * m->cols * m->elsize);

    return ve_operation_success;
}

enum VitaError mat_resize(mat_t *const m, const size_t rows, const size_t cols) {
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        return ve_error_is_null;
    }

    if(!rows || !cols) {
        DEBUG_ASSERT(rows && cols, "invalid mat_t size supplied (rows, cols) = (%zu, %zu). Must be > 0!", rows, cols);
        return ve_error_invalid_size;
    }

    if(m->rows == rows && m->cols == cols) {
        return ve_operation_success;
    }

    // allocate memory for rows*cols number of elements
    void *els = DEBUG_REALLOC(m->ptr, rows * cols * m->elsize);
    if(els == NULL) {
        DEBUG_ASSERT(els != NULL, "Unable to reallocate memory for mat_t instance before resizing!");
        return ve_error_allocation;
    }

    // update values
    m->ptr = els;
    m->rows = rows;
    m->cols = cols;

    return ve_operation_success;
}

enum VitaError mat_set(mat_t *const m, const void *val, const size_t atRow, const size_t atCol) {
    if(m == NULL || val == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        DEBUG_ASSERT(val != NULL, "value supplied is NULL!");
        return ve_error_is_null;
    }

    if(!(atRow < m->rows) || !(atCol < m->cols)) {
        DEBUG_ASSERT((atRow < m->rows) && (atCol < m->cols), "Accessing elements out of mat_t bounds at (%zu, %zu), but size is (%zu, %zu)!", m->rows, m->cols, atRow, atCol);
        return ve_error_out_of_bounds_access;
    }

    // set the value
    memcpy((char*)(m->ptr) + (atRow * m->cols + atCol) * m->elsize, val, m->elsize);

    return ve_operation_success;
}

enum VitaError mat_seti32(mat_t *const m, const int32_t val, const size_t atRow, const size_t atCol) {
    if(m->elsize != sizeof(val)) {
        DEBUG_ASSERT(m->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return mat_set(m, &val, atRow, atCol);
}

enum VitaError mat_seti64(mat_t *const m, const int64_t val, const size_t atRow, const size_t atCol) {
    if(m->elsize != sizeof(val)) {
        DEBUG_ASSERT(m->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return mat_set(m, &val, atRow, atCol);
}

enum VitaError mat_setf(mat_t *const m, const float val, const size_t atRow, const size_t atCol) {
    if(m->elsize != sizeof(val)) {
        DEBUG_ASSERT(m->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return mat_set(m, &val, atRow, atCol);
}

enum VitaError mat_setd(mat_t *const m, const double val, const size_t atRow, const size_t atCol) {
    if(m->elsize != sizeof(val)) {
        DEBUG_ASSERT(m->elsize == sizeof(val), "Incompatible data type supplied!");
        return ve_error_incompatible_datatype;
    }

    return mat_set(m, &val, atRow, atCol);
}

void *mat_get(const mat_t *const m, const size_t atRow, const size_t atCol) {
    if(m == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        return NULL;
    }

    if(!(atRow < m->rows) || !(atCol < m->cols)) {
        DEBUG_ASSERT((atRow < m->rows) && (atCol < m->cols), "Accessing elements out of mat_t bounds at (%zu, %zu), but size is (%zu, %zu)!", m->rows, m->cols, atRow, atCol);
        return NULL;
    }

    return ((char*)(m->ptr) + (atRow * m->cols + atCol) * m->elsize);
}

int32_t mat_geti32(const mat_t *const m, const size_t atRow, const size_t atCol) {
    return *(int32_t*)(mat_get(m, atRow, atCol));
}

int64_t mat_geti64(const mat_t *const m, const size_t atRow, const size_t atCol) {
    return *(int64_t*)(mat_get(m, atRow, atCol));
}

float mat_getf(const mat_t *const m, const size_t atRow, const size_t atCol) {
    return *(float*)(mat_get(m, atRow, atCol));
}

double mat_getd(const mat_t *const m, const size_t atRow, const size_t atCol) {
    return *(double*)(mat_get(m, atRow, atCol));
}

void mat_apply(const mat_t *const m, void (*func)(void*, size_t, size_t)) {
    if(m == NULL || func == NULL) {
        DEBUG_ASSERT(m != NULL, "mat_t instance was not initialized!");
        DEBUG_ASSERT(func != NULL, "func supplied is NULL!");
        return;
    }

    // call func upon each element
    for(size_t i = 0; i < m->rows; i++) {
        for(size_t j = 0; j < m->cols; j++) {
            func(mat_get(m, i, j), i, j);
        }
    }
}



