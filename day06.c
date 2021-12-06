#include <stdio.h>
#include <stdint.h>

#define CYCLE   7  // spawn every 7 days
#define DELAY   2  // add 2 days to the first spawn cycle
#define LIFE (CYCLE + DELAY)  // the whole circle of life
#define DAYS1  80  // part 1
#define DAYS2 256  // part 2

// Population histogram per age mod 9
static uint64_t pop[LIFE] = {0};

// Live for X days, return total population
// spawn = histogram index (= age mod 9) of lantern fish
//         that are about to spawn (= countdown 0)
// Advance countdown to next spawn by increasing the index every day
// Return total population count
static uint64_t live(int spawn, int days)
{
    for (int i = spawn; i < spawn + days; ++i)
        // Everyone whose birth countdown is 0 starts a new cycle
        pop[(i + CYCLE) % LIFE] += pop[i % LIFE];
        // Leaving the population of this index untouched means:
        // removing them from this bin and adding their offspring
        // to (what is the next) bin-1 = net zero effect

    uint64_t count = 0;
    for (int i = 0; i < LIFE; ++i)
        count += pop[i];
    return count;
}

int main(void)
{
    // Build population histogram by age bin
    // by reading single-digit ages from CSV
    FILE *f = fopen("input06.txt", "r");
    int c = ',';
    while (c == ',') {
        pop[fgetc(f) - '0']++;
        c = fgetc(f);
    }
    fclose(f);

    printf("Part 1: %llu\n", live(0, DAYS1));
    printf("Part 2: %llu\n", live(DAYS1 % LIFE, DAYS2 - DAYS1));
    return 0;
}
