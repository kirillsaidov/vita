#ifndef VITA_COMPARISON_H
#define VITA_COMPARISON_H

/** VITA_COMPARISON MODULE
 * Macros:
    - VT_MAX
    - VT_MIN
    - VT_CLAMP
*/

#include <stdint.h>

#define VT_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define VT_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define VT_CLAMP(x, l, h) (VT_MAX(l, VT_MIN(x, h)))

extern void cmp_test(void);

#endif // VITA_COMPARISON_H
