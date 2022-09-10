#ifndef VITA_COMPARISON_H
#define VITA_COMPARISON_H

/** VITA_COMPARISON MODULE
 * Macros:
    - MAX
    - MIN
    - CLAMP
*/

#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, l, h) (MAX(l, MIN(x, h)))

extern void cmp_test(void);

#endif // VITA_COMPARISON_H
