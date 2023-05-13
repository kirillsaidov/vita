#include "vita/datetime/time.h"

void vt_time_get_current_timestamp(char *timebuf, const size_t len) {
    if(len < VT_TIME_STAMP_BUFFER_SIZE) {
        return;
    }

    const time_t t = time(NULL);
    const struct tm *stm = localtime(&t);
    timebuf[strftime(timebuf, VT_TIME_STAMP_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", stm)] = '\0';
}


