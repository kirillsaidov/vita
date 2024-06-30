#include <assert.h>
#include "vita/allocator/mallocator.h"

int32_t main(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    int *val = VT_ALLOCATOR_ALLOC(alloctr, sizeof(int));
    char *zbuf = VT_ALLOCATOR_ALLOC(alloctr, sizeof(char) * 100);

    VT_ALLOCATOR_FREE(alloctr, zbuf);
    zbuf = VT_ALLOCATOR_ALLOC(alloctr, 210);
    // vt_mallocator_print_stats(alloctr->stats);

    VT_ALLOCATOR_FREE(alloctr, val);
    VT_ALLOCATOR_FREE(alloctr, zbuf);

    vt_mallocator_print_stats(alloctr->stats);
    vt_mallocator_destroy(alloctr);

    return 0;
}
