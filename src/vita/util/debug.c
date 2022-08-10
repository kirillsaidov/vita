#include "vita/util/debug.h"

// Memory handler
typedef struct Cache { void *ptr; size_t size; } cache_t;
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
static size_t debug_mh_handler_capacity(const debug_mh_t *const mh);
static size_t debug_mh_handler_has_space(const debug_mh_t *const mh);
static int64_t debug_mh_handler_find_element(const debug_mh_t *const mh, const void *const ptr);
static size_t debug_mh_handler_realloc(debug_mh_t *const mh, const size_t bytes);

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

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

debug_mh_t *debug_mh_handler_create(void) {
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

void debug_mh_handler_destroy(debug_mh_t *mh) {
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

void debug_mh_handler_default_create(void) {
    gi_mh = debug_mh_handler_create();
}

void debug_mh_handler_default_destroy(void) {
    debug_mh_handler_destroy(gi_mh);
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

static size_t debug_mh_handler_length(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->cache_len;
}

static size_t debug_mh_handler_capacity(const debug_mh_t *const mh) {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return mh->cache_capacity;
}

static size_t debug_mh_handler_has_space(const debug_mh_t *const mh)  {
    if(!debug_mh_handler_is_init(mh)) {
        return 0;
    }

    return (mh->cache_capacity - mh->cache_len);
}

static int64_t debug_mh_handler_find_element(const debug_mh_t *const mh, const void *const ptr) {
    if(!debug_mh_handler_is_init(mh)) {
        return -1;
    }

    // find element
    const size_t len = debug_mh_handler_length(mh);
    for(size_t i = 0; i < len; i++) {
        if(memcmp(mh->cache[i].ptr, ptr, mh->cache_elsize) == 0) {
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