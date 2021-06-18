#ifndef TIME_H
#define TIME_H

#include <bits/libc.h>

#include <stddef.h>

__BEGIN_HEADER

typedef size_t clock_t;
typedef size_t time_t;

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

__END_HEADER

#endif
