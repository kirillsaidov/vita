#include "vita/datetime/time.h"

/* Converts from `tm` to `VitaDateTime` datetime format
    @param stm tm struct
    @returns struct VitaDateTime
*/
static struct VitaDateTime vt_time_tm_to_vdt(struct tm stm);

/* Converts from `VitaDateTime` to `tm` datetime format
    @param vdt VitaDateTime struct
    @returns struct tm
*/
// static struct tm vt_time_vdt_to_tm(struct VitaDateTime vdt);

struct VitaDateTime vt_time_get_now(void) {
    // get time
    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);

    /** struct tm variables range:
        year        years passed since 1900 | + 1900 = YYYY
        month       [0, 11] | + 1 = [1, 12]
        month_day   [1, 31]

        hour        [0, 23]
        minute      [0, 59]
        second      [0, 60]

        week_day    [0, 6]   Sunday(0), Monday(1), ..., Saturday(6) | + 1 = [1, 7]
        year_day    [0, 365] days since January 1 | + 1 = [1, 366]
    */

    return vt_time_tm_to_vdt(*stm);
}

void vt_time_get_now_as_text(char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(len >= VT_TIME_BUFFER_SIZE, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // get time
    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    timebuf[strftime(timebuf, VT_TIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", stm)] = '\0';
}

void vt_time_get_now_as_text_pretty(char *timebuf, const size_t len) {
    // check for invalid input
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));
    VT_DEBUG_ASSERT(len >= VT_TIME_BUFFER_SIZE, "%s\n", vt_get_vita_error_str(vt_ve_error_invalid_arguments));

    // get time
    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    asctime_r(stm, timebuf);
}

/* ---------------------- PRIVATE FUNCTIONS ---------------------- */

static struct VitaDateTime vt_time_tm_to_vdt(struct tm stm) {
    return (struct VitaDateTime) {
        .year = stm.tm_year + 1900,
        .month = stm.tm_mon + 1,
        .month_day = stm.tm_mday,
        .hour = stm.tm_hour,
        .minute = stm.tm_min,
        .second = stm.tm_sec,
        .week_day = stm.tm_wday + 1,
        .year_day = stm.tm_yday + 1
    };
}

// static struct tm vt_time_vdt_to_tm(struct VitaDateTime vdt) {
//     return (struct tm) {
//         .tm_year = vdt.year - 1900,
//         .tm_mon = vdt.month - 1,
//         .tm_mday = vdt.month_day,
//         .tm_hour = vdt.hour, 
//         .tm_min = vdt.minute,
//         .tm_sec = vdt.second,
//         .tm_wday = vdt.week_day - 1,
//         .tm_yday = vdt.year_day - 1
//     };
// }


