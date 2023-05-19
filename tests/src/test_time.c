#include <assert.h>
#include "../../inc/vita/time/datetime.h"

int32_t main(void) {
    char timebuf[VT_DATETIME_BUFFER_SIZE] = {};
    vt_datetime_get_now_as_text(timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    vt_datetime_get_now_as_text_pretty(timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    // create custom datetime object
    const struct VitaDateTime vdt = vt_datetime_create(2023, 1, 1, 11, 24, 56, -1, -1);
    assert(vdt.year == 2023);
    assert(vdt.month == 1);
    assert(vdt.month_day == 1);
    assert(vdt.hour == 11);
    assert(vdt.minute == 24);
    assert(vdt.second == 56);
    assert(vdt.week_day == 7);
    assert(vdt.year_day == 1);

    vt_datetime_to_text(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    vt_datetime_to_text_pretty(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);

    // check week day
    assert(vt_datetime_find_week_day((const struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 15}) == 1);
    assert(vt_datetime_find_week_day((struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 16}) == 2);
    assert(vt_datetime_find_week_day((struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 17}) == 3);
    assert(vt_datetime_find_week_day((struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 18}) == 4);
    assert(vt_datetime_find_week_day((struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 19}) == 5);
    assert(vt_datetime_find_week_day((struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 20}) == 6);
    assert(vt_datetime_find_week_day((struct VitaDateTime) {.year = 2023, .month = 5, .month_day = 21}) == 7);

    // test other functions
    assert(vt_datetime_find_year_day(vdt) == 1);
    assert(vt_datetime_find_week_day(vdt) == 7);
    assert(vt_datetime_find_days_in_month(vdt) == 31);
    assert(vt_datetime_find_days_in_year(vdt) == 365);
    assert(vt_datetime_is_leap_year(vdt) == false);

    // ****** test datetime conversion from string ****** //
    struct VitaDateTime vdt_simple = vt_datetime_from_text("2023-05-19 20:31:01");
    assert(vdt_simple.year == 2023);
    assert(vdt_simple.month == 5);
    assert(vdt_simple.month_day == 19);
    assert(vdt_simple.hour == 20);
    assert(vdt_simple.minute == 31);
    assert(vdt_simple.second == 1);

    struct VitaDateTime vdt_iso = vt_datetime_from_text_iso("20230519T203101");
    assert(vdt_iso.year == 2023);
    assert(vdt_iso.month == 5);
    assert(vdt_iso.month_day == 19);
    assert(vdt_iso.hour == 20);
    assert(vdt_iso.minute == 31);
    assert(vdt_iso.second == 1);

    struct VitaDateTime vdt_iso_ext = vt_datetime_from_text_iso_ext("2023-05-19T20:31:01");
    assert(vdt_iso_ext.year == 2023);
    assert(vdt_iso_ext.month == 5);
    assert(vdt_iso_ext.month_day == 19);
    assert(vdt_iso_ext.hour == 20);
    assert(vdt_iso_ext.minute == 31);
    assert(vdt_iso_ext.second == 1);

    return 0;
}


