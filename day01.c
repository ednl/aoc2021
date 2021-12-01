#include <stdio.h>
#include <limits.h>

#define N 2000
static int depth[N] = {0};

int main(void)
{
    FILE *f = fopen("input01.txt", "r");
    if (!f) {
        return 1;
    }

    int n = 0, d;
    while (n < N && fscanf(f, "%d ", &d) == 1) {
        depth[n++] = d;
    }
    fclose(f);
    if (n != N) {
        return 2;
    }

    int inc = 0;
    for (int i = 1; i < N; ++i) {
        inc += depth[i] > depth[i - 1];
    }
    printf("Part 1: %d\n", inc);

    inc = 0;
    for (int i = 3; i < N; ++i) {
        inc += depth[i] > depth[i - 3];
    }
    printf("Part 2: %d\n", inc);

    return 0;
}
