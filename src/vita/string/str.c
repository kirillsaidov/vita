#include "vita/string/str.h"

str_t str(const char* s) {
    return (str_t) {
        .buf = (char*)s,
        .len = strlen(s),
		.capacity = 0
    };
}

const char* as_cstr(str_t s) {
    return s.buf;
}















