#ifndef NANOTIMER_H
#define NANOTIMER_H

#include <stdint.h>

// Uneven calls: start timer, return 0.
// Even calls: return elapsed wall clock time in nanoseconds
// since previous call. Multiply by 1e-9 to get seconds.
uint64_t nanotimer(void);

#endif  // NANOTIMER_H
