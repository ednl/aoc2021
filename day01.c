#include <stdio.h>

#define N 2000
static int depth[N] = {0};

static int increased(int window)
{
    int n = 0;
    for (int i = window; i < N; ++i)
        n += depth[i] > depth[i - window];
    return n;
}

int main(void)
{
    FILE *f = fopen("input01.txt", "r");
    if (!f)
        return 1;

    int i = 0;
    while (i < N && fscanf(f, "%d ", &depth[i]) == 1)
        ++i;
    fclose(f);
    if (i != N)
        return 2;

    printf("Part 1: %d\n", increased(1));
    printf("Part 2: %d\n", increased(3));
    return 0;
}
