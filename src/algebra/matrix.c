#include "algebra/matrix.h"

struct Matrix {
    // matrix size
    size_t rows;
    size_t cols;
    
    // 2D array pointer
    double** ptr;
};

Matrix* algebra_matrix_create(const size_t rows, const size_t cols) {
    // create internal memory handler
    memory_handler_internal_create();
    
    // allocate memory for matrix
    Matrix* matrix = memory_handler_malloc(memory_handler_internal(), 1, sizeof(Matrix));
    
    // default initialize the matrix
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->ptr = NULL;
    
    // create a 2d array
    // check for memory allocation errors
    matrix->ptr = (double**)memory_handler_calloc_2d(memory_handler_internal(), rows, cols, sizeof(double));
    if(matrix->ptr == NULL) {
        logger_info("unable to create a matrix; memory allocation error...", "algebra_matrix_create");
        
        // free matrix memory
        memory_handler_free(memory_handler_internal(), matrix);
        matrix = NULL;
    }
    
    return matrix;
}

size_t algebra_matrix_get_rows(const Matrix* matrix) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; cannot retrieve rows...", "algebra_matrix_rows");
        return 0;
    }
    
    return matrix->rows;
}

size_t algebra_matrix_get_cols(const Matrix* matrix) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; cannot retrieve cols...", "algebra_matrix_cols");
        return 0;
    }
    
    return matrix->cols;
}

size_t algebra_matrix_get_size(const Matrix* matrix) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; cannot retrieve size...", "algebra_matrix_size");
        return 0;
    }
    
    return (matrix->rows * matrix->cols);
}

double algebra_matrix_get_value(const Matrix* matrix, const size_t row, const size_t col) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; returning 0...", "algebra_matrix_get_ptr");
        return 0;
    }
    
    // check array bounds
    size_t i = ((row < matrix->rows) ? (row) : (matrix->rows - 1));
    size_t j = ((col < matrix->cols) ? (col) : (matrix->cols - 1));
    
    return matrix->ptr[i][j];
}

const double** algebra_matrix_get_ptr(const Matrix* matrix) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; returning NULL...", "algebra_matrix_get_ptr");
        return NULL;
    }
    
    return (const double**)matrix->ptr;
}

void algebra_matrix_destroy(Matrix* matrix) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; doing nothing...", "algebra_matrix_destroy");
        return;
    }
    
    // free matrix 2d array
    memory_handler_free_2d(memory_handler_internal(), (void**)matrix->ptr);
    
    // free the matrix itself
    memory_handler_free(memory_handler_internal(), matrix);
}

void algebra_matrix_resize(Matrix* matrix, const size_t rows, const size_t cols) {
    // check if matrix is NULL, safely exit
    if(matrix == NULL) {
        logger_info("matrix is NULL; doing nothing...", "algebra_matrix_resize");
        return;
    }
    
    // resizing the matrix and error checking
    if(!memory_handler_realloc_2d(memory_handler_internal(), (void***)&matrix->ptr, rows, cols, sizeof(double))) {
        logger_info("unable to resize the matrix!", "algebra_matrix_resize");
    } else {
        matrix->rows = rows;
        matrix->cols = cols;
    }
}








































