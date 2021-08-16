#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

void print_array_info(const array_pt arr);
void print_array(const array_pt arr);
void display(void* ptr);

int main(void) {
    array_memhandler_internal_create();	// semi-auto memory management
	// array_manual_collect(true); 			// manual memory management

    // --- CREATION AND ALLOCATION
    array_pt myarr = array_new(3, sizeof(int));
    array_reserve(myarr, 3);



    // --- DATA MANIPULATION
	int a = 14; array_push(myarr, &a);
    a = 9; array_push(myarr, &a);
	marray_push(myarr, 1, int);	// using a macro
	marray_push(myarr, 5, int);	// using a macro
		print_array_info(myarr);
    	print_array(myarr);



    // --- INSERTING VALUES
    // inserting when index < arr->len && arr->len < arr->capacity
    int val = 162; array_insert(myarr, 1, &val); 	printf("Inserting:\n");
    	print_array_info(myarr);
    	print_array(myarr);

    array_shrink(myarr);							printf("Shrinking:\n");
		print_array_info(myarr);
    	print_array(myarr);

    // inserting when index < arr->len && arr->len == arr->capacity
    val = 101; array_insert(myarr, 3, &val);		printf("Inserting:\n");
    	print_array_info(myarr);
    	print_array(myarr);

    // inserting when index > arr->len && arr->len < arr->capacity
    val = 555; array_insert(myarr, 8, &val);		printf("Inserting:\n");
    	print_array_info(myarr);
    	print_array(myarr);

    // inserting when index > arr->capacity
    marray_insert(myarr, 15, 1000, int);			printf("Inserting:\n");
    	print_array_info(myarr);
    	print_array(myarr);



    // --- DUPLICATION
    array_pt duparr = array_dup(myarr);				printf("Duplicating:\n");
		print_array_info(duparr);
		print_array(duparr);
	


	// --- ARRAY RESIZING
	array_resize(duparr, 5);						printf("Resizing:\n");
		print_array_info(duparr);
		print_array(duparr);
	


	// --- REMOVING AN ELEMENT
	array_remove(duparr, 3);						printf("Removing an element:\n");
		print_array_info(duparr);
		print_array(duparr);
	


	// --- SET VALUE
	int n = 333; array_set(duparr, 3, &n);			printf("Set value:\n");
	marray_set(duparr, 2, 444, int); // using a macro
		print_array_info(duparr); 
		print_array(duparr);
	


	// --- GET VALUE
	printf("Get value:\n");
	printf("duparr[2]: %d\n", marray_get(duparr, 2, int)); // using a macro
		print_array_info(duparr);
		print_array(duparr);
	


	// --- FIND VALUE
	int findValue = 333;
	printf("Looking for 333 in array, index = %lld\n", array_find(duparr, &findValue, NULL));
	findValue = 54;
	printf("Looking for 54 in array, index = %lld\n\n", array_find(duparr, &findValue, NULL));
	
	printf("array foreach\n");
	array_foreach(duparr, display);



	// --- CLEAR
	array_clear(duparr);							printf("Clear:\n");
		print_array_info(duparr);
		print_array(duparr);
		
	// array_free(myarr);
	// array_free(duparr);
		
    array_memhandler_internal_destroy();
    return 0;
}

void print_array_info(const array_pt arr) {
    printf("Length:  \t%zu\n", array_len(arr));
    printf("Capacity:\t%zu\n", array_capacity(arr));
    printf("Space:   \t%zu\n", array_has_space(arr));
}

void print_array(const array_pt arr) {
    for(size_t i = 0; i < array_len(arr); ++i) {
        printf("%d ", *as_int(array_get(arr, i)));
    }
    printf("\n\n");
}

void display(void* ptr) {
    printf("%d ", *(int*)ptr);
}













