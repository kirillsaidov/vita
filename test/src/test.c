#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

int main(void) {
    char str[] = "hello, world";
    char str2[10];

    //strcpy(str, "It's me"); // UB

    memcpy(str2, str, 5);
    str2[5] = 'A';
    printf("%s\n", str2);

    return 0;
}






