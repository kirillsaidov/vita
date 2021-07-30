#ifndef ALGEBRA_MATRIX_H
#define ALGEBRA_MATRIX_H

/* NUMERI_MATRIX MODULE
    - algebra_matrix_create
    - algebra_matrix_get_rows
    - algebra_matrix_get_cols
    - algebra_matrix_get_size
    - algebra_matrix_get_value
    - algebra_matrix_get_ptr
    - algebra_matrix_destroy
	- algebra_matrix_resize
	- algebra_matrix_fill
	- algebra_matrix_make_identity
	- algebra_matrix_transpose
	- algebra_matrix_cofactor
	- algebra_matrix_det
	- algebra_matrix_set_row
	- algebra_matrix_set_col
	- algebra_matrix_copy
	- algebra_matrix_copy_from
	- algebra_matrix_copy_row
	- algebra_matrix_copy_col
	- algebra_matrix_add
	- algebra_matrix_multiply
    - algebra_matrix_canbe_added
    - algebra_matrix_canbe_multiplied
	- algebra_matrix_is_singular
	- algebra_matrix_is_invertible
*/

// std includes
#include <stdbool.h>
#include <stdio.h>

// vita includes
#include "../vita/logger.h"
#include "../vita/memory/handler.h"

typedef struct Matrix Matrix;

// for keeping track of all matrix allocations that were made
// think about it or add.

/* algebra_matrix_create ==> creates a new matrix, fills with 0 (uses memory_calloc)
	params:
		const size_t rows
		const size_t cols
    returns:
        Matrix*
*/
extern Matrix* algebra_matrix_create(const size_t rows, const size_t cols);

/* algebra_matrix_rows ==> returns matrix rows
    params:
        const Matrix* matrix;
    returns:
        size_t
*/
extern size_t algebra_matrix_get_rows(const Matrix* matrix);

/* algebra_matrix_cols ==> returns matrix cols
    params:
        const Matrix* matrix;
    returns:
        size_t
*/
extern size_t algebra_matrix_get_cols(const Matrix* matrix);

/* algebra_matrix_size ==> returns matrix total number of elements
    params:
        const Matrix* matrix;
    returns:
        size_t (rows*cols)
*/
extern size_t algebra_matrix_get_size(const Matrix* matrix);

/* algebra_matrix_get_value ==> returns matrix value given index [row][col]
    params:
        const Matrix* matrix;
        const size_t row
        const size_t col
    returns:
        double
    notes:
        checks for array bounds
        in case of out-of-bounds access, returns the last element row-wise or col-wise or both based off the array index
*/
extern double algebra_matrix_get_value(const Matrix* matrix, const size_t row, const size_t col);

/* algebra_matrix_size ==> returns matrix ptr
    params:
        const Matrix* matrix;
    returns:
        const double**
    notes:
        if matrix is NULL, returns NULL
*/
extern const double** algebra_matrix_get_ptr(const Matrix* matrix);

/* algebra_matrix_destroy ==> frees matrix memory and resets the matrix for reuse
	params:
		Matrix* matrix;
*/
extern void algebra_matrix_destroy(Matrix* matrix);

/* algebra_matrix_resize
	params:
		Matrix* matrix;
		const size_t rows
		const size_t cols
*/
extern void algebra_matrix_resize(Matrix* matrix, const size_t rows, const size_t cols);

/* algebra_matrix_fill
	params:
		Matrix* matrix;
		const double value
*/
extern void algebra_matrix_fill(Matrix* matrix, const double value);

/* algebra_matrix_make_identity ==> converts matrix to an identity matrix
	params:
		Matrix* matrix;
*/
extern void algebra_matrix_make_identity(Matrix* matrix);

/* algebra_matrix_transpose ==> transposes the matrix
	params:
		Matrix* matrix;
*/
extern void algebra_matrix_transpose(Matrix* matrix);

/* algebra_matrix_cofactor ==> calculates matrix cofactor
	params:
		const Matrix* matrix;
		const size_t removeRow
		const size_t removeCol
	returns:
		Matrix*
*/
extern Matrix* algebra_matrix_cofactor(const Matrix* matrix, const size_t removeRow, const size_t removeCol);

/* algebra_matrix_det ==> calculates matrix determinant
	params:
		const Matrix* matrix;
	returns:
		matrix determinant
*/
extern double algebra_matrix_det(const Matrix* matrix);

/* algebra_matrix_set_row ==> assign row values
	params:
		Matrix* matrix;
		const double[] values
		const size_t length
*/
extern void algebra_matrix_set_row(Matrix* matrix, const double values[], const size_t length);

/* algebra_matrix_set_col ==> assign column values
	params:
		Matrix* matrix;
		const double[] values
		const size_t length
*/
extern void algebra_matrix_set_col(Matrix* matrix, const double values[], const size_t length);

/* algebra_matrix_copy ==> makes a copy of an existing matrix and returns that copy
	params:
		const Matrix* matrix;
	returns:
		Matrix* (a newly allocated copy of the original matrix)
*/
extern Matrix* algebra_matrix_copy(const Matrix* matrix);

/* algebra_matrix_copy_from ==> makes a copy of an existing matrix grabbing only the requested size
	params:
		const Matrix* matrix;
		const size_t rows
		const size_t cols
	returns:
		Matrix* (a newly allocated custom-copied copy of the original matrix)
	notes:
		if the requested size exceeds matrix size, grabs the entire matrix;
		requested.rows > matrix.rows ==> matrix.rows
		requested.cols > matrix.cols ==> matrix.cols
*/
extern Matrix* algebra_matrix_copy_from(const Matrix* matrix, const size_t rows, const size_t cols);

/* algebra_matrix_copy_row ==> makes a copy of a row of an existing matrix
	params:
		const Matrix* matrix;
		const size_t row
	returns:
		Matrix* (a newly allocated matrix [1xn])
	notes:
		if the requested.row > matrix.row, returns NULL
*/
extern Matrix* algebra_matrix_copy_row(const Matrix* matrix, const size_t row);

/* algebra_matrix_copy_col ==> makes a copy of a column of an existing matrix
	params:
		const Matrix* matrix;
		const size_t col
	returns:
		Matrix* (a newly allocated matrix [nx1])
	notes:
		if the requested.col > matrix.col, returns NULL
*/
extern Matrix* algebra_matrix_copy_col(const Matrix* matrix, const size_t row);

/* algebra_matrix_add ==> adds matrixA to matrixB
	params:
		const Matrix* matrixA;
		Matrix* matrixB;
*/
extern void algebra_matrix_add(Matrix* matrixA, const Matrix* matrixB);

/* algebra_matrix_multiply ==> multiplies matrixA with matrixB
	params:
		const Matrix* matrixA;
		Matrix* matrixB;
*/
extern void algebra_matrix_multiply(Matrix* matrixA, const Matrix* matrixB);

/* algebra_matrix_canbe_added ==> checks if matrixB can be added to matrixA
	params:
		const Matrix* matrixA;
		const Matrix* matrixB;
	returns:
		true if matrixB can be added to MatrixA (false otherwise)
*/
extern bool algebra_matrix_canbe_added(const Matrix* matrixA, const Matrix* matrixB);

/* algebra_matrix_canbe_multiplied ==> checks if matrixA can be multiplied by matrixB
    params:
        const Matrix* matrixA;
        const Matrix* matrixB;
    returns:
        true if matrixA can be multiplied to MatrixB (false otherwise)
*/
extern bool algebra_matrix_canbe_multiplied(const Matrix* matrixA, const Matrix* matrixB);


/* algebra_matrix_is_singular ==> checks if a matrix is singular
	params:
		const Matrix* matrix;
	returns:
		true if matrix is singular (false otherwise)
*/
extern bool algebra_matrix_is_singular(const Matrix* matrix);

/* algebra_matrix_is_invertible ==> checks if a matrix is invertible
	params:
		const Matrix* matrix;
	returns:
		true if matrix is invertible (false otherwise)
*/
extern bool algebra_matrix_is_invertible(const Matrix* matrix);

#endif // ALGEBRA_MATRIX_H

















