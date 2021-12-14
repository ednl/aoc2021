#include <stdio.h>
#include <stdlib.h>
#include "startstoptimer.h"

#define N 100
#define LEN 20000

typedef struct {
    int id;
    char pair[3], ins;
} Rule;

static Rule rule[N] = {0};
static char polymer[2][LEN] = {0};

static int id_asc(const void *a, const void *b)
{
    const int p = ((const Rule * const)a)->id;
    const int q = ((const Rule * const)b)->id;
    return (q < p) - (p < q);
}

static Rule* ruleindex(int id)
{
    const Rule key = (Rule){.id=id};
    return bsearch(&key, rule, N, sizeof *rule, id_asc);
}

static void insert(char *src, char *dst)
{
    Rule *p;
    int id = *dst++ = *src++;
    while (*src) {
        id = ((id & 255) << 8) | *src;
        if ((p = ruleindex(id)))
            *dst++ = p->ins;
        *dst++ = *src++;
    }
}

static int range(const char *s)
{
    int freq[26] = {0};
    const char *c = s;
    while (*c)
        freq[*c++ - 'A']++;

    int min = LEN, max = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] && freq[i] < min)
            min = freq[i];
        if (freq[i] > max)
            max = freq[i];
    }
    return max - min;
}

int main(void)
{
    starttimer();

    FILE *f = fopen("input14.txt", "r");
    fscanf(f, "%s ", polymer[0]);
    for (int i = 0; i < N && fscanf(f, "%2s -> %c", rule[i].pair, &rule[i].ins) == 2; ++i)
        rule[i].id = (rule[i].pair[0] << 8) | rule[i].pair[1];
    fclose(f);
    qsort(rule, N, sizeof *rule, id_asc);

    char *src = polymer[1], *dst = polymer[0], *t;
    for (int i = 0; i < 10; ++i) {
        t = src;
        src = dst;
        dst = t;
        insert(src, dst);
    }
    printf("Part 1: %d\n", range(dst));  // 2194

    printf("%.2f ms\n", stoptimer_ms());
    return 0;
}
