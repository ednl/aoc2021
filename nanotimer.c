#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "nanotimer.h"

// CLOCK_MONOTONIC_RAW is a Linux/Darwin extension so it might not be available
// on Posix systems. It is preferred because unaffected by any NTP adjustments
// including clock frequency (which is unrelated to CPU frequency, so even
// CLOCK_MONOTONIC will not vary with varying CPU frequency.)

#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC
#endif

uint64_t nanotimer(void)
{
    static bool start = true;
    static struct timespec t0;

    if (start) {
        start = false;
        clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
        return 0;
    } else {
        struct timespec t1;
        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        start = true;
        return (uint64_t)(t1.tv_sec - t0.tv_sec) * UINT64_C(1000000000) + (uint64_t)t1.tv_nsec - (uint64_t)t0.tv_nsec;
    }
}
