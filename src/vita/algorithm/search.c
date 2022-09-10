#include "vita/algorithm/search.h"

// int64_t search_linear(const char *const arr, const size_t len, const size_t elsize, const void *val, int8_t (*compare)(const void *a, const void *b)) {
//     if(arr == NULL || val == NULL) {
//         return -1;
//     }

//     // preparation
//     size_t index = 0;
//     const char *end = arr + len * elsize;

//     // if the element that is sought for is found, break from the loop
//     for(char *current = arr; current != end; current += elsize, index++) {
//         if(compare == NULL ? (memcmp(current, val, elsize) == 0) : (compare(current, val))) {
//             return index;
//         }
//     }

//     return -1;
// }

// int64_t search_binary(void* arr, const size_t len, const size_t elsize, const void* val, int (*compare)(const void* a, const void* b)) {
//     // variables
//     size_t start = 0;
//     size_t end = len - 1;
    
//     // binary search
//     while(start <= end) {
//         // find the middle index
//         int64_t index = start + (end - start)/2;
        
//         // check if the val we are looking for is present at index
//         int found = (
//             is_null(compare) ? 
//             memcmp((arr + index * elsize), val, elsize) : 
//             compare((arr + index * elsize), val)
//         );
//         if(found == 0) {
//             return index;
//         } else if(found < 0) { 	// if val is on the right hand side of the array, ignore the left half
//             start = index + 1;
//         } else { 				// if val is on the left hand side of the array, ignore the right half
//             end = index - 1;
//         }
//     }
    
//     return -1;
// }

void search_test(void) {}
