#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/vita.h"

void print_arr2d(int** array, int r, int c);

int main(void) {
    // -------- Option 1 --------
    memhandler_pt mh = memhandler_create();

    int** array = (int**)memhandler_calloc_2d(mh, 5, 7, sizeof(int));
    print_arr2d(array, 5, 7);

    bool result = memhandler_realloc_2d(mh, (void***)&array, 2, 2, sizeof(int));
    printf("%d\n", result);
    print_arr2d(array, 2, 2);

    //memhandler_free_2d(mh, array);

    memhandler_destroy(mh);

    // -------- Option 2 --------
    memhandler_internal_create();
    memhandler_pt mhi = memhandler_internal();

    int** array2 = (int**)memhandler_calloc_2d(mhi, 5, 7, sizeof(int));
    print_arr2d(array2, 5, 7);

    bool result2 = memhandler_realloc_2d(mhi, (void***)&array2, 2, 2, sizeof(int));
    printf("%d\n", result2);
    print_arr2d(array2, 2, 2);

    memhandler_internal_destroy();

    return 0;
}


void print_arr2d(int** array, int r, int c) {
    for(size_t i = 0; i < r; i++) {
        for(size_t j = 0; j < c; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}









