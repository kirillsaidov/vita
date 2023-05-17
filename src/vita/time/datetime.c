#include "vita/time/datetime.h"

/* Converts from `tm` to `VitaDateTime` datetime format
    @param stm tm struct
    @returns struct VitaDateTime
*/
static struct VitaDateTime vt_datetime_tm_to_vdt(struct tm stm);

/* Converts from `VitaDateTime` to `tm` datetime format
    @param vdt VitaDateTime struct
    @returns struct tm
*/
static struct tm vt_datetime_vdt_to_tm(struct VitaDateTime vdt);

struct VitaDateTime vt_datetime_get_now(void) {
    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    return vt_datetime_tm_to_vdt(*stm);
}

void vt_datetime_get_now_as_text(char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // get time
    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    timebuf[strftime(timebuf, VT_DATETIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", stm)] = '\0';
}

void vt_datetime_get_now_as_text_pretty(char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // get time
    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    asctime_r(stm, timebuf);
}

struct VitaDateTime vt_datetime_create(
    const int16_t year, const int16_t month, const int16_t month_day,
    const int16_t hour, const int16_t minute, const int16_t second,
    const int16_t week_day, const int16_t year_day
) {
    struct VitaDateTime vdt = {
        .year = year < 0 ? VT_DATETIME_MIN_YEAR_RANGE : year,
        .month = month < 1 ? 1 : (month > 12 ? 12 : month),
        .month_day = month_day < 1 ? 1 : (month_day > 31 ? vt_datetime_find_days_in_month(
            (struct VitaDateTime) {.month = month, .year = year}
        ) : month_day),
        .hour = hour < 0 ? 0 : (hour > 23 ? 23 : hour),
        .minute = minute < 0 ? 0 : (minute > 59 ? 59 : minute),
        .second = second < 0 ? 0 : (second > 60 ? 60 : second),
        .week_day = week_day < 1 ? 1 : (week_day > 7 ? 7 : week_day),
        .year_day = year_day < 1 ? 1 : (year_day > 366 ? vt_datetime_find_days_in_year(
            (struct VitaDateTime) {.year = year}
        ) : year_day)
    };

    // calculate wday
    if(vdt.week_day < 0) {
        vdt.week_day = vt_datetime_find_week_day(vdt);
    }

    // calculate yday
    if(vdt.year_day < 0) {
        vdt.year_day = vt_datetime_find_year_day(vdt);
    }

    return vdt;
}

void vt_datetime_to_text(const struct VitaDateTime vdt, char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // get time
    const struct tm stm = vt_datetime_vdt_to_tm(vdt);
    timebuf[strftime(timebuf, VT_DATETIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", &stm)] = '\0';
}

void vt_datetime_to_text_pretty(const struct VitaDateTime vdt, char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // get time
    const struct tm stm = vt_datetime_vdt_to_tm(vdt);
    asctime_r(&stm, timebuf);
}

int16_t vt_datetime_find_year_day(const struct VitaDateTime vdt) {
    struct tm stm = vt_datetime_vdt_to_tm(vdt);
    time_t ret = mktime(&stm);
    if(ret < 0) {
        VT_DEBUG_PRINTF("%s: Invalid datetime format.\n");
        return -1;
    }

    return stm.tm_yday + 1;
}

int16_t vt_datetime_find_week_day(const struct VitaDateTime vdt) {
    struct tm stm = vt_datetime_vdt_to_tm(vdt);
    time_t ret = mktime(&stm);
    if(ret < 0) {
        VT_DEBUG_PRINTF("%s: Invalid datetime format.\n");
        return -1;
    }

    return stm.tm_wday == 0 ? 7 : stm.tm_wday;
}

int16_t vt_datetime_find_days_in_month(const struct VitaDateTime vdt) {
    // check for invalid month range
    const int16_t month = vdt.month < 1 ? 1 : (vdt.month > 12 ? 12 : vdt.month);
    const int16_t year = vdt.year < 0 ? VT_DATETIME_MIN_YEAR_RANGE : vdt.year;

    // check for 31 Days
    if (month == 1 || month == 3 || month == 5 ||
        month == 7 || month == 8 || month == 10 ||
        month == 12) {
        return 31;
    } 
    // check for 30 Days
    else if (month == 4 || month == 6 ||
             month == 9 || month == 11) {
        return 30;
    }
    // check for 28/29 Days, in this case only month == 2 is left
    else {
        return (year > 0 && vt_datetime_is_leap_year(vdt)) ? 29 : 28;
    }
}

int16_t vt_datetime_find_days_in_year(const struct VitaDateTime vdt) {
    return vt_datetime_is_leap_year(vdt) ? 366 : 365;
}

bool vt_datetime_is_leap_year(const struct VitaDateTime vdt) {
    const int16_t year = vdt.year < 0 ? VT_DATETIME_MIN_YEAR_RANGE : vdt.year;
    return (year % 4 == 0);
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

static struct VitaDateTime vt_datetime_tm_to_vdt(struct tm stm) {
    /** struct tm variables range:
        year        years passed since VT_DATETIME_MIN_YEAR_RANGE | + VT_DATETIME_MIN_YEAR_RANGE = YYYY
        month       [0, 11] | + 1 = [1, 12]
        month_day   [1, 31]

        hour        [0, 23]
        minute      [0, 59]
        second      [0, 60]

        week_day    [0, 6]   Sunday(0), Monday(1), ..., Saturday(6) | + 1 = [1, 7]
        year_day    [0, 365] days since January 1 | + 1 = [1, 366]
    */
    return (struct VitaDateTime) {
        .year = stm.tm_year + VT_DATETIME_MIN_YEAR_RANGE,
        .month = stm.tm_mon + 1,
        .month_day = stm.tm_mday,
        .hour = stm.tm_hour,
        .minute = stm.tm_min,
        .second = stm.tm_sec,
        .week_day = (stm.tm_wday == 0) ? 7 : stm.tm_wday,
        .year_day = stm.tm_yday + 1
    };
}

static struct tm vt_datetime_vdt_to_tm(struct VitaDateTime vdt) {
    return (struct tm) {
        .tm_year = vdt.year - VT_DATETIME_MIN_YEAR_RANGE,
        .tm_mon = vdt.month - 1,
        .tm_mday = vdt.month_day,
        .tm_hour = vdt.hour, 
        .tm_min = vdt.minute,
        .tm_sec = vdt.second,
        .tm_wday = (vdt.week_day == 7) ? 0 : vdt.week_day,
        .tm_yday = vdt.year_day - 1,
        .tm_isdst = -1
    };
}


