#include <stdio.h>
#include <time.h>
#include <stdlib.h>

enum 
{ 
    SECONDS_IN_DAY = 60 * 60 * 24,
    MONTH_IN_YEAR = 12,
    FIRST_DAY_IN_SECOND_WEEK = 8,
    LAST_DAY_IN_SECOND_WEEK = 14,
    THURSDAY = 4,
    MOD = 3,
    START_YEAR = 1900,
    TWO_WEEKS = 14
}; 

int 
main(void) 
{
    int year;
    if (scanf("%d", &year) != 1) {
        return 1;
    }
    struct tm cur;
    cur.tm_year = year - START_YEAR;
    cur.tm_hour = cur.tm_min = cur.tm_sec = 0;
    cur.tm_isdst = -1;
    for (cur.tm_mon = 0; cur.tm_mon < MONTH_IN_YEAR; cur.tm_mon++) {
        for (cur.tm_mday = FIRST_DAY_IN_SECOND_WEEK; cur.tm_mday <= LAST_DAY_IN_SECOND_WEEK; cur.tm_mday++) {
            time_t cur_time = timegm(&cur);
            struct tm result;
            struct tm *cur_ptr = gmtime_r(&cur_time, &result);
            if (NULL == cur_ptr) {
                return 1;
            }
            if (result.tm_wday == THURSDAY) {
                if (result.tm_mday % MOD) {
                    printf("%d %d\n", result.tm_mon + 1, result.tm_mday);
                }
                if ((result.tm_mday + TWO_WEEKS) % MOD) {
                    printf("%d %d\n", result.tm_mon + 1, result.tm_mday + TWO_WEEKS);
                }
                break;
            }
        }
    }
    return 0;
}
