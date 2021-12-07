#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 1000
static int data[N] = {0};

static double timer(void)
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
        return 1.0 * t1.tv_sec + 1e-9 * t1.tv_nsec - (1.0 * t0.tv_sec + 1e-9 * t0.tv_nsec);
    }
}

static int cmp(const void *a, const void *b)
{
    int p = *(const int*)a;
    int q = *(const int*)b;
    return (q < p) - (p < q);
}

static int cost1(const int target)
{
    int cost = 0;
    for (int i = 0; i < N; ++i)
        cost += abs(data[i] - target);
    return cost;
}

static int cost2(const int target)
{
    int cost = 0;
    for (int i = 0; i < N; ++i) {
        int r = abs(data[i] - target), r2 = r >> 1;
        cost += (r & 1) ? r2 * r + r : r2 * r + r2;
    }
    return cost;
}

static int descend(int (*cost)(const int))
{
    int lo = data[0], hi = data[N-1], y0, y1;
    while (hi - lo > 1) {
        y0 = cost(lo);
        y1 = cost(hi);
        int g0 = y0 - cost(lo + 1);
        int g1 = y1 - cost(hi - 1);
        if (!g0) return y0;
        if (!g1) return y1;
        int x0 = lo + (int)((long long)(hi - lo) * g0 / (g0 + g1));
        int x1 = (lo + hi)/2;
        if (x0 <= x1) {
            lo = x0 - (x1 - x0);
            hi = x1;
        } else {
            lo = x1;
            hi = x0 + (x0 - x1);
        }
    }
    return cost(lo);
}

int main(void)
{
    timer();
    FILE *f = fopen("input07.txt", "r");
    for (int i = 0, a; i < N && fscanf(f, "%d", &a) == 1; ) {
        data[i++] = a;
        fgetc(f);
    }
    fclose(f);
    qsort(data, N, sizeof(int), cmp);
    printf("Part 1: %d\n", cost1(data[N/2]));
    printf("Part 2: %d\n", descend(cost2));
    printf("%.1f µs\n", timer() * 1e6);
    return 0;
}
