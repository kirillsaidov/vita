#include <assert.h>
#include "vita/container/vec.h"
#include "vita/container/span.h"

int32_t main(void) {
    char buffer[512] = "1234567890 hello, world 1234567890 hello, world";

    // create span from buffer
    vt_span_t span = vt_span_from(buffer, 47, sizeof(char));
    assert(((char*)vt_span_get(span, 0))[0] == '1');
    assert(((char*)vt_span_get(span, 1))[0] == '2');
    assert(((char*)vt_span_get(span, 2))[0] == '3');

    // modify span
    vt_span_setu8(span, 'X', 0);
    assert(buffer[0] == 'X');
    assert(((char*)vt_span_get(span, 0))[0] == 'X');

    // create span from vec
    vt_vec_t *v = vt_vec_create(10, sizeof(int32_t), NULL);
    {   
        // init
        VT_FOREACH(i, 0, vt_vec_capacity(v)) {
            vt_vec_push_backi32(v, i);
        }
        assert(vt_vec_geti32(v, 3) == 3);

        // create span
        vt_span_t span2 = vt_span_from_to(vt_array_head(v), 2, 5, vt_array_elsize(v));
        assert(vt_span_len(span2) == 3);
        assert(vt_span_geti32(span2, 0) == 2);
        assert(vt_span_geti32(span2, 1) == 3);
        assert(vt_span_geti32(span2, 2) == 4);
    }
    vt_vec_destroy(v);

    return 0;
}


