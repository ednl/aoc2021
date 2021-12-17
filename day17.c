#include <stdio.h>
#include <stdbool.h>
#include "startstoptimer.h"

#define TX0 (137)
#define TX1 (171)
#define TY0 (-98)
#define TY1 (-73)
#define YRANGE (-TY0 * 2 + 1)
#define XRANGE (TX1 + 1)

static bool hit[YRANGE][XRANGE] = {0};

static int dist(int v0, int t)
{
    return v0*t - t*(t - 1)/2;
}

static int maxdist(int v0)
{
    if (v0 <= 0)
        return 0;
    return dist(v0, v0);
}

static int height(int vy0, int t)
{
    if (t <= 0)
        return 0;
    return dist(vy0, t);
}

static int width(int vx0, int t)
{
    if (t <= 0)
        return 0;
    if (t >= vx0)
        return maxdist(vx0);
    return dist(vx0, t);
}

int main(void)
{
    starttimer();
    int ymax = 0;
    int hits = 0;
    for (int vy0 = TY0; vy0 <= -TY0; ++vy0)
        for (int t = 0, y; (y = height(vy0, t)) >= TY0; ++t)
            if (y <= TY1) {
                int ym = maxdist(vy0);
                if (ym > ymax)
                    ymax = ym;
                for (int vx0 = 1; vx0 <= TX1; ++vx0) {
                    int x = width(vx0, t);
                    if (x >= TX0 && x <= TX1 && !hit[vy0 - TY0][vx0]) {
                        hit[vy0 - TY0][vx0] = true;
                        ++hits;
                    }
                }
            }
    printf("Part 1: %d\n", ymax);  // 4753
    printf("Part 2: %d\n", hits);  // 1546
    printf("%.0f µs\n", stoptimer_us());
    return 0;
}
