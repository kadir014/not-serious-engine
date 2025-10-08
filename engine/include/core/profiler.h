/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_PROFILER_H
#define _NS_PROFILER_H

/**
 * @file profiler.h
 * @brief Built-in performance profiler.
 */
#include "engine/include/_internal.h"


/**
 * @brief Timings for parts of single space simulation step in seconds.
 */
typedef struct {
    double frame; /**< Time spent in one game frame. */
    double render; /**< Time spent for rendering. */
} nsProfiler;


static inline void nsProfiler_reset(nsProfiler *profiler) {
    profiler->frame = 0.0;
    profiler->render = 0.0;
}


#if NS_PLATFORM == NS_PLATFORM_WINDOWS

    #include <windows.h>

    typedef struct {
        double elapsed;
        LARGE_INTEGER _start;
        LARGE_INTEGER _end;
    } nsPrecisionTimer;

    static inline void nsPrecisionTimer_start(nsPrecisionTimer *timer) {
        QueryPerformanceCounter(&timer->_start);
    }

    static inline double nsPrecisionTimer_stop(nsPrecisionTimer *timer ) {
        QueryPerformanceCounter(&timer->_end);

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        timer->elapsed = (double)(timer->_end.QuadPart - timer->_start.QuadPart) / (double)frequency.QuadPart;
        return timer->elapsed;
    }

#else

    #include <time.h>
    #include <unistd.h>

    // TODO: On OSX, frequency can be milliseconds instead of nanoseconds
    #define NS_PER_SECOND 1e9

    typedef struct {
        double elapsed;
        struct timespec _start;
        struct timespec _end;
        struct timespec _delta;
    } nsPrecisionTimer;

    static inline void nsPrecisionTimer_start(nsPrecisionTimer *timer) {
        clock_gettime(CLOCK_REALTIME, &timer->_start);
    }

    static inline double nsPrecisionTimer_stop(nsPrecisionTimer *timer) {
        clock_gettime(CLOCK_REALTIME, &timer->_end);

        timer->_delta.tv_nsec = timer->_end.tv_nsec - timer->_start.tv_nsec;
        timer->_delta.tv_sec = timer->_end.tv_sec - timer->_start.tv_sec;

        if (timer->_delta.tv_sec > 0 && timer->_delta.tv_nsec < 0) {
            timer->_delta.tv_nsec += NS_PER_SECOND;
            timer->_delta.tv_sec--;
        }
        else if (timer->_delta.tv_sec < 0 && timer->_delta.tv_nsec > 0) {
            timer->_delta.tv_nsec -= NS_PER_SECOND;
            timer->_delta.tv_sec++;
        }

        timer->elapsed = (double)timer->_delta.tv_nsec / NS_PER_SECOND;
        return timer->elapsed;
    }

#endif


#endif