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
    int i = 0;
    FILE *f = fopen("input01.txt", "r");
    while (fscanf(f, "%d ", &depth[i++]) == 1);
    fclose(f);
    printf("Part 1: %d\n", increased(1));
    printf("Part 2: %d\n", increased(3));
    return 0;
}
