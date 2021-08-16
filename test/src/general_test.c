#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

void test(void*** a, void** b);

int main(void) {
	memhandler_internal_create();

    // test(&arr, arr);

    memhandler_internal_destroy();
}

void test(void*** a, void** b) {
	printf("equal %d\n", *a == b);
}













