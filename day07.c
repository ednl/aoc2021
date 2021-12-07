#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// #define N 10
#define N 1000

// static int data[N] = {16,1,2,0,4,2,7,1,2,14};
static int data[N] = {0};

static int residualsum(int target)
{
    int rs = 0;
    for (int i = 0; i < N; ++i)
        rs += abs(data[i] - target);
    return rs;
}

static int residualexp(int target)
{
    int rs = 0;
    for (int i = 0; i < N; ++i) {
        int d = abs(data[i] - target), d2 = d >> 1;
        rs += (d & 1) ? d2 * d + d : d2 * d + d2;
    }
    return rs;
}

int main(void)
{
    FILE *f = fopen("input07.txt", "r");
    for (int i = 0, a; fscanf(f, "%d", &a) == 1; ) {
        data[i++] = a;
        fgetc(f);
    }
    fclose(f);

    int target = 0;
    for (int i = 0; i < N; ++i)
        target += data[i];
    target = (target + N/2) / N;

    int dir = 1;
    int res = residualsum(target), next = residualsum(target + dir);
    if (next >= res) {
        dir *= -1;
        next = residualsum(target + dir);
    }
    while (next < res) {
        target += dir;
        res = next;
        next = residualsum(target + dir);
    }
    printf("Part 1: %d\n", res);

    dir = 1;
    res = residualexp(target);
    next = residualexp(target + dir);
    if (next >= res) {
        dir *= -1;
        next = residualexp(target + dir);
    }
    while (next < res) {
        target += dir;
        res = next;
        next = residualexp(target + dir);
    }
    printf("Part 2: %d\n", res);
    return 0;
}
