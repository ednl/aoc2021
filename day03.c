#include <stdio.h>

#define N 1000  // lines in input file
#define W 12    // word size (12-bit numbers)

static unsigned int data[N] = {0};

int main(void)
{
    FILE *f = fopen("input03.txt", "r");
    if (!f)
        return 1;

    unsigned int i = 0, j, k;
    while (i < N) {
        k = 0;
        for (j = 0; j < W; ++j) {
            k = (k << 1) | (fgetc(f) == '1');
        }
        data[i++] = k;
        fgetc(f); // newline
    }
    fclose(f);
    if (i != N)
        return 2;

    unsigned int hist[W] = {0};
    for (i = 0; i < N; ++i) {
        k = 1 << W;
        for (j = 0; j < W; ++j) {
            k >>= 1;
            hist[j] += (data[i] & k) != 0;
        }
    }

    unsigned int half = N >> 1, gamma = 0;
    for (i = 0; i < W; ++i) {
        gamma <<= 1;
        gamma |= (hist[i] > half);
    }
    unsigned int epsilon = gamma ^ ((1 << W) - 1);  // flip "W" LSBs in gamma
    printf("Part 1: %u\n", gamma * epsilon);

    k = 1 << W;
    for (j = 0; j < W; ++j) {
        k >>= 1;
        unsigned int remain = N;
        while (k && remain > 1) {
            //
            for (i = 0; i < N; ++i) {
                //
            }
            //
        }
    }

    return 0;
}
