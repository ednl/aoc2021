#include <stdio.h>
#include <stdint.h>    // uint64_t
#include <limits.h>    // UINT64_MAX
#include <inttypes.h>  // PRIu64
#include "startstoptimer.h"

#define PSIZE  20  // original polymer length = 20 in my input
#define RULES 100  // number of pair replacement rules in my input
#define ELMS ('Z' - 'A' + 1)  // number of elements = letters in the alphabet

typedef struct Rule {
    struct Rule *left, *right;  // pointers to replacement pairs (NULL = no replacement)
    uint64_t count, next;       // how many of this pair are there now and next round
    char pair[3], ins;          // pair (=2 elements) to replace, 1 element to insert
} Rule;

static Rule rule[RULES] = {0};
static uint64_t subtract[ELMS] = {0};

static inline int ix(const char c)
{
    return (int)(c - 'A');
}

static Rule* findrule_c(const char a, const char b)
{
    for (int i = 0; i < RULES; ++i)
        if (rule[i].pair[0] == a && rule[i].pair[1] == b)
            return &rule[i];
    return NULL;
}

static inline Rule* findrule_s(const char *s)
{
    return findrule_c(s[0], s[1]);
}

static void parse(const char *inp)
{
    char polymer[PSIZE + 1];  // must be same size as in input file, +1 for \0
    FILE *f = fopen(inp, "r");
    fscanf(f, "%s ", polymer);
    for (int i = 0; i < RULES && fscanf(f, "%2s -> %c", rule[i].pair, &rule[i].ins) == 2; ++i);
    fclose(f);

    // Count elements by pairs in initial polymer
    findrule_s(polymer)->count++;          // first pair, no duplicate element yet
    for (int i = 1; i < PSIZE - 1; ++i) {
        findrule_s(&polymer[i])->count++;  // add next overlapping pair, and
        subtract[ix(polymer[i])]++;        // subtract duplicate element
    }

    // Save links to replacement pairs
    for (int i = 0; i < RULES; ++i) {
        rule[i].left  = findrule_c(rule[i].pair[0], rule[i].ins    );
        rule[i].right = findrule_c(rule[i].ins    , rule[i].pair[1]);
    }
}

static void grow(void)
{
    for (int i = 0; i < RULES; ++i) {
        rule[i].left->next  += rule[i].count;
        rule[i].right->next += rule[i].count;
        subtract[ix(rule[i].ins)] += rule[i].count;  // subtract duplicate element
    }
    for (int i = 0; i < RULES; ++i) {
        rule[i].count = rule[i].next;
        rule[i].next = 0;  // always reset for next growth cycle!
    }
}

static uint64_t range(const int cycle)
{
    // Grow the polymer
    for (int i = 0; i < cycle; ++i)
        grow();

    // Count individual elements from pairs
    uint64_t hist[ELMS] = {0};
    for (int i = 0; i < RULES; ++i) {
        hist[ix(rule[i].pair[0])] += rule[i].count;
        hist[ix(rule[i].pair[1])] += rule[i].count;
    }

    // Correct element count with added and subtracted counts,
    // determine maximum and minimum (non zero), return range
    uint64_t min = UINT64_MAX, max = 0;
    for (int i = 0; i < ELMS; ++i) {
        hist[i] -= subtract[i];
        if (hist[i] < min && hist[i])
            min = hist[i];
        if (hist[i] > max)
            max = hist[i];
    }
    return max - min;
}

int main(void)
{
    starttimer();
    parse("input14.txt");
    printf("Part 1: %"PRIu64"\n", range(10));  // 2194
    printf("Part 2: %"PRIu64"\n", range(30));  // 2360298895777
    printf("%.0f µs\n", stoptimer_us());
    return 0;
}
