#ifndef VITA_DEBUG_H
#define VITA_DEBUG_H

/** VITA_DEBUG MODULE
 * Macros
    - DEBUG_ASSERT

 * Functions
	- debug_assert
*/

#include "../core/core.h"

// macros
#ifndef NDEBUG
	#define DEBUG_ASSERT(expr, ...) debug_assert(expr, __FILE__, __LINE__, __VA_ARGS__)
#else
	#define DEBUG_ASSERT(expr, ...)
#endif

extern void debug_assert(const bool expr, const char *const file, const int32_t line, const char *const fmt, ...);

#endif // VITA_DEBUG_H
