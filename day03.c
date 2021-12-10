#include <stdio.h>
#include <stdbool.h>

#define N 1000  // lines in input file
#define W 12    // word size (12-bit numbers)
#define S "12"  // word length in format string

typedef struct Data {
    unsigned int val;
    char word[W + 1];
    bool selected;
} Data;
static Data data[N] = {0};

// What's the most or least common bit at index 'pos'?
// most==true : choose the most  common bit, if equal prefer 1
// most==false: choose the least common bit, if equal prefer 0
static char common(int pos, bool most)
{
    int remaining = 0, ones = 0;
    for (int i = 0; i < N; ++i) {
        if (data[i].selected) {
            ++remaining;
            ones += data[i].word[pos] == '1';
        }
    }
    int zeros = remaining - ones;
    return ones >= zeros ? '0' + most : '1' - most;
}

int main(void)
{
    FILE *f = fopen("input03.txt", "r");
    for (int i = 0; i < N && fscanf(f, "%"S"s ", data[i].word) == 1; ++i)
        data[i].selected = true;
    fclose(f);

    int gamma = 0;
    for (int i = 0; i < W; ++i) {
        gamma <<= 1;
        gamma |= common(i, true) == '1';
    }
    int epsilon = gamma ^ ((1<<W) - 1);
    printf("Part 1: %d\n", gamma * epsilon);  // 4147524

    return 0;
}
