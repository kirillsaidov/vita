# Datetime
This chapter describes how to use the functionality provided by the [`datetime`](../../inc/vita/time/datetime.h) module.

## Contents
1. [Get current time](page6.md#get-current-time)
2. [Create custom datetime](page6.md#create-custom-datetime)
3. [Convert datetime to string](page6.md#convert-datetime-to-string)
4. [Convert string to datetime](page6.md#convert-string-to-datetime)
5. [Datetime arithmetic](page6.md#datetime-arithmetic)
6. [Additional functionality](page6.md#additional-functionality)

### Get current time

```c
#include "vita/time/datetime.h"

// save current time to datetime object
struct VitaDateTime time_now = vt_datetime_get_now();
assert(time_now.year == 2023);
assert(time_now.month == 1);
assert(time_now.month_day == 1);
assert(time_now.hour == 11);
assert(time_now.minute == 24);
assert(time_now.second == 56);
assert(time_now.week_day == 7);
assert(time_now.year_day == 1);

// save time to buffer
char timebuf[VT_DATETIME_BUFFER_SIZE] = {0};
vt_datetime_get_now_as_text(timebuf, VT_DATETIME_BUFFER_SIZE);        // "2023-06-30 12:11:27"
vt_datetime_get_now_as_text_pretty(timebuf, VT_DATETIME_BUFFER_SIZE); // "Fri Jun 30 12:11:27 2023"
```

### Create custom datetime

```c
// create custom datetime object
const struct VitaDateTime custom_time = vt_datetime_create(2023, 1, 1, 11, 24, 56, -1, -1);
assert(custom_time.year == 2023);
assert(custom_time.month == 1);
assert(custom_time.month_day == 1);
assert(custom_time.hour == 11);
assert(custom_time.minute == 24);
assert(custom_time.second == 56);
assert(custom_time.week_day == 7); // calculates automatically if -1 is speicified
assert(custom_time.year_day == 1); // ditto
```

### Convert datetime to string

```c
// define time buffer
char timebuf[VT_DATETIME_BUFFER_SIZE] = {0};

// convert datetime object to its string representation
vt_datetime_to_text(custom_time, timebuf, VT_DATETIME_BUFFER_SIZE);
assert(vt_str_equals_z(timebuf, "2023-01-01 11:24:56"));

vt_datetime_to_text_iso(custom_time, timebuf, VT_DATETIME_BUFFER_SIZE);
assert(vt_str_equals_z(timebuf, "20230101T112456"));

vt_datetime_to_text_iso_ext(custom_time, timebuf, VT_DATETIME_BUFFER_SIZE);
assert(vt_str_equals_z(timebuf, "2023-01-01T11:24:56"));

vt_datetime_to_text_pretty(custom_time, timebuf, VT_DATETIME_BUFFER_SIZE);
assert(vt_str_equals_z(timebuf, "Sun Jan  1 11:24:56 2023"));
```

### Convert string to datetime

```c
// convert string time representation to datetime object
struct VitaDateTime time_simple = vt_datetime_from_text("2023-05-19 20:31:01");
struct VitaDateTime time_iso = vt_datetime_from_text_iso("20230519T203101");
struct VitaDateTime time_iso_ext = vt_datetime_from_text_iso_ext("2023-05-19T20:31:01");

// check
assert(time_iso_ext.year == 2023);
assert(time_iso_ext.month == 5);
assert(time_iso_ext.month_day == 19);
assert(time_iso_ext.hour == 20);
assert(time_iso_ext.minute == 31);
assert(time_iso_ext.second == 1);
```

### Datetime arithmetic

```c
// define datetime
struct VitaDateTime datetime = vt_datetime_from_text("2023-05-19 20:31:01");

// add 6 days and 9 minutes, substract 2 hours and 1 second
struct VitaDateTime shifted = vt_datetime_op(datetime, (struct VitaDateTime) { 
    .month_day = 6, 
    .hour = -2, 
    .minute = 9, 
    .second = -1 
});

// check
vt_datetime_to_text(shifted, timebuf, VT_DATETIME_BUFFER_SIZE);
assert(vt_str_equals_z(timebuf, "2023-05-25 18:40:00"));

// add 6 days (alternative)
const time_t secs_in_one_day = 86400;
const time_t secs_shifted = vt_datetime_to_secs(datetime) + 6 * secs_in_one_day;
shifted = vt_datetime_from_secs(secs_shifted);

// difference in secs, minutes, hours, days
struct VitaDateTime vdt4 = vt_datetime_from_text("2023-05-25 18:40:00");
struct VitaDateTime vdt5 = vt_datetime_from_text("2023-05-19 20:31:01");
assert(vt_datetime_diff_days(vdt3, vdt2) == 5); // vt_datetime_diff_{secs, hours, minutes, days}(lhs, rhs)

// difference
struct VitaDateTime diff = vt_datetime_diff(lhs, rhs);
assert(diff.second == 58);
assert(diff.minute == 8);
assert(diff.hour == 22);
assert(diff.month_day == 5);
```

### Additional functionality

```c
// check if datetime string is valid
assert(vt_datetime_is_valid_text("2023-12-27 23:12:02") == true);
assert(vt_datetime_is_valid_text_iso("20231227T231202") == true);
assert(vt_datetime_is_valid_text_iso_ext("2023-12-27T23:12:02") == true);
assert(vt_datetime_is_valid_text("2023-13-27 23:1 :02") == false);
assert(vt_datetime_is_valid_text("2023-DA-27 HU:12:02") == false);
assert(vt_datetime_is_valid_text("2023-27 22:12:02 lala") == false);

// find the lacking datetime information
const int16_t year_day = vt_datetime_find_year_day(vdt);
const int16_t week_day = vt_datetime_find_week_day(vdt);
const int16_t days_in_month = vt_datetime_find_days_in_month(vdt);
const int16_t days_in_year = vt_datetime_find_days_in_year(vdt);

// check if leap year
const bool is_leap_year = vt_datetime_is_leap_year(vdt);
```

For more usage examples, please refer to [test_datetime.c](../../tests/src/test_datetime.c) files.

**[ [Back](page5.md) | [Next](page7.md) ]**
