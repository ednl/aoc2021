#include <stdio.h>

#define N 1000
typedef struct {
    int dir, val;
} Nav;
static Nav nav[N] = {0};

int main(void)
{
    FILE *f = fopen("input02.txt", "r");
    if (!f)
        return 1;

    int i = 0, c, dir, val;
    while (i < N && !feof(f)) {
        dir = c = fgetc(f);
        while (c >= 'a')
            c = fgetc(f);
        if (fscanf(f, "%d ", &val) == 1)
            nav[i++] = (Nav){dir, val};
    }
    fclose(f);
    if (i != N)
        return 2;

    int fwd = 0, depth = 0;
    for (i = 0; i < N; ++i) {
        switch (nav[i].dir) {
            case 'f': fwd += nav[i].val; break;
            case 'd': depth += nav[i].val; break;
            case 'u': depth -= nav[i].val; break;
        }
    }
    printf("Part 1: %d\n", fwd * depth);

    fwd = depth = 0;
    int aim = 0;
    for (i = 0; i < N; ++i) {
        switch (nav[i].dir) {
            case 'f': fwd += nav[i].val; depth += aim * nav[i].val; break;
            case 'd': aim += nav[i].val; break;
            case 'u': aim -= nav[i].val; break;
        }
    }
    printf("Part 2: %d\n", fwd * depth);
    return 0;
}
