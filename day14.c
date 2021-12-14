// AoC 2021 Day 14: Extended Polymerization
// https://adventofcode.com/2021/day/14

// Solution idea: treat every transformation rule as replacing 1 pair of elements
// with 2 pairs of elements. E.g. a rule given as AB->C becomes: AB->(AC,CB). So,
// n pairs of AB transform into n pairs of AC plus n pairs of CB. To compensate
// for the unwanted duplication of element C, keep a separate tally of it and
// subtract that later.
// In the whole input file only 10 different elements are present, and there are
// 100 tranformation rules, so every possible pair always has a corresponding
// rule. This simplifies the lookup and counting process, since a result is
// guaranteed and one pair will always split in two.

#include <stdio.h>
#include <stdint.h>    // uint64_t
#include <limits.h>    // UINT64_MAX
#include <inttypes.h>  // PRIu64
#include "startstoptimer.h"

#define PSIZE  20  // original polymer length = 20 in my input
#define RULES 100  // number of pair replacement rules in my input
#define ELMS ('Z' - 'A' + 1)  // number of elements = letters in the alphabet

// Representation of rule AB->C where pair=AB, ins=C.
// "left" then points to the rule for pair AC, "right" to pair CB.
// "count" is how many pairs of AB there are now in the polymer, "next" how many next.
// (Counts can't be updated in place because every pair transforms at the same time.)
typedef struct Rule {
    struct Rule *left, *right;  // pointers to replacement pairs (NULL = no replacement)
    uint64_t count, next;       // how many of this pair are there now and next round
    char pair[3], ins;          // pair (=2 elements) to replace, 1 element to insert
} Rule;

static Rule rule[RULES] = {0};         // all transformation rules
static uint64_t subtract[ELMS] = {0};  // compensate for duplication

// From element A-Z to index 0-25
static inline int ix(const char c)
{
    return (int)(c - 'A');
}

// Find the transformed pair rules for every pair in the rule set
// to initialise the left/right pointers
static Rule* findrule_c(const char a, const char b)
{
    for (int i = 0; i < RULES; ++i)
        if (rule[i].pair[0] == a && rule[i].pair[1] == b)
            return &rule[i];
    return NULL;
}

// Find the corresponding rule for every element pair in the given polymer
// to initialise its count
static inline Rule* findrule_s(const char *s)
{
    return findrule_c(s[0], s[1]);
}

// Read the input file, initialise the transformatin rules,
// the pair count (+compensation), and left/right pointers.
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

// Grow the polymer 1 step. Every pair transforms into 2 other pairs
// minus one elent as duplication compensation.
// First calculate all new counts, then set them as current counts.
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

// Execute a number of growth cycles, then count every element.
// Return the range of element counts as requested by the puzzle.
static uint64_t range(const int cycle)
{
    // Grow the polymer
    for (int i = 0; i < cycle; ++i)
        grow();

    // Count individual elements from pairs
    uint64_t hist[ELMS] = {0};  // histogram
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
