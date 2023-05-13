#include <assert.h>
#include "../../inc/vita/datetime/time.h"

int32_t main(void) {
    char timebuf[VT_TIME_BUFFER_SIZE] = {};
    vt_time_get_now_as_text(timebuf, VT_TIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    vt_time_get_now_as_text_pretty(timebuf, VT_TIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    // vt_time_get_now_as_text_pretty(timebuf, VT_TIME_BUFFER_SIZE);
    // VT_DEBUG_PRINTF("%s\n", timebuf);

    return 0;
}


