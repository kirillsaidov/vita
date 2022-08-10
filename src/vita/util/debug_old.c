#include "vita/util/debug_old.h"

// Memory handler
struct DebugMemoryHandler {
    void *data_ptr;                 // bytes allocated container
    void **data_ptr2;               // pointer container
    size_t data_len;                // container length
    size_t data_capacity;           // container capacity
    size_t data_elsize;             // element size
    bool is_init;                   // status

    // statistics
    size_t n_allocs;                // number of allocations
    size_t n_reallocs;              // number of reallocations
    size_t n_frees;                 // number of frees
    size_t bytes_totally_alloced;   // memory size totally allocated throughout the program in bytes
    size_t bytes_freed;             // memory size freed throughout the program in bytes
};

// private memory handler that used by default
static debug_mh_t *gi_mh = NULL;
static size_t debug_mh_handler_length(const debug_mh_t *const mh);
static size_t debug_mh_handler_capacity(const debug_mh_t *const mh);
static size_t debug_mh_handler_has_space(const debug_mh_t *const mh);
static size_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t bytes);
static int64_t debug_mh_handler_find_element(debug_mh_t *const mh, const void *const ptr);
static void debug_mh_handler_cleanup(debug_mh_t *const mh);

void debug_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...) {
    if(!expr) {
        // get time
        char tbuf[21];
        get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, fmt); 
        {
            // logging to stderr
            fprintf(stderr, "%s %s %s:%d: ", tbuf, "DEBUG ASSERT", file, line);
            vfprintf(stderr, fmt, args);
            fprintf(stderr, "\n");
        }
        va_end(args);

        // if log level = fatal or assert, exit
        exit(0);
    }
}

void debug_printf(const char *const fmt, ...) {
    // get time
    char tbuf[21];
    get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
    
    // getting arguments
    va_list args; va_start(args, fmt); 
    {
        // logging to stderr
        fprintf(stderr, "%s %s ", tbuf, "DEBUG INFO");
        vfprintf(stderr, fmt, args);
    }
    va_end(args);
}

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

void *debug_mh_malloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
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
    DEBUG_PRINT("%s:%d: %zu bytes allocated\n", file, line, bytes);

    return ptr;
}

void *debug_mh_calloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line) {
    void *ptr = debug_mh_malloc(mh, bytes, file, line);
    memset(ptr, 0, bytes);
    return ptr;
}

void *debug_mh_realloc(debug_mh_t *const mh, void *ptr, const size_t bytes, const char *const file, const int32_t line) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return NULL;
    }

    // remove ptr from mh
    const size_t bytes_old = debug_mh_remove(mh, ptr);
    if(bytes > 0) {
        // allocate memory
        void *ptr_new = realloc(ptr, bytes);
        if(ptr_new == NULL) {
            DEBUG_ASSERT2(0, file, line, "Unable to allocate memory!");

            // roll back
            debug_mh_add(mh, ptr, bytes_old);
            return NULL;
        }

        // add ptr to mh
        debug_mh_add(mh, ptr_new, bytes);

        // count stats
        mh->n_reallocs++;
        mh->bytes_totally_alloced += bytes - bytes_old;

        // print info
        DEBUG_PRINT("%s:%d: %zu bytes reallocated (old size: %d)\n", file, line, bytes, bytes_old);
        
        return ptr_new;
    }

    return NULL;
}

void debug_mh_free(debug_mh_t *const mh, void *ptr, const char *const file, const int32_t line) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return;
    }

    // remove from mh
    const int64_t bytes = debug_mh_remove(mh, ptr);
    if(bytes > 0) {
        // count stats
        mh->n_frees++;
        mh->bytes_freed += bytes;

        // print info
        DEBUG_PRINT("%s:%d: %zu bytes freed\n", file, line, bytes);

        // free the data
        free(ptr);
        ptr = NULL;
    }
}

void debug_mh_add(debug_mh_t *const mh, const void *const ptr, const size_t bytes) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return;
    }

    // check if we have enough space
    if(!debug_mh_handler_has_space(mh) && !debug_mh_handler_realloc(mh, CONTAINER_GROWTH_RATE * mh->data_capacity)) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return;
    }

    // add cache data
    size_t *const p2b = mh->data_ptr;
    void **const p2p = mh->data_ptr2;
    p2b[mh->data_len] = bytes;
    p2p[mh->data_len++] = (void*)ptr;
}

size_t debug_mh_remove(debug_mh_t *const mh, const void *const ptr) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    // handlers
    size_t *const p2b = mh->data_ptr;
    void **const p2p = mh->data_ptr2;

    // remove data from cache
    size_t bytes = 0;
    if(mh->data_len > 0) {
        const int64_t i = debug_mh_handler_find_element(mh, ptr);
        if(i < 0) {
            DEBUG_ASSERT(0, "Element not found!");
            return bytes;
        }

        // remove ptr from cache
        bytes = p2b[i];
        p2b[i] = p2b[mh->data_len - 1];
        p2b[mh->data_len - 1] = 0;

        p2p[i] = p2p[mh->data_len - 1];
        p2p[mh->data_len--] = NULL;
    }

    return bytes;
}

/* ---------------------- STATS FUNCTIONS ---------------------- */

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
    *mh = (debug_mh_t) {
        .data_capacity = DEFAULT_INIT_ELEMENTS,
        .data_elsize = sizeof(char*)
    };

    // allocate memory for the container
    mh->data_ptr = calloc(DEFAULT_INIT_ELEMENTS, sizeof(size_t));
    mh->data_ptr2 = calloc(DEFAULT_INIT_ELEMENTS, sizeof(char*));

    // error checking
    if(mh->data_ptr == NULL || mh->data_ptr2 == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");

        free(mh->data_ptr);
        free(mh->data_ptr2);
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

    // free all ptrs
    const size_t len = debug_mh_handler_length(mh);
    for(size_t i = 0; i < len; i++) {
        free(mh->cache.ptr);
    }

    // reset mh
    *mh = (debug_mh_t) {0};

    // free mh
    free(mh);
    mh = NULL;
}

bool debug_mh_handler_is_init(const debug_mh_t *const mh) {
    return (mh != NULL && mh->is_init);
}

void debug_mh_handler_default_create(void) {
    gi_mh = debug_mh_handler_create();
}

void debug_mh_handler_default_destroy(void) {
    debug_mh_handler_destroy(gi_mh);
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

static size_t debug_mh_handler_length(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    return mh->data_len;
}

static size_t debug_mh_handler_capacity(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    return mh->data_capacity;
}

static size_t debug_mh_handler_has_space(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    return (mh->data_capacity - mh->data_len);
}

/// Return how much memory was allocated on success and -1 on failure
static size_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t bytes) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    // allocate memory for ptr
    void *ptr = realloc(mh->data_ptr, bytes);
    if(ptr == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return 0;
    }

    // allocate memory for ptr2
    void *ptr2 = realloc(mh->data_ptr2, bytes);
    if(ptr2 == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");

        free(ptr);
        return 0;
    }

    // update data
    mh->data_ptr = ptr;
    mh->data_ptr2 = &ptr2;
    mh->data_capacity = bytes;

    return bytes;
}

static int64_t debug_mh_handler_find_element(debug_mh_t *const mh, const void *const ptr) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return -1;
    }

    // find element
    for(size_t i = 0; i < mh->data_len; i++) {
        if(memcmp(mh->data_ptr2[i], ptr, mh->data_elsize) == 0) {
            return i;
        }
    }

    return -1;
}

debug_mh_t *debug_mh_handler_default_get_handler(void) {
    return gi_mh;
}

