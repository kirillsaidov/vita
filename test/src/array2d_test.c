#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

void display(void* ptr);

int main(void) {
    array_memhandler_internal_create();	// semi-auto memory management
	// array_manual_collect(true); 			// manual memory management

    // --- CREATION AND ALLOCATION
    array2d_pt myarr = array2d_new(3, 2, sizeof(int));
    int val = 1; array2d_set(myarr, 0, 0, &val);
    val = 2; array2d_set(myarr, 0, 1, &val);
    val = 3; array2d_set(myarr, 1, 0, &val);
    val = 4; array2d_set(myarr, 1, 1, &val);
    val = 5; array2d_set(myarr, 2, 0, &val);
    val = 6; array2d_set(myarr, 2, 1, &val);

	array2d_foreach(myarr, display);
	printf("\nsize: %zu\n\n", array2d_size(myarr));



	// --- CLEAR (SET TO 0)
	array2d_clear(myarr);
	array2d_foreach(myarr, display);
	printf("\nsize: %zu\n\n", array2d_size(myarr));



	// --- RESIZE
	array2d_resize(myarr, 4, 4); // does not work!
	array2d_foreach(myarr, display);
	printf("\nsize: %zu\n\n", array2d_size(myarr));



	// --- DUPLICATION
	array2d_pt myarrdup = array2d_dup(myarr);
	val = 9; array2d_set(myarrdup, 0, 0, &val);
	val = 7; array2d_set(myarrdup, 1, 1, &val);

	array2d_foreach(myarrdup, display);
	printf("\nsize: %zu\n\n", array2d_size(myarrdup));

	for(size_t i = 0; i < array2d_rows(myarrdup); i++) {
		for(size_t j = 0; j < array2d_cols(myarrdup); j++) {
			printf("%d ", *(int*)array2d_get(myarrdup, i, j));
		}
		printf("\n");
	}



	// --- MACROS
	marray2d_set(myarrdup, 0, 0, 99, int);
	printf("[0][0] = %d\n", marray2d_get(myarrdup, 0, 0, int));



    // array2d_free(myarr);
    array_memhandler_internal_destroy();
}

void display(void* ptr) {
    printf("%d ", *(int*)ptr);
}













