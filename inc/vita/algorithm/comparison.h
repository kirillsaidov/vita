#ifndef VITA_COMPARISON_H
#define VITA_COMPARISON_H

/** VITA_COMPARISON MODULE
    - //
*/

#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, l, h) (MAX(l, MIN(x, h)))

#endif // VITA_COMPARISON_H
