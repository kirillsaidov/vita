#ifndef VITA_ARRAY2D_H
#define VITA_ARRAY2D_H

/** VITA_ARRAY2D MODULE
	- array2d
	- array2d_new
	- array2d_dup *
	- array2d_free

	- array2d_rows
	- array2d_cols
	- array2d_size

	- array2d_resize
	- array2d_set
	- array2d_get
	- array2d_clear
	- array2d_foreach
*/

#include "../core/core.h"
#include "../memory/memhandler.h"
#include "array.h"

// fancy macros for some functions to simplify work
#define marray2d_set(arr, row, col, val, type); { type x = val; array2d_set(arr, row, col, (void*)(&x)); }
#define marray2d_get(arr, row, col, type) (*(type*)(array2d_get(arr, row, col))) 

// new array2d type
// NOTE: here len and capacity are used as rows and cols
typedef struct BaseArrayType* array2d_pt; 

/** array2d ==> creates a nxn array matrix with n=DEFAULT_ALLOCATION_SIZE
	params:
		const size_t elsize (element size)
	returns:
		array2d_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		allocates nxn matrix by default (calls array2d_new)
		initializes array2d elements to zero
*/
extern array2d_pt array2d(const size_t elsize);

/** array2d_new ==> creates an mxn array matrix
	params:
		const size_t rows
		const size_t cols
		const size_t elsize (element size)
	returns:
		array2d_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure

		initializes array2d elements to zero
*/
extern array2d_pt array2d_new(const size_t rows, const size_t cols, const size_t elsize);

/** array2d_dup ==> duplicates an array2d
	params:
		const array2d_ptr arr
	returns:
		array2d_pt 
	notes:
		valid pointer 	: upon success
		NULL 			: upon failure
*/
extern array2d_pt array2d_dup(const array2d_pt arr);

/** array2d_free ==> frees the array2d and its elements
	params:
		array2d_ptr arr
*/
extern void array2d_free(array2d_pt arr);







/** array2d_row/col/size ==> returns number of rows, cols and number of elements
	params:
		array2d_ptr arr
	returns:
		size_t
*/
extern size_t array2d_rows(const array2d_pt arr);
extern size_t array2d_cols(const array2d_pt arr);
extern size_t array2d_size(const array2d_pt arr);







/** array2d_resize ==> resizes array2d to the specified length 
	params:
		array2d_pt arr
		const size_t rows
		const size_t cols
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool array2d_resize(array2d_pt arr, const size_t rows, const size_t cols);

/** array2d_set ==> assigns a new value to array2d element
	params:
		array2d_ptr arr
		const size_t row
		const size_t col
		const void* valptr
	returns:
		bool
	notes:
		true 	: upon success
		false	: upon failure
*/
extern bool array2d_set(array2d_pt arr, const size_t row, const size_t col, const void* valptr);

/** array2d_get ==> returns the pointer to array2d element
	params:
		array2d_ptr arr
		const size_t row
		const size_t col
	returns:
		void* ptr
	notes:
		valid ptr 	: upon success
		NULL		: upon failure
*/
extern void* array2d_get(const array2d_pt arr, const size_t row, const size_t col);

/** array2d_clear ==> memset everthing to zero
	params:
		array2d_ptr arr
	notes:
		all elements up to array2d capacity are set to zero
*/
extern void array2d_clear(array2d_pt arr);

/** array_foreach ==> iterates through each element and calls func on that element
	params:
		const array2d_pt arr
		void (*func)(void* ptr)
*/
extern void array2d_foreach(const array2d_pt arr, void (*func)(void* ptr));

#endif // VITA_ARRAY2D_H


