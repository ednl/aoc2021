#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define N 1000
static int data[N] = {0};

static uint64_t nanotimer(void)
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

int main(void)
{
    FILE *f = fopen("input07.txt", "r");
    for (int i = 0, a; fscanf(f, "%d", &a) == 1; ++i) {
        data[i] = a;
        fgetc(f);  // CSV with newline
    }
    fclose(f);

    // Start timer
    nanotimer();

    // Linear cost => take median
    // N is even, so N/2 is good (N/2-1 will be the same)
    qsort(data, N, sizeof(int), cmp);
    printf("Part 1: %d\n", cost1(data[N/2]));

    // Quadratic cost => take mean
    // Could go 1 either way because of rounding, but this happens to work for me
    int mean = 0;
    for (int i = 0; i < N; ++i)
        mean += data[i];
    mean /= N;
    printf("Part 2: %d\n", cost2(mean));

    // Run time of the algorithms only, not the disk read
    printf("%.9f s\n", nanotimer() * 1e-9);
    return 0;
}
