#include "vita/time/datetime.h"

static struct VitaDateTime vt_datetime_tm_to_vdt(const struct tm stm);
static struct tm vt_datetime_vdt_to_tm(const struct VitaDateTime vdt);
static void vt_datetime_to_text_fmt(const struct VitaDateTime vdt, char *timebuf, const size_t len, const char *fmt);
static struct VitaDateTime vt_datetime_from_text_fmt(const char *timebuf, const char *fmt);
static bool vt_datetime_is_valid_fmt(const char *const timebuf, const char *const fmt, const size_t len);

struct VitaDateTime vt_datetime_get_now(void) {
    const time_t t = time(NULL);
    const struct tm stm = *localtime(&t);
    return vt_datetime_tm_to_vdt(stm);
}

void vt_datetime_get_now_as_text(char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // get time
    const time_t t = time(NULL);
    const struct tm stm = *localtime(&t);
    timebuf[strftime(timebuf, len, "%Y-%m-%d %H:%M:%S", &stm)] = '\0';
}

void vt_datetime_get_now_as_text_pretty(char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // get time
    const time_t t = time(NULL);
    const struct tm stm = *localtime(&t);
    const struct VitaDateTime vdt = vt_datetime_tm_to_vdt(stm);
    vt_datetime_to_text_pretty(vdt, timebuf, len);
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
        .week_day = (week_day < 1 || week_day > 7) ? -1 : week_day,
        .year_day = (year_day < 1 || year_day > 366) ? -1 : year_day
    };

    // calculate wday
    if (vdt.week_day < 0) {
        vdt.week_day = vt_datetime_find_week_day(vdt);
    }

    // calculate yday
    if (vdt.year_day < 0) {
        vdt.year_day = vt_datetime_find_year_day(vdt);
    }

    return vdt;
}

void vt_datetime_to_text(const struct VitaDateTime vdt, char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    vt_datetime_to_text_fmt(vdt, timebuf, len, "%Y-%m-%d %H:%M:%S");
}

void vt_datetime_to_text_iso(const struct VitaDateTime vdt, char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    vt_datetime_to_text_fmt(vdt, timebuf, len, "%Y%m%dT%H%M%S");
}

void vt_datetime_to_text_iso_ext(const struct VitaDateTime vdt, char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    vt_datetime_to_text_fmt(vdt, timebuf, len, "%Y-%m-%dT%H:%M:%S");
}

void vt_datetime_to_text_pretty(const struct VitaDateTime vdt, char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // get time
    const struct tm stm = vt_datetime_vdt_to_tm(vdt);
    asctime(timebuf, len, &stm);

    // set day digit to blank space when < 10, e.g.: 'Jan 01' to 'Jan 1'
    if (timebuf[8] == '0') {
        timebuf[8] = ' ';
    }
    timebuf[len-1] = '\0';
    
    // remove '\n' that's appended at the end
    if (timebuf[VT_DATETIME_BUFFER_SIZE-1] == '\n') {
        timebuf[VT_DATETIME_BUFFER_SIZE-1] = '\0';
    } else {
        timebuf[VT_DATETIME_BUFFER_SIZE-2] = '\0'; 
    }
}

struct VitaDateTime vt_datetime_from_text(const char *timebuf) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(
        vt_datetime_is_valid_text(timebuf), 
        "Invalid timestamp format! Expected \'%s\', but recieved \'%s\'.\n", 
        "YYYY-MM-DD HH:MM:SS", 
        timebuf
    );

    return vt_datetime_from_text_fmt(timebuf, "%4d-%2d-%2d %2d:%2d:%2d");
}

struct VitaDateTime vt_datetime_from_text_iso(const char *timebuf) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(
        vt_datetime_is_valid_text_iso(timebuf), 
        "Invalid timestamp format! Expected \'%s\', but recieved \'%s\'.\n", 
        "YYYYMMDDTHHMMSS", 
        timebuf
    );

    return vt_datetime_from_text_fmt(timebuf, "%4d%2d%2dT%2d%2d%2d");
}

struct VitaDateTime vt_datetime_from_text_iso_ext(const char *timebuf) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(
        vt_datetime_is_valid_text_iso_ext(timebuf), 
        "Invalid timestamp format! Expected \'%s\', but recieved \'%s\'.\n", 
        "YYYY-MM-DDTHH:MM:SS", 
        timebuf
    );

    return vt_datetime_from_text_fmt(timebuf, "%4d-%2d-%2dT%2d:%2d:%2d");
}

int16_t vt_datetime_find_year_day(const struct VitaDateTime vdt) {
    struct tm stm = vt_datetime_vdt_to_tm(vdt);
    time_t ret = mktime(&stm);
    if (ret < 0) {
        VT_DEBUG_PRINTF("%s: Invalid datetime format.\n");
        return -1;
    }

    return stm.tm_yday + 1;
}

int16_t vt_datetime_find_week_day(const struct VitaDateTime vdt) {
    struct tm stm = vt_datetime_vdt_to_tm(vdt);
    time_t ret = mktime(&stm);
    if (ret < 0) {
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

bool vt_datetime_is_valid_text(const char *const timebuf) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const char fmt[] = "YYYY-MM-DD HH:MM:SS";
    return vt_datetime_is_valid_fmt(timebuf, fmt, sizeof(fmt));
}

bool vt_datetime_is_valid_text_iso(const char *const timebuf) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const char fmt[] = "YYYYMMDDTHHMMSS";
    return vt_datetime_is_valid_fmt(timebuf, fmt, sizeof(fmt));
}

bool vt_datetime_is_valid_text_iso_ext(const char *const timebuf) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    const char fmt[] = "YYYY-MM-DDTHH:MM:DD";
    return vt_datetime_is_valid_fmt(timebuf, fmt, sizeof(fmt));
}

time_t vt_datetime_to_secs(const struct VitaDateTime vdt) {
    const struct tm stm = vt_datetime_vdt_to_tm(vdt);
    return mktime((struct tm*)&stm);
}

struct VitaDateTime vt_datetime_from_secs(const time_t secs) {
    const struct tm stm = *localtime(&secs);
    return vt_datetime_tm_to_vdt(stm);
}

struct VitaDateTime vt_datetime_op(const struct VitaDateTime vdt, const struct VitaDateTime delta){
    struct tm stm = vt_datetime_vdt_to_tm(vdt);

    // add delta
    stm.tm_year += delta.year;
    stm.tm_mon += delta.month;
    stm.tm_mday += delta.month_day;
    stm.tm_hour += delta.hour;
    stm.tm_min += delta.minute;
    stm.tm_sec += delta.second;

    // correct for delta
    mktime(&stm);

    return vt_datetime_tm_to_vdt(stm);
}

time_t vt_datetime_diff_secs(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2) {
    return vt_datetime_to_secs(vdt1) - vt_datetime_to_secs(vdt2);
}

time_t vt_datetime_diff_minutes(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2) {
    return vt_datetime_diff_secs(vdt1, vdt2) / 60;
}

time_t vt_datetime_diff_hours(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2) {
    return vt_datetime_diff_minutes(vdt1, vdt2) / 60;
}

time_t vt_datetime_diff_days(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2) {
    return vt_datetime_diff_hours(vdt1, vdt2) / 24;
}

struct VitaDateTime vt_datetime_diff(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2) {
    // find datetime diff
    const time_t diff_secs = vt_datetime_diff_secs(vdt1, vdt2);

    // calculate delta
    const float secs_in_one_day = 86400;
    const time_t days = diff_secs/secs_in_one_day;
    const time_t hours = (diff_secs/secs_in_one_day - days)*24;
    const time_t minutes = ((diff_secs/secs_in_one_day - days)*24 - hours)*60;
    const time_t secs = (((diff_secs/secs_in_one_day - days)*24 - hours)*60 - minutes)*60;

    return (struct VitaDateTime) {
        .second = secs,
        .minute = minutes,
        .hour = hours,
        .month_day = days
    };
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

/** Converts from `tm` to `VitaDateTime` datetime format
    @param stm tm struct
    @returns struct VitaDateTime
*/
static struct VitaDateTime vt_datetime_tm_to_vdt(const struct tm stm) {
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

/** Converts from `VitaDateTime` to `tm` datetime format
    @param vdt VitaDateTime struct
    @returns struct tm
*/
static struct tm vt_datetime_vdt_to_tm(const struct VitaDateTime vdt) {
    return (struct tm) {
        .tm_year = vdt.year ? (vdt.year - VT_DATETIME_MIN_YEAR_RANGE) : VT_DATETIME_MIN_YEAR_RANGE,
        .tm_mon = vdt.month - 1,
        .tm_mday = vdt.month_day,
        .tm_hour = vdt.hour, 
        .tm_min = vdt.minute,
        .tm_sec = vdt.second,
        .tm_wday = (vdt.week_day == 7) ? 0 : vdt.week_day,
        .tm_yday = vdt.year_day - 1,
        .tm_isdst = 0
    };
}

/** Converts VitaDateTime to text
    @param vdt VitaDateTime struct
    @param timebuf to store the data
    @param len timebuf length
    @param fmt timestamp format

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
static void vt_datetime_to_text_fmt(const struct VitaDateTime vdt, char *timebuf, const size_t len, const char *fmt) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(len >= VT_DATETIME_BUFFER_SIZE, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // get time
    const struct tm stm = vt_datetime_vdt_to_tm(vdt);
    timebuf[strftime(timebuf, len, fmt, &stm)] = '\0';
}

/** Reads timestamp text
    @param timebuf to store the data
    @param fmt timestamp format

    @returns struct VitaDateTime
*/
static struct VitaDateTime vt_datetime_from_text_fmt(const char *timebuf, const char *fmt) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // convert to tm
    struct tm stm = {0};
    sscanf(timebuf, fmt, &stm.tm_year, &stm.tm_mon, &stm.tm_mday, &stm.tm_hour, &stm.tm_min, &stm.tm_sec);

    // adjust
    stm.tm_year -= VT_DATETIME_MIN_YEAR_RANGE;
    stm.tm_mon -= 1;
    stm.tm_isdst = 0;

    return vt_datetime_tm_to_vdt(stm);
}

/** Checks if timestamp is valid format
    @param timebuf timestamp
    @param fmt timestamp format with 0 in place of digits, see notes below
    @param len timestamp/fmt buffer size
    @returns true if valid

    @note checks if format is valid, does not check for correct values in timestamp itself.
    @note `fmt` must use 0 for digits and non-digit for separators, e.g.: "0000-00-00 00:00:00", "00000000T000000", "0000-00-00T00:00:00"
*/
static bool vt_datetime_is_valid_fmt(const char *const timebuf, const char *const fmt, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(fmt != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    for (size_t i = 0; i < len; i++) {
        if (fmt[i] != 'T' && isalpha(fmt[i]) && !isdigit(timebuf[i])) {
            return false;
        }
    }

    return true;
}

