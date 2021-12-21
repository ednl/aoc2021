#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include "startstoptimer.h"

#define POS1      1U  // from input
#define POS2     10U  // from input
#define LOOP     10U
#define WIN1   1000U
#define WIN2     21U
#define CACHE 80000U  // (((9*10 + 9)*20 + 19)*20 + 19)*2 + 1 = 79999

// Possible ways of summing to N with 3 rolls of 3-sided die [1,2,3],
// e.g. sum 4 = (1+1+2) or (1+2+1) or (2+1+1) = 3 ways.
static const uint32_t mult[10] = {0,0,0,1,3,6,7,6,3,1};

static uint64_t cache[CACHE] = {0};
static bool cached[CACHE] = {0};

// Only calculates the number of wins for player 1,
// so, not suitable for inputs where player 2 would win.
static uint64_t player1wins(uint32_t pos1, uint32_t pos2, uint32_t score1, uint32_t score2, bool player1)
{
    if (score1 >= WIN2) return 1U;
    if (score2 >= WIN2) return 0;

    uint32_t hash = ((((pos1 - 1)*10 + pos2 - 1)*20 + score1 - 1)*20 + score2 - 1)*2 + player1;
    if (cached[hash])
        return cache[hash];

    uint64_t wins = 0;
    if (player1) {
        for (uint32_t d3 = 3; d3 < 10; ++d3) {
            uint32_t p = (pos1 - 1U + d3) % LOOP + 1U;
            wins += mult[d3] * player1wins(p, pos2, score1 + p, score2, false);
        }
    } else {
        for (uint32_t d3 = 3; d3 < 10; ++d3) {
            uint32_t p = (pos2 - 1U + d3) % LOOP + 1U;
            wins += mult[d3] * player1wins(pos1, p, score1, score2 + p, true);
        }
    }

    cached[hash] = true;
    cache[hash] = wins;
    return wins;
}

int main(void)
{
    starttimer();

    uint32_t roll = 3, d3 = 6, pos1 = POS1, pos2 = POS2, score1 = 0, score2 = 0;
    while (1) {
        if (roll & 1U) {
            pos1 = (pos1 - 1U + d3) % LOOP + 1U;
            score1 += pos1;
            if (score1 >= WIN1)
                break;
        } else {
            pos2 = (pos2 - 1U + d3) % LOOP + 1U;
            score2 += pos2;
            if (score2 >= WIN1)
                break;
        }
        roll += 3;
        d3 += 9;
    }
    printf("Part 1: %"PRIu32"\n", roll * (roll & 1U ? score2 : score1));  // 428736
    printf("Part 2: %"PRIu64"\n", player1wins(POS1, POS2, 0, 0, true));   // 57328067654557

    printf("%.0f µs\n", stoptimer_us());
    return 0;
}
