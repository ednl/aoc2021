#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define CYCLE   7  // spawn every 7 days
#define DELAY   2  // add 2 days to the first spawn cycle
#define LIFE (CYCLE + DELAY)  // the whole circle of life
#define DAYS1  80  // part 1
#define DAYS2 256  // part 2

// Histogram of fish population count per age mod 9
static uint64_t age[LIFE] = {0};

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

// Live for days, return population
static uint64_t live(const int days)
{
    static int day = 0;  // remember the day
    for (; day < days; ++day)
        age[(day + CYCLE) % LIFE] += age[day % LIFE];  // start new spawn cycle
        // Leaving current age bin untouched = each fishie gets one kiddo

    uint64_t pop = 0;
    for (int i = 0; i < LIFE; ++i)
        pop += age[i];
    return pop;
}

int main(void)
{
    timer();

    // Build population histogram by age bin
    FILE *f = fopen("input06.txt", "r");
    int c = ',';
    while (c == ',') {
        age[fgetc(f) - '0']++;
        c = fgetc(f);
    }
    fclose(f);

    printf("Part 1: %llu\n", live(DAYS1));  // 374927
    printf("Part 2: %llu\n", live(DAYS2));  // 1687617803407
    printf("Time: %.6f s\n", timer());
    return 0;
}
