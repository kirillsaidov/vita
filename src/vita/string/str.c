#include "vita/string/str.h"

str_t str(const char* s) {
    if(is_null(s)) {
        s = " ";
    }

    return (str_t) {
        .ptr = (char*)s,
        .len = strlen(s),
		.capacity = 0
    };
}

const char* const as_cstr(str_t s) {
    return s.ptr;
}















