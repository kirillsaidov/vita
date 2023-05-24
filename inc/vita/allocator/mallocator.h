#ifndef VITA_MALLOCATOR_H
#define VITA_MALLOCATOR_H

/** VITA_MALLOCATOR MODULE 
    - .
*/

#include "../core/core.h"

struct VitaBaseAllocatorType {

    // 
    void *(*alloc)(size_t);
    void *(*realloc)(void*, size_t);
    void  (*free)(void*);
};

#endif // VITA_MALLOCATOR_H
