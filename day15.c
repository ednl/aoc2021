#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include "startstoptimer.h"

#define INP "input15.txt"
#define DIM 100
static int grid[DIM][DIM] = {0};

typedef struct {
    int x, y;
} Point;

typedef struct PQ {
    struct PQ *prev, *next;
    int64_t prio;
    Point data;
} PQ;

static bool pq_clean(PQ **pq)
{
    while (*pq) {
        PQ *next = (*pq)->next;
        free(*pq);
        *pq = next;
    }
    return true;
}

static bool pq_push(PQ **pq, const Point data, const int prio)
{
    PQ *node = malloc(sizeof(PQ));
    if (!node)
        return false;
    PQ *p = NULL, *q = *pq;
    while (q && q->prio < prio) {
        p = q;
        q = q->next;
    }
    *node = (PQ){.prev=p, .next=q, .prio=prio, .data=data};
    if (p) p->next = node; else *pq = node;
    if (q) q->prev = node;
    return true;
}

static bool pq_pop(PQ **pq, Point * const data)
{
    if (!*pq)
        return false;
    PQ *p = *pq;
    *pq = p->next;
    *data = p->data;
    free(p);
    return true;
}

static int astar(const int mult)
{
    static const Point nb[4] = {{-1,0},{0,-1},{1,0},{0,1}};
    int cost[DIM * mult][DIM * mult];
    int f[DIM * mult][DIM * mult];
    int g[DIM * mult][DIM * mult];
    bool inq[DIM * mult][DIM * mult];
    Point cur = (Point){0, 0};
    Point end = (Point){DIM * mult - 1, DIM * mult - 1};
    PQ *pq = NULL;

    for (int i = 0; i < DIM * mult; ++i) {
        const int id = i / DIM;
        const int im = i % DIM;
        for (int j = 0; j < DIM * mult; ++j) {
            cost[i][j] = 1 + (grid[im][j % DIM] - 1 + id + j / DIM) % 9;
            f[i][j] = g[i][j] = INT_MAX;
            inq[i][j] = false;
        }
    }
    f[0][0] = end.x + end.y;  // h() = Manhattan distance to goal
    g[0][0] = 0;
    pq_push(&pq, cur, f[0][0]);
    inq[cur.y][cur.x] = true;

    while (pq) {
        pq_pop(&pq, &cur);
        if (cur.x == end.x && cur.y == end.y) {
            pq_clean(&pq);
            return f[end.y][end.x];
        }
        inq[cur.y][cur.x] = false;
        for (size_t i = 0; i < sizeof nb / sizeof *nb; ++i) {
            Point nxt = (Point){cur.x + nb[i].x, cur.y + nb[i].y};
            if (nxt.x >= 0 && nxt.y >= 0 && nxt.x < DIM * mult && nxt.y < DIM * mult) {
                int nxtg = g[cur.y][cur.x] + cost[nxt.y][nxt.x];
                if (nxtg < g[nxt.y][nxt.x]) {
                    g[nxt.y][nxt.x] = nxtg;
                    f[nxt.y][nxt.x] = nxtg + end.x - nxt.x + end.y - nxt.y;  // h() = Manhattan distance to goal
                    if (!inq[nxt.y][nxt.x]) {
                        pq_push(&pq, nxt, f[nxt.y][nxt.x]);
                        inq[nxt.y][nxt.x] = true;
                    }
                }
            }
        }
    }
    pq_clean(&pq);
    return -1;
}

int main(void)
{
    starttimer();
    FILE *f = fopen(INP, "r");
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j)
            grid[i][j] = fgetc(f) - '0';
        fgetc(f);  // newline
    }
    fclose(f);

    printf("Part 1: %d\n", astar(1));  // 592
    printf("Part 2: %d\n", astar(5));  // 2897
    printf("%.0f ms\n", stoptimer_ms());
    return 0;
}
