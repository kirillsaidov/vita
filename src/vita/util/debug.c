#include "vita/util/debug.h"

// Memory handler
typedef struct DebugMemoryCache { void *ptr; size_t bytes; } cache_t;
struct DebugMemoryHandler {
    cache_t *cache;                 // cache
    size_t cache_len;               // cache length
    size_t cache_capacity;          // cache capacity
    size_t cache_elsize;            // cache element size

    // statistics
    size_t n_allocs;                // number of allocations
    size_t n_reallocs;              // number of reallocations
    size_t n_frees;                 // number of frees
    size_t bytes_totally_alloced;   // memory size totally allocated throughout the program in bytes
    size_t bytes_freed;             // memory size freed throughout the program in bytes
};

// private functions
static debug_mh_t *gi_mh = NULL;
static size_t debug_mh_handler_length(const debug_mh_t *const mh);
static size_t debug_mh_handler_has_space(const debug_mh_t *const mh);
static int64_t debug_mh_handler_find_element(const debug_mh_t *const mh, const void *const ptr);
static size_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t bytes);

void debug_assert(const bool expr, const char *const file, const char *const func, const int32_t line, const char *const fmt, ...) {
    if(!expr) {
        // get time
        char tbuf[21] = {0};
        get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, fmt); 
        {
            // logging to stderr
            fprintf(stderr, "%s %s %s:%s:%d: ", tbuf, "DEBUG ASSERT", file, func, line);
            vfprintf(stderr, fmt, args);
            fprintf(stderr, "\n");
        }
        va_end(args);

        // exit upon failure
        exit(EXIT_FAILURE);
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

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

debug_mh_t *debug_mh_handler_new(void) {
    // create a mh handler instance
    debug_mh_t *mh = malloc(sizeof(debug_mh_t));
    if(mh == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return NULL;
    }

    // initialize mh container
    *mh = (debug_mh_t) {
        .cache = calloc(1, DEFAULT_INIT_ELEMENTS * sizeof(cache_t)),
        .cache_capacity = DEFAULT_INIT_ELEMENTS,
        .cache_elsize = sizeof(char*)
    };

    // allocate memory for cache
    if(mh->cache == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");

        free(mh);
        return NULL;
    }

    return mh;
}

void debug_mh_handler_free(debug_mh_t *mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return;
    }

    // free all ptrs
    const size_t len = debug_mh_handler_length(mh);
    for(size_t i = 0; i < len; i++) {
        free(mh->cache[i].ptr);
    }

    // free cache
    free(mh->cache);
    mh->cache = NULL;

    // reset mh to zero
    *mh = (debug_mh_t) {0};

    // free mh
    free(mh);
    mh = NULL;
}

bool debug_mh_handler_is_init(const debug_mh_t *const mh) {
    return (mh != NULL && mh->cache != NULL);
}

bool debug_mh_handler_default_init(void) {
    gi_mh = (gi_mh == NULL) ? debug_mh_handler_new() : gi_mh;

    DEBUG_ASSERT(gi_mh != NULL, "Failed to initialize the default memory handler!");
    return (gi_mh != NULL);
}

void debug_mh_handler_default_quit(void) {
    debug_mh_handler_free(gi_mh);
}

debug_mh_t *debug_mh_handler_default_get_handler(void) {
    return gi_mh;
}

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

void *debug_mh_malloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    // error checking
    DEBUG_ASSERT2(bytes, file, func, line, "Cannot allocate %zu bytes!", bytes);
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_PRINT("%s\n", "Memory handler was not initialized. Initializing the default memory handler.");
        
        // create the default memory handler
        if(!debug_mh_handler_default_init()) {
            return NULL;
        }

        return debug_mh_malloc(debug_mh_handler_default_get_handler(), bytes, file, func, line);
    }

    // allocate memory
    void *ptr = malloc(bytes);
    if(ptr == NULL) {
        DEBUG_ASSERT2(0, file, func, line, "Unable to allocate memory!");
        return NULL;
    }

    // add ptr to mh
    debug_mh_add(mh, ptr, bytes);

    // count stats
    mh->n_allocs++;
    mh->bytes_totally_alloced += bytes;

    // print info
    DEBUG_PRINT("%s:%s:%d: %zu bytes allocated\n", file, func, line, bytes);

    return ptr;
}

void *debug_mh_calloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    void *ptr = debug_mh_malloc(mh, bytes, file, func, line);
    memset(ptr, 0, bytes);
    return ptr;
}

void *debug_mh_realloc(debug_mh_t *const mh, void *ptr, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    // error checking
    DEBUG_ASSERT2(bytes, file, func, line, "Cannot reallocate to %zu bytes! Use `debug_mh_free` instead!", bytes);
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_PRINT("%s\n", "Memory handler was not initialized. Initializing the default memory handler.");
        
        // create the default memory handler
        if(!debug_mh_handler_default_init()) {
            return NULL;
        }

        return debug_mh_realloc(debug_mh_handler_default_get_handler(), ptr, bytes, file, func, line);
    }

    // remove ptr from mh
    const size_t bytes_old = debug_mh_remove(mh, ptr);
    if(bytes > 0) {
        // allocate memory
        void *ptr_new = realloc(ptr, bytes);
        if(ptr_new == NULL) {
            DEBUG_ASSERT2(0, file, func, line, "Unable to allocate memory!");

            // roll back
            debug_mh_add(mh, ptr, bytes_old);
            return NULL;
        }

        // add ptr to mh
        debug_mh_add(mh, ptr_new, bytes);

        // count stats
        mh->n_reallocs++;
        mh->bytes_totally_alloced += ((int64_t)bytes - (int64_t)bytes_old);

        // print info
        DEBUG_PRINT("%s:%s:%d: %zu bytes reallocated (old size: %d)\n", file, func, line, bytes, bytes_old);
        
        return ptr_new;
    }

    return NULL;
}

void debug_mh_free(debug_mh_t *const mh, void *ptr, const char *const file, const char *const func, const int32_t line) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT2(0, file, func, line, "Memory handler was not initialized!");
        return;
    }

    // remove from mh
    const size_t bytes = debug_mh_remove(mh, ptr);
    if(bytes > 0) {
        // count stats
        mh->n_frees++;
        mh->bytes_freed += bytes;

        // print info
        DEBUG_PRINT("%s:%s:%d: %zu bytes freed (left: %zu)\n", file, func, line, bytes, debug_mh_get_bytes_currently_alloced(mh));

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
    if(!debug_mh_handler_has_space(mh) && !debug_mh_handler_realloc(mh, 2 * mh->cache_capacity)) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return;
    }

    // add cache data
    mh->cache[mh->cache_len].ptr = (void*)ptr;
    mh->cache[mh->cache_len++].bytes = bytes;
}

size_t debug_mh_remove(debug_mh_t *const mh, const void *const ptr) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return 0;
    }

    // remove data from cache
    size_t bytes = 0;
    if(mh->cache_len > 0) {
        const int64_t i = debug_mh_handler_find_element(mh, ptr);
        if(i < 0) {
            return bytes;
        }

        // remove ptr from cache
        bytes = mh->cache[i].bytes;
        mh->cache[i].ptr = mh->cache[mh->cache_len - 1].ptr;
        mh->cache[i].bytes = mh->cache[--mh->cache_len].bytes;
    }

    return bytes;
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

static size_t debug_mh_handler_length(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? mh->cache_len : 0;
}

static size_t debug_mh_handler_has_space(const debug_mh_t *const mh)  {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? (mh->cache_capacity - mh->cache_len) : 0;
}

static int64_t debug_mh_handler_find_element(const debug_mh_t *const mh, const void *const ptr) {
    if(!debug_mh_handler_is_init(mh)) {
        return -1;
    }

    // find element
    const size_t len = debug_mh_handler_length(mh);
    for(size_t i = 0; i < len; i++) {
        if(mh->cache[i].ptr == ptr) {
            return i;
        }
    }

    return -1;
}

static size_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t len) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    // reallocate memory for cache
    void *ptr = realloc(mh->cache, len * sizeof(cache_t));
    if(ptr == NULL) {
        return 0;
    }

    // update data
    mh->cache = ptr;
    mh->cache_capacity = len;

    return (len * sizeof(cache_t));
}

/* ---------------------- STATS FUNCTIONS ---------------------- */

size_t debug_mh_get_nallocs(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? mh->n_allocs : 0;
}

size_t debug_mh_get_nreallocs(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? mh->n_reallocs : 0;
}

size_t debug_mh_get_nfrees(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? mh->n_frees : 0;
}

size_t debug_mh_get_bytes_totally_alloced(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? mh->bytes_totally_alloced : 0;
}

size_t debug_mh_get_bytes_currently_alloced(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? (mh->bytes_totally_alloced - mh->bytes_freed) : 0;
}

size_t debug_mh_get_bytes_freed(const debug_mh_t *const mh) {
    DEBUG_ASSERT(debug_mh_handler_is_init(mh), "Memory handler was not initialized!");
    return debug_mh_handler_is_init(mh) ? mh->bytes_freed : 0;
}

char *debug_mh_get_stats_str(const debug_mh_t *const mh, char *buffer, const size_t buff_len) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return NULL;
    }

    // check buffer size
    const size_t req_buf_len = 256;
    if(buff_len < req_buf_len) {
        DEBUG_ASSERT(0, "buff_len must be >= 256!");
        return NULL;
    }

    char *buf = (buffer == NULL) ? calloc(req_buf_len, sizeof(char)) : buffer;
    if(buf == NULL) {
        DEBUG_ASSERT(0, "Unable to allocate memory!");
        return NULL;
    }

    // clear buffer
    memset(buf, 0, req_buf_len * sizeof(char));

    // create formatter
    const char *const fmt = 
"\
N      allocs: %zu\n\
N    reallocs: %zu\n\
N       frees: %zu\n\
B tot.alloced: %zu\n\
B cur.alloced: %zu\n\
B       freed: %zu\n\
(N) - count\n\
(B) - bytes\
";

    // print data to buffer
    sprintf(
        buffer, fmt,
        debug_mh_get_nallocs(mh),
        debug_mh_get_nreallocs(mh),
        debug_mh_get_nfrees(mh),
        debug_mh_get_bytes_totally_alloced(mh),
        debug_mh_get_bytes_currently_alloced(mh),
        debug_mh_get_bytes_freed(mh)
    );

    return buffer;
}

void debug_mh_print_stats(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        DEBUG_ASSERT(0, "Memory handler was not initialized!");
        return;
    }

    // get stats
    char s_stats[256] = {0};
    debug_mh_get_stats_str(mh, s_stats, sizeof(s_stats)/sizeof(s_stats[0]));

    // print to stderr
    fprintf(stderr, "%s\n", s_stats);
}







