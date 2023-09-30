#include <assert.h>
#include "../../inc/vita/container/str.h"
#include "../../inc/vita/time/datetime.h"

int32_t main(void) {
    char timebuf[VT_DATETIME_BUFFER_SIZE] = {0};
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

    // ****** test datetime conversion TO string ****** //
    vt_datetime_to_text(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    assert(vt_str_equals_z(timebuf, "2023-01-01 11:24:56"));

    vt_datetime_to_text_iso(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    assert(vt_str_equals_z(timebuf, "20230101T112456"));

    vt_datetime_to_text_iso_ext(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    assert(vt_str_equals_z(timebuf, "2023-01-01T11:24:56"));
    vt_memset(timebuf, '\0', VT_DATETIME_BUFFER_SIZE);

    vt_datetime_to_text_pretty(vdt, timebuf, VT_DATETIME_BUFFER_SIZE);
    VT_DEBUG_PRINTF("%s\n", timebuf);
    assert(vt_str_equals_z(timebuf, "Sun Jan  1 11:24:56 2023"));

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

    // ****** test datetime conversion FROM string ****** //
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

    assert(vt_datetime_is_valid_text("2023-12-27 23:12:02") == true);
    assert(vt_datetime_is_valid_text_iso("20231227T231202") == true);
    assert(vt_datetime_is_valid_text_iso_ext("2023-12-27T23:12:02") == true);
    assert(vt_datetime_is_valid_text("2023-13-27 23:1 :02") == false);
    assert(vt_datetime_is_valid_text("2023-1d-27 22:12:02") == false);
    assert(vt_datetime_is_valid_text("2023-27 22:12:02   ") == false);
    assert(vt_datetime_is_valid_text("a023-27 22:12:02   ") == false);

    // time arithmetic
    struct VitaDateTime vdt2 = vt_datetime_from_text("2023-05-19 20:31:01");
    const time_t secs_in_one_day = 86400;
    const time_t secs_vdt2 = vt_datetime_to_secs(vdt2) + 6 * secs_in_one_day; // add 6 days
    struct VitaDateTime vdt2_new = vt_datetime_from_secs(secs_vdt2);
    vt_datetime_to_text(vdt2_new, timebuf, VT_DATETIME_BUFFER_SIZE);
    assert(vt_str_equals_z(timebuf, "2023-05-25 20:31:01"));

    // add 2 days and 9 minutes, substract 2 hours and 1 second
    struct VitaDateTime vdt3 = vt_datetime_op(vdt2, (struct VitaDateTime) { .month_day = 6, .hour = -2, .minute = 9, .second = -1 });
    vt_datetime_to_text(vdt3, timebuf, VT_DATETIME_BUFFER_SIZE);
    assert(vt_str_equals_z(timebuf, "2023-05-25 18:40:00"));

    struct VitaDateTime vdt4 = vt_datetime_from_text("2023-05-25 18:40:00");
    struct VitaDateTime vdt5 = vt_datetime_from_text("2023-05-19 20:31:01");
    assert(vt_datetime_diff_days(vdt3, vdt2) == 5);

    struct VitaDateTime vdt_diff = vt_datetime_diff(vdt4, vdt5);
    assert(vdt_diff.second == 58);
    assert(vdt_diff.minute == 8);
    assert(vdt_diff.hour == 22);
    assert(vdt_diff.month_day == 5);

    printf("====> expri: %ld\n", vt_datetime_to_secs(vt_datetime_from_text("2023-09-25 13:30:00")));


    return 0;
}


