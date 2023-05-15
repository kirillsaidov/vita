#include <assert.h>
#include "../../inc/vita/time/datetime.h"

int32_t main(void) {
    char timebuf[VT_DATETIME_BUFFER_SIZE] = {};
    vt_datetime_get_now_as_text(timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    vt_datetime_get_now_as_text_pretty(timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    // vt_datetime_get_now_as_text_pretty(timebuf, VT_DATETIME_BUFFER_SIZE);
    // VT_DEBUG_PRINTF("%s\n", timebuf);

    return 0;
}


