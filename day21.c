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

static const bool usecache = true;   // factor 100 in run time
static uint64_t cache[CACHE] = {0};  // cache of intermediate results
static bool cached[CACHE] = {0};

// Multiple ways of summing to N with 3 rolls of 3-sided die [1,2,3],
// e.g. sum 4 = (1+1+2) or (1+2+1) or (2+1+1) = 3 different ways.
static const uint32_t combi[10] = {0,0,0,1,3,6,7,6,3,1};

static uint32_t practicegame(uint32_t pos1, uint32_t pos2)
{
    uint32_t roll = 3, d3 = 6, score1 = 0, score2 = 0;
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
    return roll * (roll & 1U ? score2 : score1);
}

// Only calculates the number of wins for player 1,
// so, not suitable for inputs where player 2 would win.
// (Could be done with a struct to return multiple values in one.)
static uint64_t diracdice(uint32_t pos1, uint32_t pos2, uint32_t score1, uint32_t score2, bool player1)
{
    // Return immediately if one player wins
    if (score1 >= WIN2) return 1U;
    if (score2 >= WIN2) return 0;

    // Return cached result if available (hash should be unique)
    uint32_t hash;
    if (usecache) {
        hash = ((((pos1 - 1)*10 + pos2 - 1)*20 + score1 - 1)*20 + score2 - 1)*2 + player1;
        if (cached[hash])
            return cache[hash];
    }

    // Check every possible progression from this state and add up all wins for player 1
    uint64_t wins = 0;
    if (player1) {
        for (uint32_t d3 = 3; d3 < 10; ++d3) {
            uint32_t p = (pos1 - 1U + d3) % LOOP + 1U;
            wins += combi[d3] * diracdice(p, pos2, score1 + p, score2, false);
        }
    } else {
        for (uint32_t d3 = 3; d3 < 10; ++d3) {
            uint32_t p = (pos2 - 1U + d3) % LOOP + 1U;
            wins += combi[d3] * diracdice(pos1, p, score1, score2 + p, true);
        }
    }

    // Save result and return it
    if (usecache) {
        cached[hash] = true;
        cache[hash] = wins;
    }
    return wins;
}

int main(void)
{
    starttimer();
    printf("Part 1: %"PRIu32"\n", practicegame(POS1, POS2));        // 428736
    printf("Part 2: %"PRIu64"\n", diracdice(POS1, POS2, 0, 0, 1));  // 57328067654557
    printf("%.2f ms\n", stoptimer_ms());
    return 0;
}
