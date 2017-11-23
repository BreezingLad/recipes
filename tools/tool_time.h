/*************************************************************************
	> File Name: tool_time.h
	> Author: liulianglu
	> Description: 
	> Created Time: Fri 20 Oct 2017 03:56:03 PM EDT
 ************************************************************************/

#ifndef _TOOL_TIME_H
#define _TOOL_TIME_H
#include<sys/time.h>
#include<time.h>
#include<stdio.h>

class TimeStastics
{
    struct timeval m_start;
    struct timeval m_end;

public:
    inline void start()
    {
        gettimeofday(&m_start, 0);
    }

    inline void stop()
    {
        gettimeofday(&m_end, 0);
    }

    inline unsigned long total_us() 
    { 
        return 1000000 * (m_end.tv_sec - m_start.tv_sec) 
            + m_end.tv_usec - m_start.tv_usec;
    }

    inline unsigned long total_ms()
    {
        return 1000 * (m_end.tv_sec - m_start.tv_sec)
            + (m_end.tv_usec - m_start.tv_usec) / 1000;
    }
};

//2017102016:19:23
inline void current_time(char* result)
{
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    tm nowTime;

    localtime_r(&time.tv_sec, &nowTime);

    sprintf(result, "%04d%02d%02d%02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon + 1,
            nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

}
#endif
