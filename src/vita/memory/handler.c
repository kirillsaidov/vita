#include "vita/memory/handler.h"

struct MemoryHandler {
    size_t length;
    size_t capacity;
    void** ptr;
};

// internal memory handler
static MemoryHandler* memoryHandlerInternal = NULL;

MemoryHandler* memory_handler_create(void) {
    // create a memory handler
    MemoryHandler* handler = NULL;
    handler = memory_malloc(1, sizeof(MemoryHandler));
    if(handler == NULL) {
        logger("ERROR", "unable to create memory handler!", "memory_handler_create");
        return NULL;
    }
    
    // initialize the memory handler
    handler->length = 0;
    handler->capacity = 4;
    handler->ptr = NULL;
    
    // allocate handler storage ptr memory
    handler->ptr = memory_malloc(handler->capacity, sizeof(void*));
    if(handler->ptr == NULL) {
        logger("ERROR", "unable to create memory handler storage ptr!", "memory_handler_create");
        
        // free handler
        memory_free(handler);
        handler = NULL;
        
        return NULL;
    }
    
    return handler;
}

void memory_handler_internal_create(void) {
    if(memoryHandlerInternal == NULL) {
        memoryHandlerInternal = memory_handler_create();
    }
}

MemoryHandler* memory_handler_internal(void) {
    return memoryHandlerInternal;
}

void memory_handler_add(MemoryHandler* handler, const void* ptr) {
    // if ptr is NULL, safely exit
    if(ptr == NULL) {
        logger_info("unable to add ptr(NULL) to memory handler; doing nothing...", "memory_handler_add");
        return;
    }
    
    // if handler is NULL, create a new handler
    if(handler == NULL) {
        logger_info("memory handler is NULL; creating memory handler...", "memory_handler_add");
        
        // create memory handler
        handler = memory_handler_create();
        if(handler == NULL) {
            return;
        }
    }
    
    // check memory handler length (does it require more memory), if true, reallocate
    if(handler->length >= handler->capacity) {
        handler->capacity += handler->capacity;
        memory_realloc(handler->ptr, handler->capacity, sizeof(void*));
    }
    
    // add ptr to memory handler
    handler->ptr[handler->length++] = (void*)ptr;
}

void memory_handler_add_n(MemoryHandler* handler, const size_t n, const void* ptrs[]) {
    // add ptrs
    for(size_t i = 0; i < n; i++) {
        memory_handler_add(handler, ptrs[i]);
    }
}

void memory_handler_remove(MemoryHandler* handler, const void* ptr) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("memory handler is NULL; doing nothing...", "memory_handler_remove");
        return;
    }
    
    // if ptr is NULL, safely exit
    if(ptr == NULL) {
        logger_info("unable to remove ptr(NULL) from memory handler; doing nothing...", "memory_handler_remove");
        return;
    }
    
    // find the index of the ptr stored in memory handler, remove it
    int64_t index = algorithm_search_linear_ptr(ptr, (const void**)handler->ptr, handler->length);
    if(index == -1) {
        logger_info("ptr wasnt found in memory handler; exiting...", "memory_handler_remove");
    } else {
        handler->ptr[index] = handler->ptr[handler->length-1];
        handler->ptr[handler->length--] = NULL;
    }
}

void memory_handler_update(MemoryHandler* handler, const void* oldPtr, const void* newPtr) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("memory handler is NULL; doing nothing...", "memory_handler_update");
        return;
    }
    
    // if ptrOld is NULL, safely exit
    if(oldPtr == NULL) {
        logger_info("unable to update oldPtr => is NULL; doing nothing...", "memory_handler_update");
        return;
    }
    
    // if ptrNew is NULL, safely exit
    if(newPtr == NULL) {
        logger_info("unable to update newPtr => is NULL; doing nothing...", "memory_handler_update");
        return;
    }
    
    // find the index of the ptr stored in memory handler, remove it
    int64_t index = algorithm_search_linear_ptr(oldPtr, (const void**)handler->ptr, handler->length);
    if(index == -1) {
        logger_info("oldPtr wasnt found in memory handler; exiting...", "memory_handler_update");
    } else {
        handler->ptr[index] = (void*)newPtr;
    }
}

void* memory_handler_malloc(MemoryHandler* handler, const size_t n, const size_t size) {
    // if handler is NULL, create handler
    if(handler == NULL) {
        logger_info("memory handler is NULL; creating memory handler...", "memory_handler_malloc");
        
        // create memory handler
        handler = memory_handler_create();
        if(handler == NULL) {
            return NULL;
        }
    }
    
    // allocate memory
    void* ptr = memory_malloc(n, size);
    
    // add it to memory handler
    memory_handler_add(handler, ptr);
    
    return ptr;
}

void** memory_handler_malloc_2d(MemoryHandler* handler, const size_t rows, const size_t cols, const size_t size) {
    // if handler is NULL, create handler
    if(handler == NULL) {
        logger_info("memory handler is NULL; creating memory handler...", "memory_handler_malloc_2d");
        
        // create memory handler
        handler = memory_handler_create();
        if(handler == NULL) {
            return NULL;
        }
    }
    
    // create a 2d array
    void** ptr = memory_malloc_2d(rows, cols, size);
    
    // add it to memory handler
    memory_handler_add(handler, *ptr);
    memory_handler_add(handler, ptr);
    
    return ptr;
}

void* memory_handler_calloc(MemoryHandler* handler, const size_t n, const size_t size) {
    // if handler is NULL, create handler
    if(handler == NULL) {
        logger_info("memory handler is NULL; creating memory handler...", "memory_handler_calloc");
        
        // create memory handler
        handler = memory_handler_create();
        if(handler == NULL) {
            return NULL;
        }
    }
    
    // allocate memory
    void* ptr = memory_calloc(n, size);
    
    // add it to memory handler
    memory_handler_add(handler, ptr);
    
    return ptr;
}

void** memory_handler_calloc_2d(MemoryHandler* handler, const size_t rows, const size_t cols, const size_t size) {
    // if handler is NULL, create handler
    if(handler == NULL) {
        logger_info("memory handler is NULL; creating memory handler...", "memory_handler_calloc_2d");
        
        // create memory handler
        handler = memory_handler_create();
        if(handler == NULL) {
            return NULL;
        }
    }
    
    // create a 2d array
    void** ptr = memory_calloc_2d(rows, cols, size);
    
    // add it to memory handler
    memory_handler_add(handler, *ptr);
    memory_handler_add(handler, ptr);
    
    return ptr;
}

bool memory_handler_realloc(MemoryHandler* handler, void** ptr, const size_t n, const size_t size) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("handler is NULL; doing nothing...", "memory_handler_realloc");
        return false;
    }
    
    // remove ptr from memory handler
    memory_handler_remove(handler, *ptr);
    
    // reallocate ptr
    if(!memory_realloc(ptr, n, size)) {
        logger_info("unable to reallocate memory!", "memory_handler_realloc");
        
        // add the old memory back to memory handler
        memory_handler_add(handler, *ptr);

        return false;
    }
    
    // add it back to memory handler
    memory_handler_add(handler, *ptr);
    
    return false;
}

bool memory_handler_realloc_2d(MemoryHandler* handler, void*** ptr, const size_t rows, const size_t cols, const size_t size) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("handler is NULL; doing nothing...", "memory_handler_realloc_2d");
        return false;
    }
    
    // remove 2d array ptrs from memory handler
    memory_handler_remove(handler, **ptr);
    memory_handler_remove(handler, *ptr);
    
    // reallocate 2d array
    if(!memory_realloc_2d(ptr, rows, cols, size)) {
        logger_info("unable to reallocate memory!", "memory_handler_realloc_2d");
        
        // add the old memory back to memory handler
        memory_handler_add(handler, **ptr);
        memory_handler_add(handler, *ptr);
        
        return false;
    }
    
    // add the newly allocated 2d array back to handler
    memory_handler_add(handler, **ptr);
    memory_handler_add(handler, *ptr);
    
    return true;
}

void memory_handler_free(MemoryHandler* handler, void* ptr) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("handler is NULL; doing nothing...", "memory_handler_free");
        return;
    }
    
    // remove ptr from memory handler
    memory_handler_remove(handler, ptr);
    
    // free ptr
    memory_free(ptr);
}

void memory_handler_free_2d(MemoryHandler* handler, void** ptr) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("handler is NULL; doing nothing...", "memory_handler_realloc_2d");
        return;
    }
    
    // remove 2d array ptrs from memory handler
    memory_handler_remove(handler, *ptr);
    memory_handler_remove(handler, ptr);
    
    // free 2d array
    memory_free_2d(ptr);
}

void memory_handler_destroy(MemoryHandler* handler) {
    // check if handler is NULL, safely exit
    if(handler == NULL) {
        logger_info("handler is NULL; doing nothing...", "memory_handler_destroy");
        return;
    }
    
    // free all memory pointers
    for(size_t i = 0; i < handler->length; i++) {
        memory_free(handler->ptr[i]);
    }
    
    // free the handler storage ptr itself
    memory_free(handler->ptr);
    
    // reset to defaults
    handler->ptr = NULL;
    handler->length = 0;
    handler->capacity = 0;
    
    // free the handler itself
    memory_free(handler);
    handler = NULL;
}






















