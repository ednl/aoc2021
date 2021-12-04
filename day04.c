#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DRAW  100
#define CARDS 100
#define DIM     5

typedef struct Index {
    struct Index *next;
    unsigned char card, row, col;
} Index;

static unsigned char draw[DRAW] = {0};
static unsigned char card[CARDS][DIM][DIM] = {0};
static Index *index[DRAW] = {0};

int main(void)
{
    FILE *f = fopen("input04.txt", "r");
    unsigned int i = 0;
    unsigned char a;
    if (fscanf(f, "%hhu", &a) == 1)
        draw[i++] = a;
    while (i < DRAW && fscanf(f, ",%hhu", &a) == 1)
        draw[i++] = a;

    unsigned char b = 0, r = 0, c = 0;
    Index *p, *q;
    while (b < CARDS && fscanf(f, " %hhu", &a) == 1) {
        card[b][r][c] = a;

        p = malloc(sizeof(Index));
        *p = (Index){NULL, b, r, c};
        if ((q = index[a])) {
            while (q->next)
                q = q->next;
            q->next = p;
        } else {
            index[a] = p;
        }

        if (++c == DIM) {
            c = 0;
            if (++r == DIM) {
                r = 0;
                ++b;
            }
        }
    }
    fclose(f);

    unsigned char wins = 0;
    bool won[DRAW] = {false};
    for (i = 0; i < DRAW; ++i) {
        p = index[draw[i]];
        while (p) {
            if (!won[p->card]) {
                card[p->card][p->row][p->col] = DRAW;
                unsigned int rowsum = 0;
                unsigned int colsum = 0;
                for (unsigned int j = 0; j < DIM; ++j) {
                    rowsum += card[p->card][p->row][j];
                    colsum += card[p->card][j][p->col];
                }
                if (rowsum == DIM * DRAW || colsum == DIM * DRAW) {
                    ++wins;
                    won[p->card] = true;
                    if (wins == 1 || wins == DRAW) {
                        unsigned int cardsum = 0;
                        for (unsigned int j = 0; j < DIM; ++j)
                            for (unsigned int k = 0; k < DIM; ++k)
                                if (card[p->card][j][k] < DRAW)
                                    cardsum += card[p->card][j][k];
                        // 1: 28082, 2: 8224
                        printf("Part %d: %u\n", 1 + (wins == DRAW), draw[i] * cardsum);
                    }
                }
            }
            p = p->next;
        }
    }

    for (i = 0; i < DRAW; ++i) {
        p = index[i];
        while (p) {
            q = p->next;
            free(p);
            p = q;
        }
    }
    return 0;
}

