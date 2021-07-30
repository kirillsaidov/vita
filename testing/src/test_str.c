#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/vita.h"

int main(void) {
    int** a = (int**)mem_calloc_2d(7, 7, sizeof(int));
    
    for (size_t i = 0; i < 7; i++) {
        for (size_t j = 0; j < 7; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    mem_realloc_2d((void ***)&a, 5, 5, sizeof(int));
    
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    mem_free_2d((void**)a);

    return 0;
}






