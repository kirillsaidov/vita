#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

int main(void) {
    int i = 0;
    int val = 10;
    int size = 4;

    printf("%d\n", (val+size*++i));

    return 0;
}














