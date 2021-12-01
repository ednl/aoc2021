#include <stdio.h>
#include <limits.h>

#define N 4
static int d[N] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};

int main(void)
{
    FILE *f = fopen("input01.txt", "r");
    int a = 0, b = 0, i = 0, j = N - 1, k = 1;
    while (fscanf(f, "%d ", &d[i]) == 1) {
        a += d[i] > d[j];
        b += d[i] > d[k];
        i = (i + 1) % N;
        j = (j + 1) % N;
        k = (k + 1) % N;
    }
    fclose(f);
    printf("Part 1: %d\n", a);
    printf("Part 2: %d\n", b);
    return 0;
}
