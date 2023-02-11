#include "../../inc/vita/core/conv.h"

int32_t main(void) {
    int32_t val = (int32_t)vt_conv_str_to_int64("123");
    assert(val == 123);
}





