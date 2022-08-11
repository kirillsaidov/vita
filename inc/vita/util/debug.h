#ifndef VITA_DEBUG_H
#define VITA_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - DEBUG_ASSERT

 * Functions
    - debug_assert
*/

#include "../core/core.h"

// memory handler instance
typedef struct DebugMemoryHandler debug_mh_t;

#ifndef NDEBUG
    #define DEBUG_ASSERT(expr, ...) debug_assert(expr, __FILE__, __LINE__, __VA_ARGS__)
    #define DEBUG_ASSERT2(expr, file, line, ...) debug_assert(expr, file, line, __VA_ARGS__)
    #define DEBUG_PRINT(fmt, ...) debug_printf(fmt, __VA_ARGS__)
    
    // memory management
    #define DEBUG_MALLOC(bytes) debug_mh_malloc(debug_mh_handler_default_get_handler(), bytes, __FILE__, __LINE__)
    #define DEBUG_CALLOC(bytes) debug_mh_calloc(debug_mh_handler_default_get_handler(), bytes, __FILE__, __LINE__)
    #define DEBUG_REALLOC(ptr, bytes) debug_mh_realloc(debug_mh_handler_default_get_handler(), ptr, bytes, __FILE__, __LINE__)
    #define DEBUG_FREE(ptr) debug_mh_free(debug_mh_handler_default_get_handler(), ptr, __FILE__, __LINE__)
    #define DEBUG_SHUTDOWN debug_mh_handler_default_destroy()
#else
    #define DEBUG_ASSERT(expr, ...)
    #define DEBUG_ASSERT2(expr, file, line, ...)
    #define DEBUG_PRINT(fmt, ...)

    // memory management
    #define DEBUG_MALLOC(bytes) malloc(bytes)
    #define DEBUG_CALLOC(bytes) calloc(1, bytes)
    #define DEBUG_REALLOC(ptr, bytes) realloc(ptr, bytes)
    #define DEBUG_FREE(ptr) free(ptr)
#endif

/**
Asserts an expression and exits upon its evaluation to false

Params:
    expr = expression to test
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)
    fmt = formatting
    ... = additional arguments
*/
extern void debug_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...);

/**
Debug info print

Params:
    fmt = formatting
    ... = additional arguments
*/
extern void debug_printf(const char *const fmt, ...);

/* ------------------- MH HANDLER INIT FUNCTIONS -------------------- */

/**
Creates a mh handler

Returns: handler pointer, `NULL` upon failure
*/
extern debug_mh_t *debug_mh_handler_create(void);

/**
Frees mh handler

Params:
    mh = memory handler instance
*/
extern void debug_mh_handler_destroy(debug_mh_t *mh);

/**
Checks if mh was initialized

Params:
    mh = memory handler instance

Returns: true if it was initialized
*/
extern bool debug_mh_handler_is_init(const debug_mh_t *const mh);

/**
Creates the default internal mh handler
*/
extern void debug_mh_handler_default_create(void);

/**
Frees the default internal mh handler
*/
extern void debug_mh_handler_default_destroy(void);

/**
Returns the default internal mh handler instance

Returns: handler pointer
*/
extern debug_mh_t *debug_mh_handler_default_get_handler(void);

/* ---------------- MEMORY MANAGEMENT FUNCTIONS ----------------- */

/**
Allocates memory like malloc

Params:
    mh = memory handler instance
    bytes = memory size in bytes
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)

Returns: pointer to allocated block of memory, `NULL` upon failure
*/
extern void *debug_mh_malloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line);

/**
Allocates memory like calloc

Params:
    mh = memory handler instance
    bytes = memory size in bytes
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)

Returns: pointer to allocated block of memory, `NULL` upon failure
*/
extern void *debug_mh_calloc(debug_mh_t *const mh, const size_t bytes, const char *const file, const int32_t line);

/**
Rellocates memory like realloc

Params:
    mh = memory handler instance
    ptr = pointer to current memory
    bytes = memory size in bytes
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)

Returns: pointer to allocated block of memory, `NULL` upon failure

Note:
    It's meant to work with mh instance. 
    If ptr is not found the mh cache, then nothing happens.
*/
extern void *debug_mh_realloc(debug_mh_t *const mh, void *ptr, const size_t bytes, const char *const file, const int32_t line);

/**
Frees memory like free

Params:
    mh = memory handler instance
    ptr = pointer to current memory
    file = source file name (__FILE__) from where the logger has been called
    line = source file line (__LINE__)

Note:
    It's meant to work with mh instance. 
    If ptr is not found the mh cache, then nothing happens.
*/
extern void debug_mh_free(debug_mh_t *const mh, void *ptr, const char *const file, const int32_t line);

/**
Adds allocated memory to memory handler cache

Params:
    mh = memory handler instance
    ptr = pointer to current memory
    bytes = memory size in bytes
*/
extern void debug_mh_add(debug_mh_t *const mh, const void *const ptr, const size_t bytes);

/**
Removes allocated memory from memory handler cache

Params:
    mh = memory handler instance
    ptr = pointer to current memory
*/
extern size_t debug_mh_remove(debug_mh_t *const mh, const void *const ptr);


#endif // VITA_DEBUG_H
