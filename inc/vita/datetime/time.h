#ifndef VITA_TIME_H
#define VITA_TIME_H

/** VITA_TIME MODULE 
    - vt_time_get_now_as_text
*/

#include "../core/core.h"
#include "../util/debug.h"

#define VT_TIME_BUFFER_SIZE 26

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
extern struct VitaDateTime vt_time_get_now(void);

/** Sets a timestamp "yyyy-mm-dd hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be at least VT_TIME_BUFFER_SIZE!
*/
extern void vt_time_get_now_as_text(char *timebuf, const size_t len);

/** Sets a textual representation of timestamp "year-month-day hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be at least VT_TIME_BUFFER_SIZE!
*/
extern void vt_time_get_now_as_text_pretty(char *timebuf, const size_t len);

#endif // VITA_TIME_H
