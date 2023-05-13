#ifndef VITA_TIME_H
#define VITA_TIME_H

/** VITA_TIME MODULE 
    - vt_time_get_current_timestamp
*/

#include "../core/core.h"

#define VT_TIME_STAMP_BUFFER_SIZE 21

/** Sets a timestamp "year-month-day hour-minute-seconds" to timebuf
    @param timebuf to store timestamp data
    @param len timebuf length

    @note len must be >= 21! Otherwise, it returns doing nothing.
*/
extern void vt_time_get_current_timestamp(char *timebuf, const size_t len);

#endif // VITA_TIME_H
