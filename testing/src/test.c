#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/vita.h"

int main(void) {
    memhandler_pt mh = memhandler_create();

    memhandler_destroy(mh);

    return 0;
}






