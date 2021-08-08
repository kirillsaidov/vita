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
    
    void* ptr = &arr[4];
    printf("Value arr[4]: %d\n", to_int(ptr));

    return 0;
}













