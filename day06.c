#include <stdio.h>
#include <stdint.h>

#define CYCLE   7  // spawn every 7 days
#define DELAY   2  // add 2 days to the first spawn cycle
#define LIFE (CYCLE + DELAY)  // the whole circle of life
#define DAYS1  80  // part 1
#define DAYS2 256  // part 2

// Histogram of fish population count per age mod 9
static uint64_t age[LIFE] = {0};

// Live for X days, return total population
static uint64_t live(int days)
{
    // Remember day where day % LIFE = age index for spawn countdown 0
    static int day = 0;
    for (; day < days; ++day)
        // On each new day, fish on countdown 0 start a new spawn cycle
        age[(day + CYCLE) % LIFE] += age[day % LIFE];
        // Leaving the population of the current index untouched means:
        //   removing them from this bin and adding their offspring
        //   to bin+LIFE (= the next bin-1) is net zero effect

    uint64_t count = 0;
    for (int i = 0; i < LIFE; ++i)
        count += age[i];
    return count;
}

int main(void)
{
    // Build population histogram by age bin
    // by reading single-digit ages from CSV
    FILE *f = fopen("input06.txt", "r");
    int c = ',';
    while (c == ',') {
        age[fgetc(f) - '0']++;
        c = fgetc(f);
    }
    fclose(f);

    printf("Part 1: %llu\n", live(DAYS1));  // 374927
    printf("Part 2: %llu\n", live(DAYS2));  // 1687617803407
    return 0;
}
