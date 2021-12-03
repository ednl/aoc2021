#include <stdio.h>

#define N 1000
#define L 12
#define W "12"

// typedef struct {
//     int cmd, arg;
// } Nav;
// static Nav nav[N] = {0};

int main(void)
{
    FILE *f = fopen("input03.txt", "r");
    if (!f)
        return 1;

    unsigned int i = 0, j, bin[L] = {0};
    char word[L + 1];
    while (i < N && fscanf(f, "%"W"s ", word) == 1) {
        for (j = 0; j < L; ++j) {
            bin[j] += word[j] == '1';
        }
        ++i;
    }
    fclose(f);
    if (i != N)
        return 2;

    for (i = 0; i < L; ++i) {
        printf("%4u", bin[i]);
    }
    printf("\n");

    unsigned int threshold = N >> 1, gamma = 0;
    for (i = 0; i < L; ++i) {
        gamma <<= 1;
        gamma |= (bin[i] > threshold);
    }
    unsigned int epsilon = gamma ^ ((1 << L) - 1);
    printf("Part 1: %u\n", gamma * epsilon);
    return 0;
}
