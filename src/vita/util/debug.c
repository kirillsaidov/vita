#include "vita/util/debug.h"

// Memory handler
typedef struct VitaDebugMemoryCache { void *ptr; size_t bytes; } vt_cache_t;
struct VitaDebugMemoryHandler {
    vt_cache_t *cache;              // cache array
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

// private
static bool vt_debug_output_on = true;
static vt_debug_handler_t *gi_mh = NULL;
static const char *vt_debug_output_filename = NULL;

static size_t vt_debug_handler_handler_length(const vt_debug_handler_t *const mh);
static size_t vt_debug_handler_handler_has_space(const vt_debug_handler_t *const mh);
static int64_t vt_debug_handler_handler_find_element(const vt_debug_handler_t *const mh, const void *const ptr);
static size_t vt_debug_handler_handler_realloc(vt_debug_handler_t *const mh, const size_t bytes);

void vt_debug_assert(const bool expr, const char *const zexpr, const char *const file, const char *const func, const int32_t line, const char *const zfmt, ...) {
    if(!expr) {
        // get time
        char tbuf[VT_TIME_STAMP_BUFFER_SIZE] = {0};
        vt_time_get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
        
        // getting arguments
        va_list args; va_start(args, zfmt); 
        {
            if(vt_debug_output_on) {
                // create a file
                FILE *fp = (vt_debug_output_filename == NULL) ? stderr : fopen(vt_debug_output_filename, "a");
                if(fp == NULL) { fp = stderr; }

                // logging to fp
                fprintf(fp, "%s %s [%s] %s:%s:%d: ", tbuf, "DEBUG ASSERTION FAILURE", zexpr, file, func, line);
                vfprintf(fp, zfmt, args);
                fprintf(fp, "\n");

                // close the file, if needed
                if(vt_debug_output_filename != NULL) { fclose(fp); }
            }
        }
        va_end(args);

        // exit upon failure
        exit(EXIT_FAILURE);
    }
}

void vt_debug_printf(const char *const zfmt, ...) {
    // check for invalid input
    assert(zfmt != NULL);

    // get time
    char tbuf[VT_TIME_STAMP_BUFFER_SIZE] = {0};
    vt_time_get_current_timestamp(tbuf, sizeof(tbuf) / sizeof(tbuf[0]));
    
    // getting arguments
    va_list args; va_start(args, zfmt); 
    {
        if(vt_debug_output_on) {
            // create a file
            FILE *fp = (vt_debug_output_filename == NULL) ? stderr : fopen(vt_debug_output_filename, "a");
            if(fp == NULL) { fp = stderr; }

            // logging to stderr
            fprintf(fp, "%s %s ", tbuf, "DEBUG INFO");
            vfprintf(fp, zfmt, args);

            // close the file, if needed
            if(vt_debug_output_filename != NULL) { fclose(fp); }
        }
    }
    va_end(args);
}

void vt_debug_redirect_output(const char *const zfilename) {
    vt_debug_output_filename = zfilename;
}

void vt_debug_disable_output(const bool off) {
    vt_debug_output_on = !off;
}

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

vt_debug_handler_t *vt_debug_handler_create(void) {
    // create a mh handler instance
    vt_debug_handler_t *mh = malloc(sizeof(vt_debug_handler_t));
    if(mh == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // initialize mh container
    *mh = (vt_debug_handler_t) {
        .cache = calloc(1, VT_DEFAULT_INIT_ELEMENTS * sizeof(vt_cache_t)), // each cache is a single instance, we need an array of them
        .cache_capacity = VT_DEFAULT_INIT_ELEMENTS,
        .cache_elsize = sizeof(char*)
    };

    // allocate memory for cache
    if(mh->cache == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        free(mh);
        return NULL;
    }

    return mh;
}

void vt_debug_handler_destroy(vt_debug_handler_t *mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // free all ptrs
    const size_t len = vt_debug_handler_handler_length(mh);
    for(size_t i = 0; i < len; i++) {
        free(mh->cache[i].ptr);
    }

    // free cache
    free(mh->cache);
    mh->cache = NULL;

    // reset to zero
    memset(mh, 0, sizeof(vt_debug_handler_t));

    // free mh
    free(mh);
    mh = NULL;
}

bool vt_debug_handler_default_init(void) {
    gi_mh = (gi_mh == NULL) ? vt_debug_handler_create() : gi_mh;

    VT_DEBUG_ASSERT(gi_mh != NULL, "%s: Failed to initialize the default memory handler!\n", vt_get_vita_error_str(vt_ve_error_allocation));
    return (gi_mh != NULL);
}

void vt_debug_handler_default_quit(void) {
    vt_debug_handler_destroy(gi_mh);
}

vt_debug_handler_t *vt_debug_handler_default_get_handler(void) {
    return gi_mh;
}

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

void *vt_debug_handler_malloc(vt_debug_handler_t *const mh, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate memory
    void *ptr = malloc(bytes);
    if(ptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // add ptr to mh
    vt_debug_handler_add(mh, ptr, bytes);

    // count stats
    mh->n_allocs++;
    mh->bytes_totally_alloced += bytes;

    // print info
    VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes allocated\n", file, func, line, bytes);

    return ptr;
}

void *vt_debug_handler_calloc(vt_debug_handler_t *const mh, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // allocate memory
    void *ptr = calloc(1, bytes);
    if(ptr == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // add ptr to mh
    vt_debug_handler_add(mh, ptr, bytes);

    // count stats
    mh->n_allocs++;
    mh->bytes_totally_alloced += bytes;

    // print info
    VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes allocated\n", file, func, line, bytes);

    return ptr;
}

void *vt_debug_handler_realloc(vt_debug_handler_t *const mh, void *ptr, const size_t bytes, const char *const file, const char *const func, const int32_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // remove ptr from mh
    const size_t bytes_old = vt_debug_handler_remove(mh, ptr);
    
    // allocate memory
    void *ptr_new = realloc(ptr, bytes);
    if(ptr_new == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));

        // roll back
        vt_debug_handler_add(mh, ptr, bytes_old);
        return NULL;
    }

    // add ptr to mh
    vt_debug_handler_add(mh, ptr_new, bytes);

    // count stats
    mh->n_reallocs++;
    mh->bytes_totally_alloced += ((int64_t)bytes - (int64_t)bytes_old);

    // print info
    VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes reallocated (old size: %d)\n", file, func, line, bytes, bytes_old);
    
    return ptr_new;
}

void vt_debug_handler_free(vt_debug_handler_t *const mh, void *ptr, const char *const file, const char *const func, const int32_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // remove from mh
    const size_t bytes = vt_debug_handler_remove(mh, ptr);
    if(bytes > 0) {
        // count stats
        mh->n_frees++;
        mh->bytes_freed += bytes;

        // print info
        VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes freed (left: %zu)\n", file, func, line, bytes, vt_debug_handler_get_bytes_currently_alloced(mh));
    }

    // free the data
    free(ptr);
    ptr = NULL;
}

void vt_debug_handler_add(vt_debug_handler_t *const mh, const void *const ptr, const size_t bytes) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // check if we have enough space
    if(!vt_debug_handler_handler_has_space(mh) && !vt_debug_handler_handler_realloc(mh, 2 * mh->cache_capacity)) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return;
    }

    // add cache data
    mh->cache[mh->cache_len].ptr = (void*)ptr;
    mh->cache[mh->cache_len++].bytes = bytes;
}

size_t vt_debug_handler_remove(vt_debug_handler_t *const mh, const void *const ptr) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // remove data from cache
    size_t bytes = 0;
    if(mh->cache_len > 0) {
        const int64_t i = vt_debug_handler_handler_find_element(mh, ptr);
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

static size_t vt_debug_handler_handler_length(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return mh->cache_len;
}

static size_t vt_debug_handler_handler_has_space(const vt_debug_handler_t *const mh)  {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return (mh->cache_capacity - mh->cache_len);
}

static int64_t vt_debug_handler_handler_find_element(const vt_debug_handler_t *const mh, const void *const ptr) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // find element
    const size_t len = vt_debug_handler_handler_length(mh);
    for(size_t i = 0; i < len; i++) {
        if(mh->cache[i].ptr == ptr) {
            return i;
        }
    }

    return -1;
}

static size_t vt_debug_handler_handler_realloc(vt_debug_handler_t *const mh, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));
    VT_DEBUG_ASSERT(len > 0, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // reallocate memory for cache
    void *ptr = realloc(mh->cache, len * sizeof(vt_cache_t));
    if(ptr == NULL) {
        return 0;
    }

    // update data
    mh->cache = ptr;
    mh->cache_capacity = len;

    return (len * sizeof(vt_cache_t));
}

/* ---------------------- STATS FUNCTIONS ---------------------- */

size_t vt_debug_handler_get_nallocs(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return mh->n_allocs;
}

size_t vt_debug_handler_get_nreallocs(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return mh->n_reallocs;
}

size_t vt_debug_handler_get_nfrees(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return mh->n_frees;
}

size_t vt_debug_handler_get_bytes_totally_alloced(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return mh->bytes_totally_alloced;
}

size_t vt_debug_handler_get_bytes_currently_alloced(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return (mh->bytes_totally_alloced - mh->bytes_freed);
}

size_t vt_debug_handler_get_bytes_freed(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    return mh->bytes_freed;
}

char *vt_debug_handler_get_stats_str(const vt_debug_handler_t *const mh, char *buffer, const size_t buff_len) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // check buffer size
    const size_t req_buf_len = VT_DEBUG_STATS_BUFFER_SIZE;
    if(buff_len < req_buf_len) {
        VT_DEBUG_PRINTF("%s: buff_len must be >= %zu!\n", vt_get_vita_error_str(vt_ve_error_out_of_memory), req_buf_len);
        return NULL;
    }

    // allocate buffer if needed
    char *buf = (buffer == NULL) ? calloc(req_buf_len, sizeof(char)) : buffer;
    if(buf == NULL) {
        VT_DEBUG_PRINTF("%s\n", vt_get_vita_error_str(vt_ve_error_allocation));
        return NULL;
    }

    // clear buffer
    memset(buf, 0, req_buf_len * sizeof(char));

    // create formatter
    const char *const zfmt = 
        "N      allocs: %zu\n"
        "N    reallocs: %zu\n"
        "N       frees: %zu\n"
        "B tot.alloced: %zu\n"
        "B cur.alloced: %zu\n"
        "B       freed: %zu\n"
        "(N) - count\n"
        "(B) - bytes\n";

    // print data to buffer
    sprintf(
        buffer, zfmt,
        vt_debug_handler_get_nallocs(mh),
        vt_debug_handler_get_nreallocs(mh),
        vt_debug_handler_get_nfrees(mh),
        vt_debug_handler_get_bytes_totally_alloced(mh),
        vt_debug_handler_get_bytes_currently_alloced(mh),
        vt_debug_handler_get_bytes_freed(mh)
    );

    return buffer;
}

void vt_debug_handler_print_stats(const vt_debug_handler_t *const mh) {
    // check for invalid input
    VT_DEBUG_ASSERT(mh != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(mh->cache != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_is_null));

    // get stats
    char z_stats[VT_DEBUG_STATS_BUFFER_SIZE] = {0};
    vt_debug_handler_get_stats_str(mh, z_stats, VT_DEBUG_STATS_BUFFER_SIZE);

    // print to stderr
    fprintf(stderr, "%s\n", z_stats);
}







