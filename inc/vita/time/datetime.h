#ifndef VITA_TIME_DATETIME_H
#define VITA_TIME_DATETIME_H

/** DATETIME MODULE 
 * This is a datetime handling module. By defaut it will always return the VitaDateTime struct.
 * If a function has `text` in its name, then it returns data in textual format.
    - vt_datetime_get_now
    - vt_datetime_get_now_as_text
    - vt_datetime_get_now_as_text_pretty
    - vt_datetime_create
    - vt_datetime_to_text
    - vt_datetime_to_text_iso
    - vt_datetime_to_text_iso_ext
    - vt_datetime_from_text
    - vt_datetime_from_text_iso
    - vt_datetime_from_text_iso_ext
    - vt_datetime_find_year_day
    - vt_datetime_find_week_day
    - vt_datetime_find_days_in_month
    - vt_datetime_find_days_in_year
    - vt_datetime_is_leap_year
    - vt_datetime_to_secs
    - vt_datetime_from_secs
    - vt_datetime_op
    - vt_datetime_diff_secs
    - vt_datetime_diff_minutes
    - vt_datetime_diff_hours
    - vt_datetime_diff_days
    - vt_datetime_diff
*/

#include "../core/core.h"
#include "../util/debug.h"

#if defined(_WIN32) || defined(_WIN64)
    #define asctime(timebuf, timebuf_size, tm) asctime_s(timebuf, timebuf_size, tm)
#else
    #define asctime(timebuf, timebuf_size, tm) asctime_r(tm, timebuf)
#endif

#define VT_DATETIME_BUFFER_SIZE 26
#define VT_DATETIME_MIN_YEAR_RANGE 1900

/** VitaDateTime structure
    Range:
        year        YYYY
        month       [1, 12]
        month_day   [1, 31]

        hour        [0, 23]
        minute      [0, 59]
        second      [0, 60]

        week_day    [1, 7]   Monday(1), Tuesday(2), ..., Sunday(7)
        year_day    [1, 366] January 1 (1st day), January 2 (2nd day), ..., December 31 (366th day)
*/
struct VitaDateTime {
    int16_t year, month, month_day;
    int16_t hour, minute, second;
    int16_t week_day, year_day;
};

/** Returns current datetime in VitaDateTime struct
    @returns struct VitaDateTime
*/
extern struct VitaDateTime vt_datetime_get_now(void);

/** Sets a timestamp "yyyy-mm-dd hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
extern void vt_datetime_get_now_as_text(char *timebuf, const size_t len);

/** Sets a prettyfied text representation of timestamp "year-month-day hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
extern void vt_datetime_get_now_as_text_pretty(char *timebuf, const size_t len);

/** Creates a VitaDateTime object
    @param year ditto
    @param month ditto
    @param month_day ditto
    @param hour ditto
    @param minute ditto
    @param second ditto
    @param week_day ditto
    @param year_day ditto

    @returns struct VitaDateTime

    @note passing -1 for `week_day` and `year_day` will auto fill from date.
    @note adjusts each argument to their normal ranges. See `struct VitaDateTime`.
*/
extern struct VitaDateTime vt_datetime_create(
    const int16_t year, const int16_t month, const int16_t month_day,
    const int16_t hour, const int16_t minute, const int16_t second,
    const int16_t week_day, const int16_t year_day
);

/** Converts VitaDateTime to simple text format "YYYY-MM-DD HH:MM:SS" and saves to timebuf
    @param timebuf to store the data
    @param len timebuf length

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
extern void vt_datetime_to_text(const struct VitaDateTime vdt, char *timebuf, const size_t len);

/** Converts VitaDateTime to ISO text format "YYYYMMDDTHHMMSS" and saves to timebuf
    @param timebuf to store the data
    @param len timebuf length

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
extern void vt_datetime_to_text_iso(const struct VitaDateTime vdt, char *timebuf, const size_t len);

/** Converts VitaDateTime to ISO Ext text format "YYYY-MM-DDTHH:MM:SS" and saves to timebuf
    @param timebuf to store the data
    @param len timebuf length

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
extern void vt_datetime_to_text_iso_ext(const struct VitaDateTime vdt, char *timebuf, const size_t len);

/** Converts VitaDateTime to prettyfied text format "year-month-day hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be at least VT_DATETIME_BUFFER_SIZE!
*/
extern void vt_datetime_to_text_pretty(const struct VitaDateTime vdt, char *timebuf, const size_t len);

/** Read simple timestamp format "YYYY-MM-DD HH:MM:SS" to VitaDateTime structure
    @param timebuf timestamp
    @returns struct VitaDateTime
*/
extern struct VitaDateTime vt_datetime_from_text(const char *timebuf);

/** Read ISO timestamp format "YYYYMMDDTHHMMSS" to VitaDateTime structure
    @param timebuf timestamp
    @returns struct VitaDateTime
*/
extern struct VitaDateTime vt_datetime_from_text_iso(const char *timebuf);

/** Read ISO Ext timestamp format "YYYY-MM-DDTHH:MM:SS" to VitaDateTime structure
    @param timebuf timestamp
    @returns struct VitaDateTime
*/
extern struct VitaDateTime vt_datetime_from_text_iso_ext(const char *timebuf);


/** Calculates year day from a date
    @param vdt VitaDateTime struct
    @returns day of year [1; 365/366]

    @note returns -1 upon failure (invalid datetime format)
*/
extern int16_t vt_datetime_find_year_day(const struct VitaDateTime vdt);

/** Calculates week day from a date
    @param vdt VitaDateTime struct
    @returns day of week [1; 7]

    @note returns -1 upon failure (invalid datetime format)
*/
extern int16_t vt_datetime_find_week_day(const struct VitaDateTime vdt);

/** Calculates days in a month from a date
    @param vdt VitaDateTime struct
    @returns days in month

    @note adjusts the `month` range if invalid one is specified ([month < 1 => 1], [month > 12 => 12])
    @note will always return 28 days for february if invalid year is specified
*/
extern int16_t vt_datetime_find_days_in_month(const struct VitaDateTime vdt);

/** Calculates days in a year from a date
    @param vdt VitaDateTime struct
    @returns days in month

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern int16_t vt_datetime_find_days_in_year(const struct VitaDateTime vdt);

/** Checks if datetime is a leap year
    @param vdt VitaDateTime struct
    @returns true if leap year

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern bool vt_datetime_is_leap_year(const struct VitaDateTime vdt);

/** Checks if timestamp is a valid string format
    @param timebuf timestamp
    @returns true if valid

    @note checks if format is valid, does not check for correct values in timestamp itself
*/
extern bool vt_datetime_is_valid_text(const char *const timebuf);

/** Checks if timestamp is a valid ISO format string
    @param timebuf timestamp
    @returns true if valid

    @note checks if format is valid, does not check for correct values in timestamp itself
*/
extern bool vt_datetime_is_valid_text_iso(const char *const timebuf);

/** Checks if timestamp is a valid ISO Extended format string
    @param timebuf timestamp
    @returns true if valid

    @note checks if format is valid, does not check for correct values in timestamp itself
*/
extern bool vt_datetime_is_valid_text_iso_ext(const char *const timebuf);

/** Returns datetime in seconds
    @param vdt VitaDateTime struct
    @returns time_t seconds

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern time_t vt_datetime_to_secs(const struct VitaDateTime vdt);

/** Converts seconds to vita datetime structure representation
    @param secs seconds
    @returns struct VitaDateTime
*/
extern struct VitaDateTime vt_datetime_from_secs(const time_t secs);

/** Perform addition/substraction operation on datetime
    @param vdt valid datetime
    @param delta delta time
    @returns struct VitaDateTime (vdt + delta)
*/
extern struct VitaDateTime vt_datetime_op(const struct VitaDateTime vdt, const struct VitaDateTime delta);

/** Returns difference between two datetime stamps in seconds
    @param vdt1 valid datetime
    @param vdt2 valid datetime
    @returns time_t seconds

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern time_t vt_datetime_diff_secs(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2);

/** Returns difference between two datetime stamps in minutes
    @param vdt1 valid datetime
    @param vdt2 valid datetime
    @returns time_t seconds

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern time_t vt_datetime_diff_minutes(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2);

/** Returns difference between two datetime stamps in hours
    @param vdt1 valid datetime
    @param vdt2 valid datetime
    @returns time_t seconds

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern time_t vt_datetime_diff_hours(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2);

/** Returns difference between two datetime stamps in days
    @param vdt1 valid datetime
    @param vdt2 valid datetime
    @returns time_t seconds

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern time_t vt_datetime_diff_days(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2);

/** Returns difference between two datetime stamps
    @param vdt1 valid datetime
    @param vdt2 valid datetime
    @returns delta datetime (days, hours, minutes, seconds)

    @note if year is invalid, VT_DATETIME_MIN_YEAR_RANGE(1900) is used
*/
extern struct VitaDateTime vt_datetime_diff(const struct VitaDateTime vdt1, const struct VitaDateTime vdt2);

#endif // VITA_TIME_DATETIME_H
