#include "vita/util/debug.h"

static int64_t debug_mh_handler_length(const debug_mh_t *const mh);
static int64_t debug_mh_handler_capacity(const debug_mh_t *const mh);
static int64_t debug_mh_handler_has_space(const debug_mh_t *const mh);
static int64_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t bytes);
static int64_t debug_mh_handler_find_element(debug_mh_t *const mh, const void *const ptr);

void debug_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...) {
    if(!expr) {
        // get time
        char tbuf[21];
        get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, fmt); 
        {
            // logging to stderr
            fprintf(stderr, "%s %5s %s:%d: ", tbuf, "DEBUG ASSERT", file, line);
            vfprintf(stderr, fmt, args);
            fprintf(stderr, "\n");
        }
        va_end(args);

        // if log level = fatal or assert, exit
        exit(0);
    }
}

void *debug_mh_malloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line) {
    if(!debug_mh_handler_is_init_then(mh)) {
        return NULL;
    }

    // allocate memory
    void *ptr = malloc(bytes);
    if(ptr == NULL) {
        DEBUG_ASSERT2(0, file, line, "Unable to allocate memory!");
        return NULL;
    }

    // add ptr to mh
    debug_mh_add(mh, ptr, bytes);

    // count stats
    mh->n_allocs++;
    mh->bytes_totally_alloced += bytes;

    // print info
    DEBUG_PRINT("Allocated: %zu bytes at %s:%d\n", bytes, file, line);

    return ptr;
}

void *debug_mh_calloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line) {
    void *ptr = debug_mh_malloc(mh, bytes, file, line);
    memset(ptr, 0, bytes);
    return ptr;
}

void *debug_mh_realloc(debug_mh_t *const mh, void *ptr, const size_t bytes, const char *const file, const int32_t line) {
    if(!debug_mh_handler_is_init_then(mh)) {
        return NULL;
    }

    // remove ptr from mh
    const size_t bytes_old = debug_mh_remove(mh, ptr);

    // allocate memory
    void *ptr_new = realloc(ptr, bytes);
    if(ptr_new == NULL) {
        DEBUG_ASSERT2(0, file, line, "Unable to allocate memory!");

        debug_mh_add(mh, ptr, bytes_old);
        return NULL;
    }

    // add ptr to mh
    debug_mh_add(mh, ptr_new, bytes);

    // count stats
    mh->n_reallocs++;
    mh->bytes_totally_alloced += bytes;

    // print info
    DEBUG_PRINT("Reallocated: %zu bytes at %s:%d\n", bytes, file, line);

    return ptr_new;
}

void debug_mh_free(debug_mh_t *const mh, void *ptr, const char *const file, const int32_t line) {
    if(debug_mh_handler_is_init(mh)) {
        goto debug_cleanup;
    }

    // remove from mh
    const size_t bytes = debug_mh_remove(mh, ptr);
    if(bytes < -1) {
        DEBUG_ASSERT(0, "Element was not found!");
        goto debug_cleanup;
    }

    // count stats
    mh->n_frees++;
    mh->bytes_freed += bytes;

    // print info
    DEBUG_PRINT("Freed: %zu bytes at %s:%d\n", bytes, file, line);

debug_cleanup:
    free(ptr);
    ptr = NULL;
}

void debug_mh_add(debug_mh_t *const mh, const void *const ptr, const size_t bytes) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return;
    }

    // check if we have enough space
    if(!debug_mh_handler_has_space(mh) && !debug_mh_handler_realloc(mh, CONTAINER_GROWTH_RATE * mh->data.capacity)) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return;
    }

    // add data to cache
    size_t *const p2b = mh->data.ptr;
    void **const p2p = mh->data.ptr2;
    p2b[mh->data.len] = bytes;
    p2p[mh->data.len++] = (void*)ptr;
}

int64_t debug_mh_remove(debug_mh_t *const mh, const void *const ptr) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return -1;
    }

    // remove data from cache
    uint64_t bytes = -1;
    if(mh->data.len > 0) {
        const size_t i = debug_mh_handler_find_element(mh, ptr);
        if(i < 0) {
            DEBUG_ASSERT(0, "Element not found!");
            return bytes;
        }

        // remove ptr from cache
        size_t *const p2b = mh->data.ptr;
        void **const p2p = mh->data.ptr2;

        bytes = p2b[i];
        p2b[i] = p2b[mh->data.len - 1];
        p2b[mh->data.len - 1] = 0;

        p2p[i] = p2p[mh->data.len - 1];
        p2p[mh->data.len--] = NULL;
    }

    return bytes;
}

void debug_mh_cleanup(void) {
    if(!debug_mh_handler_is_init(gi_mh)) {
        return;
    }

    // free all data
    const size_t len = debug_mh_handler_length(gi_mh);
    for(size_t i = 0; i < len; i++) {
        free(gi_mh->data.ptr2[i]);
    }

    // free the mh
    debug_mh_handler_destroy(gi_mh);
}

size_t debug_mh_get_nallocs(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->n_allocs;
}

size_t debug_mh_get_nreallocs(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->n_reallocs;
}

size_t debug_mh_get_nfrees(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->n_frees;
}

size_t debug_mh_get_bytes_totally_alloced(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->bytes_totally_alloced;
}

size_t debug_mh_get_bytes_currently_alloced(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return (mh->bytes_totally_alloced - mh->bytes_freed);
}

size_t debug_mh_get_bytes_freed(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->bytes_freed;
}

// char *debug_mh_get_stats_str(const debug_mh_t *const mh, char *buffer);
// void debug_mh_print_stats(const debug_mh_t *const mh);

/* ---------------------- mh HANDLER FUNCTIONS ---------------------- */

debug_mh_t *debug_mh_handler_create(void) {
    // create a mh handler instance
    debug_mh_t *mh = calloc(1, sizeof(debug_mh_t));
    if(mh == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return NULL;
    }

    // initialize data container
    mh->data = (struct BaseArrayType) {
        .len = 0,
        .capacity = DEFAULT_INIT_ELEMENTS,
        .elsize = sizeof(char*)
    };

    // allocate memory for the container
    mh->data.ptr = calloc(DEFAULT_INIT_ELEMENTS, sizeof(size_t));
    mh->data.ptr2 = calloc(DEFAULT_INIT_ELEMENTS, sizeof(size_t));

    // error checking
    if(mh->data.ptr == NULL || mh->data.ptr2 == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");

        free(mh->data.ptr);
        free(mh->data.ptr2);
        free(mh);
        return NULL;
    }

    // set is_init to true
    mh->is_init = true;

    return mh;
}

void debug_mh_handler_destroy(debug_mh_t *mh) {
    if(mh == NULL) {
        return;
    }

    // free data
    free(mh->data.ptr);
    free(mh->data.ptr2);
    mh->data = (struct BaseArrayType) {0};

    // free mh
    free(mh);
    mh = NULL;
}

bool debug_mh_handler_is_init(const debug_mh_t *const mh) {
    if(mh == NULL || !mh->is_init) {
        return false;
    }

    return true;
}

bool debug_mh_handler_is_init_then(debug_mh_t *mh) {
    if(mh == NULL || !mh->is_init) {
        free(mh);
        mh = debug_mh_handler_create();
    }

    return (mh != NULL);
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

static int64_t debug_mh_handler_length(const debug_mh_t *const mh) {
    if(mh == NULL || !mh->is_init) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return -1;
    }

    return mh->data.len;
}

static int64_t debug_mh_handler_capacity(const debug_mh_t *const mh) {
    if(mh == NULL || !mh->is_init) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return -1;
    }

    return mh->data.capacity;
}

static int64_t debug_mh_handler_has_space(const debug_mh_t *const mh) {
    if(mh == NULL || !mh->is_init) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    return (mh->data.capacity - mh->data.len);
}

/// Return how much memory was allocated on success and -1 on failure
static int64_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t bytes) {
    if(mh == NULL || !mh->is_init) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return -1;
    }

    // allocate memory for ptr
    void *ptr = realloc(mh->data.ptr, bytes);
    if(ptr == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return -1;
    }

    // allocate memory for ptr2
    void *ptr2 = realloc(*mh->data.ptr2, bytes);
    if(ptr2 == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");

        free(ptr);
        return -1;
    }

    // update data
    mh->data.ptr = ptr;
    mh->data.ptr2 = &ptr;
    mh->data.capacity = bytes;

    return bytes;
}

static int64_t debug_mh_handler_find_element(debug_mh_t *const mh, const void *const ptr) {
    if(mh == NULL || !mh->is_init) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return -1;
    }

    // find element
    for(size_t i = 0; i < mh->data.len; i++) {
        if(memcmp(mh->data.ptr2[i], ptr, mh->data.elsize) == 0) {
            return i;
        }
    }

    return -1;
}