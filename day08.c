#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 200
#define IN 10
#define OUT 4
#define L   7
static char inp[N][IN ][L + 1] = {0};
static char out[N][OUT][L + 1] = {0};
static bool possib[L][L] = {0};
static char pcount[L] = {0};
static char wire[L] = {0};

static int cmp(const void *a, const void *b)
{
    char p = *(const char*)a;
    char q = *(const char*)b;
    return (q < p) - (p < q);
}

int main(void)
{
    int ans = 0;
    FILE *f = fopen("input08.txt", "r");
    for (int i = 0; i < N &&
        fscanf(f, "%7s %7s %7s %7s %7s %7s %7s %7s %7s %7s | %7s %7s %7s %7s ",
            inp[i][0], inp[i][1], inp[i][2], inp[i][3], inp[i][4], inp[i][5], inp[i][6], inp[i][7], inp[i][8], inp[i][9],
            out[i][0], out[i][1], out[i][2], out[i][3]) == 14; ++i) {
        for (int j = 0; j < OUT; ++j) {
            size_t k = strlen(out[i][j]);
            if (k == 2 || k == 4 || k == 3 || k == 7)
                ++ans;
        }
    }
    fclose(f);
    printf("Part 1: %d\n", ans);

    // num:  000000   111111   222222   333333   444444   555555   666666   777777   888888   999999
    //
    //        aaaa              aaaa     aaaa              aaaa     aaaa     aaaa     aaaa     aaaa
    //       b    c        c        c        c   b    c   b        b             c   b    c   b    c
    //       b    c        c        c        c   b    c   b        b             c   b    c   b    c
    //                          dddd     dddd     dddd     dddd     dddd              dddd     dddd
    //       e    f        f   e             f        f        f   e    f        f   e    f        f
    //       e    f        f   e             f        f        f   e    f        f   e    f        f
    //        gggg              gggg     gggg              gggg     gggg              gggg     gggg
    //
    // len:       6        2        5        5        4        5        6        3        7        6
    //
    // len 2: "1" = cf                              (not: abdeg)
    // len 3: "7" = acf                             (not: bdeg )
    // len 4: "4" = bcdf                            (not: aeg  )
    // len 5: "2" = acdeg, "3" = acdfg, "5" = abdfg (common: adg  , unique: be, twice: cf)
    // len 6: "0" = abcefg, "6" = abdefg            (common: abcfg, unique: de)
    // len 7: "8" = abcdefg

    ans = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < IN; ++j)
            qsort(inp[i][j], strlen(inp[i][j]), 1, cmp);
        for (int j = 0; j < OUT; ++j)
            qsort(out[i][j], strlen(out[i][j]), 1, cmp);
    }

    for (int i = 0; i < 1; ++i) {
        int val = 0;
        for (int j = 0; j < L; ++j) {
            wire[j] = -1;  // nothing known yet
            for (int k = 0; k < L; ++k)
                possib[j][k] = true;  // all is still possible
            pcount[j] = L;
        }
        for (int j = 0; j < OUT; ++j) {
            size_t k = strlen(out[i][j]);
            if (k == 2) { // len 2 = "1" = cf
                const char elim[] = "abdeg";
                const char *e = elim;
                while (*e) {
                    int p = *e - 'a';
                    for (int l = 0; l < k; ++l) {
                        int q = out[i][j][l] - 'a';
                        if (possib[p][q]) {
                            possib[p][q] = false;  // p is not connected to q
                            pcount[p]--;
                        }
                    }
                    ++e;
                }
            }
            else if (k == 3) { // len 3 = "7" = acf
                for (int l = 0; l < k; ++l) {
                    int w = out[i][j][l] - 'a';
                    possib['b' - 'a'][w] = false;  // not b
                    possib['d' - 'a'][w] = false;  // not d
                    possib['e' - 'a'][w] = false;  // not e
                    possib['g' - 'a'][w] = false;  // not g
                }
            }
            else if (k == 4) { // len 4 = "4" = bcdf
                for (int l = 0; l < k; ++l) {
                    int w = out[i][j][l] - 'a';
                    possib['a' - 'a'][w] = false;  // not a
                    possib['e' - 'a'][w] = false;  // not e
                    possib['g' - 'a'][w] = false;  // not g
                }
            }

            bool changed = true;
            while (changed) {
                changed = false;
                for (int l = 0; l < L; ++l) {
                    if (pcount[l] == 1 && wire[l] == -1) {
                        int m = 0;
                        while (!possib[l][m])
                            ++m;
                        wire[l] = m;
                        for (int n = 0; n < L; ++n) {
                            if (n != l) {
                                if (possib[n][m]) {
                                    possib[n][m] = false;
                                    changed = true;
                                }
                            }
                        }
                    } else if (pcount[l] == 2) {
                        // TODO
                    }
                }
            }

        }

        int fixed = 0;
        for (int j = 0; j < L; ++j) {
            fixed += wire[j] != -1;
        }
        if (fixed == L) {
            // TODO: compute val
            ans += val;
        } else {
            printf("  abcdefg\n");
            for (int j = 0; j < L; ++j) {
                printf("%c ", 'a' + j);
                for (int k = 0; k < L; ++k) {
                    printf("%c", possib[j][k] ? 'x' : '.');
                }
                printf("\n");
            }
            printf("wire: ");
            for (int j = 0; j < L; ++j) {
                printf("%c", wire[j] == -1 ? '.' : 'a' + wire[j]);
            }
            printf("\n");
        }
    }

    printf("Part 2: %d\n", ans);
    return 0;
}