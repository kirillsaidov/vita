#include "vita/memory/memory.h"

// for keeping track of all memory allocations and deallocations that were made
static size_t memoryTotalAllocationsCount = 0;
static size_t memoryTotalFreeCount = 0;

/* memory_count ==> counts number of allocations
*/
static void memory_count(const short count);

static void memory_count(const short count) {
    if(count == 0) {
        logger_info("count cannot be zero! exiting...", "memory_count");
    } else if(count > 0) {
        memoryTotalAllocationsCount += count;
    } else {
        memoryTotalFreeCount -= count;
    }
}

void* memory_malloc(const size_t n, const size_t size) {
	// memory pointer
	void* ptr = malloc(n*size);
	
	// error checking
	if(ptr == NULL) {
		logger("ERROR", "unable to allocate memory!", "memory_malloc");
	} else {
		memory_count(1);
	}
	
	return ptr;
}

void** memory_malloc_2d(const size_t rows, const size_t cols, const size_t size) {
	// allocate rows
	void** ptr = memory_malloc(rows, sizeof(void*));
	void* temp = memory_malloc(rows*cols, size);
	
	// allocate columns
	for(size_t i = 0; i < rows; i++, temp += cols*size) {
		ptr[i] = temp;
	}
	
	return ptr;
}

void* memory_calloc(const size_t n, const size_t size) {
	// memory pointer
	void* ptr = calloc(n, size);
	
	// error checking
	if(ptr == NULL) {
		logger("ERROR", "unable to allocate memory!", "memory_calloc");
	} else {
		memory_count(1);
	}
	
	return ptr;
}

void** memory_calloc_2d(const size_t rows, const size_t cols, const size_t size) {
	// allocate rows
	void** ptr = memory_calloc(rows, sizeof(void*));
	void* temp = memory_calloc(rows*cols, size);
	
	// allocate columns
	for(size_t i = 0; i < rows; i++, temp += cols*size) {
		ptr[i] = temp;
	}
	
	return ptr;
}

bool memory_realloc(void** ptr, const size_t n, const size_t size) {
	// memory pointer
	void* ptrNew = realloc(*ptr, n*size);
	
	// error checking
	if(ptrNew == NULL) {
		logger("ERROR", "unable to reallocate memory!", "memory_realloc");
		return false;
	} else {
		*ptr = ptrNew;
	}
	
	return true;
}

bool memory_realloc_2d(void*** ptr, const size_t rows, const size_t cols, const size_t size) {
	// reallocate rows
	if(!memory_realloc(*ptr, rows, sizeof(void*))) {
		logger("ERROR", "unable to reallocate memory! failed reallocating rows...", "memory_realloc_2d");
		return false;
	}
	
	// reallocate columns
	if(!memory_realloc(&(**ptr), cols*rows, size)) {
		logger("ERROR", "unable to reallocate memory! failed reallocating columns...", "memory_realloc_2d");
		return false;
	}
	
	// re-organize columns
	void* temp = **ptr;
	for(size_t i = 0; i < rows; i++, temp += cols*size) {
		(*ptr)[i] = temp;
	}
	
	return true;
}

void memory_free(void* ptr) {
	// check if ptr is NULL to avoid double frees
	if(ptr == NULL) {
		logger_info("ptr is NULL; skipping...", "memory_free");
		return;
	}
	
	// free memory
	free(ptr);
	
	// reset to NULL
	ptr = NULL;
	
	// decrement total number of allocations
	memory_count(-1);
}

void memory_free_2d(void** ptr) {
	// check if ptr is NULL to avoid double frees
	if(ptr == NULL) {
		logger_info("ptr is NULL; skipping...", "memory_free");
		return;
	}
	
	// free columns
	memory_free(*ptr);
	
	// free rows
	memory_free(ptr);
}

void memory_summary(void) {
    printf("*----------------------------\n");
    printf("* MEMORY SUMMARY [ALLOCATED]: %zu\n", memoryTotalAllocationsCount);
    printf("* \t\t     [FREED]: %zu\n", memoryTotalFreeCount);
    printf("* \t\t    [IN USE]: %zu\n", (memoryTotalAllocationsCount - memoryTotalFreeCount));
    printf("*----------------------------\n");
}

size_t memory_summary_get_allocated(void) {
    return memoryTotalAllocationsCount;
}

size_t memory_summary_get_freed(void) {
    return memoryTotalFreeCount;
}

size_t memory_summary_get_inuse(void) {
    return (memoryTotalAllocationsCount - memoryTotalFreeCount);
}






























