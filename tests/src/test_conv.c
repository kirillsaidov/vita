#include "../../inc/vita/core/conv.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {
    assert(
        (int32_t)vt_conv_str_to_int64("123") == 123
    );
    assert(
        (uint64_t)vt_conv_str_to_uint64("985454325454") == 985454325454
    );
    assert(
        vt_conv_str_to_double("985454325454.22548") == (double)985454325454.22548
    );

    char buf[36];
    vt_conv_int64_to_str(buf, 36, 123456);
    assert(vt_str_equals(buf, "123456"));

    vt_conv_double_to_str(buf, 36, 123456.123456);
    assert(vt_str_equals(buf, "123456.123456"));

    return 0;
}





