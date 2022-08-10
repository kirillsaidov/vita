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
	//
#else
	//
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

extern debug_mh_t *debug_mh_handler_create(void);
extern void debug_mh_handler_destroy(debug_mh_t *mh);
extern bool debug_mh_handler_is_init(const debug_mh_t *const mh);

extern void debug_mh_handler_default_create(void);
extern void debug_mh_handler_default_destroy(void);
extern debug_mh_t *debug_mh_handler_default_get_handler(void);


#endif // VITA_DEBUG_H
