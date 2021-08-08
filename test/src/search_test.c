#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

int main(void) {
    int arr[] = {
    	1, 2, 4, 5, 7, 9, 23, 45, 56, 345, 456, 778
    };
    
    int val = 45;
    printf("find value 55, index = %lld\n", search_binary(arr, sizeof(arr)/sizeof(int), sizeof(int), &val, NULL));

    return 0;
}













