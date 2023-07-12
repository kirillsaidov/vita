#include "vita/allocator/mallocator.h"

static void vt_mallocator_obj_list_add(vt_mallocator_t *const alloctr, const struct VitaAllocatedObject obj);
static size_t vt_mallocator_obj_list_remove(vt_mallocator_t *const alloctr, const void *const ptr);
static void vt_mallocator_obj_list_resize(vt_mallocator_t *const alloctr, const size_t length);
static bool vt_mallocator_obj_list_has_space(const vt_mallocator_t *const alloctr);
static int64_t vt_mallocator_obj_list_find(const vt_mallocator_t *const alloctr, const void *const ptr);

vt_mallocator_t *vt_mallocator_create(void) {
    // create a mallocator instance
    vt_mallocator_t *alloctr = VT_CALLOC(sizeof(vt_mallocator_t));
    
    // initialize object list
    *alloctr = (vt_mallocator_t) {
        .obj_list = VT_CALLOC(sizeof(struct VitaAllocatedObject) * VT_ARRAY_DEFAULT_INIT_ELEMENTS),
        .obj_list_capacity = VT_ARRAY_DEFAULT_INIT_ELEMENTS
    };

    // set up functions
    alloctr->alloc = vt_mallocator_alloc;
    alloctr->realloc = vt_mallocator_realloc;
    alloctr->free = vt_mallocator_free;

    return alloctr;
}

void vt_mallocator_destroy(vt_mallocator_t *alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // reset function pointers to NULL
    alloctr->alloc = NULL;
    alloctr->realloc = NULL;
    alloctr->free = NULL;

    // free all objects in object list
    VT_FOREACH(iter, 0, alloctr->obj_list_len) {
        VT_FREE(alloctr->obj_list[iter].ptr);

        // reset
        alloctr->obj_list[iter].ptr = NULL;
        alloctr->obj_list[iter].bytes = 0;
    }

    // free the object list itself
    VT_FREE(alloctr->obj_list);
    alloctr->obj_list = NULL;
    alloctr->obj_list_len = 0;
    alloctr->obj_list_capacity = 0;

    // free allocator itself
    VT_FREE(alloctr);
    alloctr = NULL;
}

void *vt_mallocator_alloc(vt_mallocator_t *const alloctr, const size_t bytes, const char *const file, const char *const func, const size_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // allocate memory
    const struct VitaAllocatedObject obj = { 
        .ptr = vt_calloc(bytes, file, func, line),
        .bytes = bytes
    };

    // add the pointer to the mallocator instance object list
    vt_mallocator_obj_list_add(alloctr, obj);

    // update stats
    alloctr->stats.count_allocs++;
    alloctr->stats.count_bytes_allocated += bytes;

    // debug info
    VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes allocated\n", file, func, line, bytes);

    return obj.ptr;
}

void *vt_mallocator_realloc(vt_mallocator_t *const alloctr, void *ptr, const size_t bytes, const char *const file, const char *const func, const size_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // remove the pointer from the mallocator instance object list
    const size_t bytes_old = vt_mallocator_obj_list_remove(alloctr, ptr);
    
    // reallocate memory
    const struct VitaAllocatedObject obj = { 
        .ptr = vt_realloc(ptr, bytes, file, func, line), 
        .bytes = bytes
    };

    // add the pointer to the mallocator instance object list
    vt_mallocator_obj_list_add(alloctr, obj);

    // update stats
    const int64_t bytes_diff = ((int64_t)bytes - bytes_old);
    alloctr->stats.count_reallocs++;
    alloctr->stats.count_bytes_allocated += bytes_diff;
    alloctr->stats.count_bytes_freed -= bytes_diff < 0 ? bytes_diff : 0;

    // debug info
    VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes reallocated (old size: %d)\n", file, func, line, bytes, bytes_old);

    return obj.ptr;
}

void vt_mallocator_free(vt_mallocator_t *const alloctr, void *ptr, const char *const file, const char *const func, const size_t line) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // remove the pointer from the mallocator instance object list
    const size_t bytes = vt_mallocator_obj_list_remove(alloctr, ptr);
    if (bytes > 0) {
        // update stats
        alloctr->stats.count_frees++;
        alloctr->stats.count_bytes_freed += bytes;
        alloctr->stats.count_bytes_allocated -= bytes;

        // debug info
        VT_DEBUG_PRINTF("%s:%s:%d: %zu bytes freed (left: %zu)\n", file, func, line, bytes, alloctr->stats.count_bytes_allocated);
        (void)file;
        (void)func;
        (void)line;
    }

    // free the data
    vt_free(ptr);
    ptr = NULL;
}

void vt_mallocator_print_stats(const struct VitaAllocatorStats stats) {
    // create stats formatter
    const int8_t width = 10;
    const char *const fmt = 
        "|-------------------------|\n"
        "|    MALLOCATOR STATS     |\n"
        "|-------------------------|\n"
        "| N   ALLOCS | %*zu |\n"        // %*zu, * = 10 = width
        "| N REALLOCS | %*zu |\n"
        "| N    FREES | %*zu |\n"
        "|-------------------------|\n"
        "| B T.ALLOCD | %*zu |\n"
        "| B C.ALLOCD | %*zu |\n"
        "| B    FREED | %*zu |\n"
        "|-------------------------|\n"
        "| (N) - COUNT             |\n"
        "| (B) - BYTES             |\n"
        "| (T) - TOTAL             |\n"
        "| (C) - CURRENT           |\n"
        "|-------------------------|\n";
    
    printf(
        fmt,
        width, stats.count_allocs,
        width, stats.count_reallocs,
        width, stats.count_frees,
        width, stats.count_bytes_allocated + stats.count_bytes_freed,
        width, stats.count_bytes_allocated,
        width, stats.count_bytes_freed
    );
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

/** Adds pointer to object list
    @param alloctr allocator instance
    @param obj allocator object instance
*/
static void vt_mallocator_obj_list_add(vt_mallocator_t *const alloctr, const struct VitaAllocatedObject obj) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(obj.ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    
    // check if we have enough space
    if (!vt_mallocator_obj_list_has_space(alloctr)) {
        vt_mallocator_obj_list_resize(alloctr, 2 * alloctr->obj_list_capacity);
    }

    // add object
    alloctr->obj_list[alloctr->obj_list_len++] = obj;
}

/** Removes pointer from object list
    @param alloctr allocator instance
    @param ptr pointer to remove

    @returns bytes allocated for that pointer or `0` if pointer wasn't found in the object list
*/
static size_t vt_mallocator_obj_list_remove(vt_mallocator_t *const alloctr, const void *const ptr) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // remove only if the object list isn't empty
    size_t bytes_old = 0;
    const size_t obj_list_len = alloctr->obj_list_len;
    if (obj_list_len > 0) {
        const int64_t idx = vt_mallocator_obj_list_find(alloctr, ptr);
        if (idx < 0) {
            return bytes_old;
        }

        // retrieve bytes from the object we want to remove
        bytes_old = alloctr->obj_list[idx].bytes;

        // remove only if it's not the last in the list
        if (idx != (int64_t)(obj_list_len-1)) { 
            alloctr->obj_list[idx] = alloctr->obj_list[obj_list_len-1];
        }
        alloctr->obj_list_len--;
    }

    return bytes_old;
}

/** Resize object list
    @param alloctr allocator instance
    @param size new size
*/
static void vt_mallocator_obj_list_resize(vt_mallocator_t *const alloctr, const size_t length) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(length > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // resize object list
    alloctr->obj_list = VT_REALLOC(alloctr->obj_list, length * sizeof(struct VitaAllocatedObject));
    alloctr->obj_list_capacity = length;
}

/** Check if object list has enough space
    @param alloctr allocator instance
    @returns ditto
*/
static bool vt_mallocator_obj_list_has_space(const vt_mallocator_t *const alloctr) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    return (alloctr->obj_list_capacity - alloctr->obj_list_len);
}

/** Find index of a pointer in object list
    @param alloctr allocator instance
    @param ptr pointer
    
    @returns index of a pointer in object list or `-1` if wasn't found in the object list
*/
static int64_t vt_mallocator_obj_list_find(const vt_mallocator_t *const alloctr, const void *const ptr) {
    // check for invalid input
    VT_DEBUG_ASSERT(alloctr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(alloctr->obj_list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(ptr != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // find element
    const size_t len = alloctr->obj_list_len;
    for (size_t i = 0; i < len; i++) {
        if (alloctr->obj_list[i].ptr == ptr) {
            return i;
        }
    }

    return -1;
}


