#ifndef VITA_STR
#define VITA_STR

/* VITA_STR MODULE (string : assign any value, but cannot change individual chars)
    - str
    - as_cstr
*/

// std includes
#include <string.h>

// str struct
typedef struct Str {
    char* buf;
    size_t len;
} str_t;

/** str ==> creates a new string with fixed length
    params:
        const char* s
    returns:
         str_t (fixed-length string)
*/
extern str_t str(const char* s);

/** as_cstr ==> returns string buffer to str_t
    params:
        const strt_t s
    returns:
    const char* (buffer to the begining of str_t)
*/
extern const char* as_cstr(const str_t s);

#endif // VITA_STR



















