#include <stdio.h>
#include <stdint.h>

#define CYCLE 7
#define DELAY 2
#define LIFE  (CYCLE + DELAY)
#define DAYS1 80
#define DAYS2 256

static uint64_t pop[LIFE] = {0};

static uint64_t live(int birth, int days)
{
    for (int i = birth; i < birth + days; ++i)
        pop[(i + CYCLE) % LIFE] += pop[i % LIFE];
    uint64_t census = 0;
    for (int i = 0; i < LIFE; ++i)
        census += pop[i];
    return census;
}

int main(void)
{
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
