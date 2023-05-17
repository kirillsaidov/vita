#include <assert.h>
#include "../../inc/vita/time/datetime.h"

int32_t main(void) {
    char timebuf[VT_DATETIME_BUFFER_SIZE] = {};
    vt_datetime_get_now_as_text(timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    vt_datetime_get_now_as_text_pretty(timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    const struct VitaDateTime vdt = vt_datetime_create(2023, 1, 1, 23, 0, 0, -1, -1);
    vt_datetime_to_text(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    vt_datetime_to_text_pretty(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    assert(vt_datetime_find_year_day(vdt) == 1);
    assert(vt_datetime_find_week_day(vdt) == 7);
    assert(vt_datetime_find_days_in_month(vdt) == 31);
    assert(vt_datetime_find_days_in_year(vdt) == 365);
    assert(vt_datetime_is_leap_year(vdt) == false);

    // vt_datetime_get_now_as_text_pretty(timebuf, VT_DATETIME_BUFFER_SIZE);
    // VT_DEBUG_PRINTF("%s\n", timebuf);

    return 0;
}


