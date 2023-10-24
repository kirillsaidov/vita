#include "../../inc/vita/core/conv.h"
#include "../../inc/vita/container/str.h"

int32_t main(void) {
    assert(
        (int32_t)vt_conv_str_to_i64("123") == 123
    );
    assert(
        (int32_t)vt_conv_str_to_i64("00123") == 123
    );
    assert(
        (uint64_t)vt_conv_str_to_u64("985454325454") == 985454325454
    );
    assert(
        vt_conv_str_to_d("985454325454.22548") == (double)985454325454.22548
    );

    char buf[36];
    vt_conv_i64_to_str(buf, 36, 123456);
    assert(vt_str_equals_z(buf, "123456"));

    vt_conv_d_to_str(buf, 36, 123456.123456);
    assert(vt_str_equals_z(buf, "123456.123456"));

    return 0;
}





