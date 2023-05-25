#include "vita/allocator/mallocator.h"

// void *vt_mallocator_alloc(const size_t bytes) {
//     // allocate
//     void *ptr = VT_CALLOC(bytes);

//     // update stats
//     vt_mallocator.stats.count_allocs++;

//     return ptr;
// }

// void *vt_mallocator_realloc(void *ptr, const size_t bytes) {
//     // reallocate
//     void *new_ptr = VT_REALLOC(ptr, bytes);

//     // update stats
//     vt_mallocator.stats.count_reallocs++;

//     return new_ptr;
// }

// void vt_mallocator_free(void *ptr) {
//     VT_FREE(ptr);
//     vt_mallocator.stats.count_frees++;
// }

// void vt_mallocator_print_stats(void) {
//     // create formatter
//     const char *const zfmt = 
//         "| MALLOCATOR STATS "
//         "|------------------"
//         "|   allocs: %zu\n"
//         "| reallocs: %zu\n"
//         "|    frees: %zu\n";

//     // print data to buffer
//     printf(
//         zfmt,
//         vt_mallocator.stats.count_allocs,
//         vt_mallocator.stats.count_reallocs,
//         vt_mallocator.stats.count_frees
//     );
// }


