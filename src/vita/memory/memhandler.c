#include "vita/memory/memhandler.h"

// memory handler structure
struct Memhandler {
	void** ptr;

    size_t len;
    size_t capacity;
};

// internal memory handler
static memhandler_pt memhandlerInternal = NULL;

/** memhandler_check ==> checks if a memory handler requires more memory, if true, it allocates additional memory (twice the old size)
	params:
		memhandler_pt mh
		const void* ptr
	returns:
		bool
	notes:
		true - if additional memory isn't required or if additional allocation was successful
		false - upon failure
*/
static bool memhandler_check(memhandler_pt mh);

static bool memhandler_check(memhandler_pt mh) {
	// check if memory handler needs more memory, if true, allocate additional memory
    if(mh->len >= mh->capacity) {
        mh->capacity += mh->capacity;
        
        // allocate
        if(!mem_realloc(mh->ptr, mh->capacity, sizeof(void*))) {
        	return false;
        }
    }

	return true;
}

memhandler_pt memhandler_create(void) {
	// create a memory handler
	memhandler_pt mh = mem_malloc(1, sizeof(struct Memhandler));
	if(is_null(mh)) {
		logger_error(str("unable to create memory handler!"), str("memhandler_create"));
	} else {
		// initialize the memory handler
		*mh = (struct Memhandler) {
			.ptr = NULL,
			.len = 0,
			.capacity = 6
		};

		// allocate handler storage ptr memory
		mh->ptr = mem_malloc(mh->capacity, sizeof(void*));
		if(is_null(mh->ptr)) {
			logger_error(str("unable to create a memory handler storage pointer!"), str("memhandler_create"));

			// free handler
			mem_free(mh);
		}
	}

	return mh;
}

void memhandler_destroy(memhandler_pt mh) {
	// check if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_destroy"));
		return;
	}

	// free all memory pointers
	for(size_t i = 0; i < mh->len; i++) {
		mem_free(mh->ptr[i]);
	}

	// free the memory handler and the storage ptr itself
	mem_free(mh->ptr);
	mem_free(mh);
}

bool memhandler_internal_create(void) {
	memhandlerInternal = ((is_null(memhandlerInternal)) ? (memhandler_create()) : (memhandlerInternal));
	if(is_null(memhandlerInternal)) {
		return false;
	}

	return true;
}

void memhandler_internal_destroy(void) {
	if(!is_null(memhandlerInternal)) {
		memhandler_destroy(memhandlerInternal);
	}
}

const memhandler_pt memhandler_internal(void) {
	return memhandlerInternal;
}







bool memhandler_add(memhandler_pt mh, const void* ptr) {    
	// if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_add"));
		return false;
	}

	// check if memory handler needs more memory, if true allocate additional memory
	if(!memhandler_check(mh)) {
		logger_error(str("unable to resize memory handler; doing nothing..."), str("memhandler_add"));
		return false;
	}

	// add ptr to memory handler
	mh->ptr[mh->len++] = (void*)ptr;

	return true;
}

bool memhandler_remove(memhandler_pt mh, const void* ptr) {
	// check if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_remove"));
		return false;
	}

	if(mh->len > 1) {
		// find the index of the ptr stored in memory handler, remove it
		int64_t index = search_linear_ptr(ptr, (const void**)mh->ptr, mh->len);
		if(index == -1) {
			logger_warn(str("element does not exist in memory handler; exiting..."), str("memhandler_remove"));
			return false;
		} else {
			mh->ptr[index] = mh->ptr[mh->len-1];
			mh->ptr[mh->len--] = NULL;
		}
	} else {
		mh->ptr[--(mh->len)] = NULL;
	}

	return true;
}







void* memhandler_malloc(memhandler_pt mh, const size_t n, const size_t size) {
	// if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_malloc"));
		return NULL;
	}

	// allocate new memory
	void* ptr = mem_malloc(n, size);
	if(is_null(ptr)) {
		logger_error(str("unable to allocate new memory!"), str("memhandler_malloc"));
		return NULL;
	}

	// add it to memory handler
	if(!memhandler_add(mh, ptr)) {
		logger_error(str("unable to add new memory to memory handler!"), str("memhandler_malloc"));
		mem_free(ptr);

		return NULL;
	}

	return ptr;
}

void* memhandler_calloc(memhandler_pt mh, const size_t n, const size_t size) {
	// if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_calloc"));
		return NULL;
	}

	// allocate new memory
	void* ptr = mem_calloc(n, size);
	if(is_null(ptr)) {
		logger_error(str("unable to allocate new memory!"), str("memhandler_calloc"));
		return NULL;
	}

	// add it to memory handler
	if(!memhandler_add(mh, ptr)) {
		logger_error(str("unable to add new memory to memory handler!"), str("memhandler_calloc"));
		mem_free(ptr);
		
		return NULL;
	}

	return ptr;
}

bool memhandler_realloc(memhandler_pt mh, const void** ptr, const size_t n, const size_t size) {
	// if handler is NULL, safely exit
	if(is_null(mh) || is_null(ptr)) {
		logger_warn(str("memory handler or ptr is NULL; doing nothing..."), str("memhandler_realloc"));
		return false;
	}

	// remove ptr from memory handler
	if(!memhandler_remove(mh, (const void**)*ptr)) {
		logger_error(str("unable to remove ptr from memory handler before realloc operation!"), str("memhandler_realloc"));
		return false;
	}

	// reallocate ptr
	if(!mem_realloc((void*)ptr, n, size)) {
		logger_error(str("unable to remove ptr from memory handler before realloc operation!"), str("memhandler_realloc"));

		// add the old memory back to memory handler
		memhandler_add(mh, *ptr);
		
		return false;
	}

	// add new memory to memory handler
	memhandler_add(mh, *ptr);

	return true;
}

void memhandler_free(memhandler_pt mh, const void* ptr) {
	// if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; safely exiting..."), str("memhandler_free"));
		return;
	}

	// remove ptr from memory handler
	if(!memhandler_remove(mh, ptr)) {
		logger_error(str("unable to remove ptr from memory handler before free operation!"), str("memhandler_free"));
		return;
	}

	// free ptr
	mem_free((void*)ptr);
}







void** memhandler_malloc_2d(memhandler_pt mh, const size_t rows, const size_t cols, const size_t size) {
	// if handler is NULL, create handler
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_malloc_2d"));
		return NULL;
	}

	// create a 2d array
	void** ptr = mem_malloc_2d(rows, cols, size);
	if(is_null(ptr)) {
		logger_error(str("unable to allocate new memory!"), str("memhandler_malloc_2d"));
		return NULL;
	}

	// add ptr to memory handler
	memhandler_add(mh, *ptr);
	memhandler_add(mh, ptr);

	return ptr;
}

void** memhandler_calloc_2d(memhandler_pt mh, const size_t rows, const size_t cols, const size_t size) {
	// if handler is NULL, create handler
	if(is_null(mh)) {
		logger_warn(str("memory handler is NULL; doing nothing..."), str("memhandler_calloc_2d"));
		return NULL;
	}

	// create a 2d array
	void** ptr = mem_calloc_2d(rows, cols, size);
	if(is_null(ptr)) {
		logger_error(str("unable to allocate new memory!"), str("memhandler_calloc_2d"));
		return NULL;
	}

	// add ptr to memory handler
	memhandler_add(mh, *ptr);
	memhandler_add(mh, ptr);

	return ptr;
}

bool memhandler_realloc_2d(memhandler_pt mh, void*** ptr, const size_t rows, const size_t cols, const size_t size) {
	// check if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("handler is NULL; doing nothing..."), str("memhandler_realloc_2d"));
		return false;
	}

	// remove 2d array ptrs from memory handler
	if(!(memhandler_remove(mh, **ptr) && memhandler_remove(mh, *ptr))) {
		logger_error(str("unable to remove ptr from memory handler before realloc operation!"), str("memhandler_realloc_2d"));
		return false;
	}

	// reallocate 2d array
	if(!mem_realloc_2d(ptr, rows, cols, size)) {
		logger_error(str("unable to reallocate memory!"), str("memhandler_realloc_2d"));

		// add the old memory back to memory handler
		memhandler_add(mh, **ptr);
		memhandler_add(mh, *ptr);

		return false;
	}

	// add the newly allocated 2d array back to handler
	memhandler_add(mh, **ptr);
	memhandler_add(mh, *ptr);

	return true;
}

void memhandler_free_2d(memhandler_pt mh, void** ptr) {
	// check if handler is NULL, safely exit
	if(is_null(mh)) {
		logger_warn(str("handler is NULL; doing nothing..."), str("memhandler_free_2d"));
		return;
	}

	// remove 2d array ptrs from memory handler
	if(!(memhandler_remove(mh, *ptr) && memhandler_remove(mh, ptr))) {
		logger_error(str("unable to remove ptrs from memory handler; memory was NOT freed..."), str("memhandler_free_2d"));
	} else {
		// free 2d array
		mem_free_2d(ptr);
	}
}





















