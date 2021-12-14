#include <stdio.h>
#include <stdint.h>    // int64_t
#include <limits.h>    // UINT64_MAX
#include <inttypes.h>  // PRIu64
#include <string.h>    // strlen
#include "startstoptimer.h"

#define BUFLEN  64   // original polymer length = 20 in my input
#define BUFFRM "63"  // read max BUFLEN-1 chars
#define RULES  100   // number of pair replacement rules
#define ELMS  ('Z' - 'A' + 1)  // number of elements = letters in the alphabet

typedef struct Rule {
    struct Rule *left, *right;  // pointers to replacement pairs (NULL = no replacement)
    int64_t count, next;        // how many of this pair now and next round
    char pair[3], ins;          // pair (2 elements) to replace, 1 element to insert
} Rule;

static Rule rule[RULES] = {0};
static int64_t corr[ELMS] = {0};

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

static Rule* findrule_s(const char *s)
{
    return findrule_c(s[0], s[1]);
}

static void read(const char *inp)
{
    char polymer[BUFLEN];
    int len = 0, n = 0;

    FILE *f = fopen(inp, "r");
    if (fscanf(f, "%"BUFFRM"s ", polymer) == 1)
        len = (int)strlen(polymer);
    while (n < RULES && fscanf(f, "%2s -> %c", rule[n].pair, &rule[n].ins) == 2)
        ++n;
    fclose(f);

    // Count elements by pairs in initial polymer
    if (len > 1) {
        Rule *r;
        // First pair
        if ((r = findrule_s(polymer))) {
            r->count++;                      // no duplicate element yet
        } else {
            corr[ix(polymer[0])]++;          // not found, add both elements individually
            corr[ix(polymer[1])]++;
        }
        // Proceed with next (overlapping) pair
        for (int i = 1; i < len - 1; ++i)
            if ((r = findrule_s(&polymer[i]))) {
                r->count++;                  // add another pair, and
                corr[ix(polymer[i])]--;      // subtract duplicate element
            } else
                corr[ix(polymer[i + 1])]++;  // add only the new element
    }

    // Links to replacement pairs
    for (int i = 0; i < n; ++i) {
        rule[i].left  = findrule_c(rule[i].pair[0], rule[i].ins    );
        rule[i].right = findrule_c(rule[i].ins    , rule[i].pair[1]);
    }
}

static void grow(void)
{
    for (int i = 0; i < RULES; ++i)
        if (rule[i].count) {
            if (rule[i].left && rule[i].right) {
                rule[i].left->next        += rule[i].count;
                rule[i].right->next       += rule[i].count;
                corr[ix(rule[i].ins)]     -= rule[i].count;  // subtract duplicate element
            } else if (rule[i].left) {
                rule[i].left->next        += rule[i].count;
                corr[ix(rule[i].pair[1])] += rule[i].count;
            } else if (rule[i].right) {
                corr[ix(rule[i].pair[0])] += rule[i].count;
                rule[i].right->next       += rule[i].count;
            } else
                rule[i].next              += rule[i].count;
        }
    for (int i = 0; i < RULES; ++i) {
        rule[i].count = rule[i].next;
        rule[i].next = 0;  // always reset for next growth cycle!
    }
}

static int64_t range(const int cycle)
{
    // Grow the polymer
    for (int i = 0; i < cycle; ++i)
        grow();

    // Count individual elements from pairs
    int64_t hist[ELMS] = {0};
    for (int i = 0; i < RULES; ++i) {
        hist[ix(rule[i].pair[0])] += rule[i].count;
        hist[ix(rule[i].pair[1])] += rule[i].count;
    }

    // Correct element count with added and subtracted counts,
    // determine maximum and minimum (non zero), return range
    int64_t min = INT64_MAX, max = 0;
    for (int i = 0; i < ELMS; ++i) {
        hist[i] += corr[i];
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
    read("input14.txt");
    printf("Part 1: %"PRId64"\n", range(10));  // 2194
    printf("Part 2: %"PRId64"\n", range(30));  // 2360298895777
    printf("%.0f µs\n", stoptimer_us());
    return 0;
}
