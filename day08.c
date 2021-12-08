#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N 200  // lines
#define M  14  // digits per line
#define L   7  // segments per digit

static char seg[N][M][L + 1] = {0};
static size_t len[N][M] = {0};  // avoid calling strlen every time

// Does A contain B? (unsorted so has to check the whole range every time)
static bool contains(const char *a, const char *b)
{
    while (*b) {
        const char *c = a;
        while (*c && *c != *b)
            c++;
        if (!*c)
            return false;
        b++;
    }
    return true;
}

int main(void)
{
    int ans = 0;
    FILE *f = fopen("input08.txt", "r");
    for (int i = 0; i < N &&
        fscanf(f, "%7s %7s %7s %7s %7s %7s %7s %7s %7s %7s | %7s %7s %7s %7s ",
            seg[i][0], seg[i][1], seg[i][2], seg[i][3], seg[i][4], seg[i][5], seg[i][6], seg[i][7], seg[i][8], seg[i][9],
            seg[i][10], seg[i][11], seg[i][12], seg[i][13]) == 14; ++i) {
        for (int j = 0; j < M; ++j)
            len[i][j] = strlen(seg[i][j]);
        for (int j = 10; j < M; ++j) {
            if ((len[i][j] >= 2 && len[i][j] <= 4) || len[i][j] == 7)
                ++ans;
        }
    }
    fclose(f);
    printf("Part 1: %d\n", ans);  // 534

    // len 2: "1" = ..c..f.
    // len 3: "7" = a.c..f.
    // len 4: "4" = .bcd.f.
    // len 5: "2" = a.cde.g
    //        "5" = ab.d.fg
    //        "3" = a.cd.fg
    // len 6: "0" = abc.efg
    //        "6" = ab.defg
    //        "9" = abcd.fg
    // len 7: "8" = abcdefg

    ans = 0;
    for (int i = 0; i < N; ++i) {
        char seg1[L + 1], seg4[L + 1], seg9[L + 1];  // deciders
        for (int j = 0; j < 10; ++j)
            switch (len[i][j]) {
                case 2: strcpy(seg1, seg[i][j]); break;
                case 4: strcpy(seg4, seg[i][j]); break;  // 4 is needed to find 9
            }
        for (int j = 0; j < 10; ++j)
            if (len[i][j] == 6 && contains(seg[i][j], seg4))
                strcpy(seg9, seg[i][j]);

        int val = 0;
        for (int j = 10; j < M; ++j) {
            val *= 10;
            switch (len[i][j]) {
                case 2: val += 1; break;
                case 3: val += 7; break;
                case 4: val += 4; break;
                case 5:
                    if (contains(seg[i][j], seg1)) {
                        val += 3;
                    } else if (contains(seg9, seg[i][j])) {
                        val += 5;
                    } else {
                        val += 2;
                    }
                    break;
                case 6:
                    if (!contains(seg[i][j], seg1)) {
                        val += 6;
                    } else if (contains(seg[i][j], seg4)) {
                        val += 9;
                    } else {
                        val += 0;
                    }
                    break;
                case 7: val += 8; break;
            }
        }
        ans += val;
    }
    printf("Part 2: %d\n", ans);  // 1070188
    return 0;
}
