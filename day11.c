#include <stdio.h>
#include <stdbool.h>

#define STEPS 100
#define FLASH 10
#define DIM 10
static int lum[DIM + 2][DIM + 2] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,5,4,8,3,1,4,3,2,2,3,0},
    {0,2,7,4,5,8,5,4,7,1,1,0},
    {0,5,2,6,4,5,5,6,1,7,3,0},
    {0,6,1,4,1,3,3,6,1,4,6,0},
    {0,6,3,5,7,3,8,5,4,7,8,0},
    {0,4,1,6,7,5,2,4,6,4,5,0},
    {0,2,1,7,6,8,4,1,7,2,1,0},
    {0,6,8,8,2,8,8,1,1,3,4,0},
    {0,4,8,4,6,8,4,8,5,5,4,0},
    {0,5,2,8,3,7,5,1,5,2,6,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
};
static bool flashed[DIM + 2][DIM + 2] = {0};

static int cascade(void)
{
    for (int i = 1; i <= DIM; ++i)
        for (int j = 1; j <= DIM; ++j) {
            lum[i][j]++;
            flashed[i][j] = false;
        }
    bool check = true;
    while (check) {
        check = false;
        for (int i = 1; i <= DIM; ++i)
            for (int j = 1; j <= DIM; ++j)
                if (lum[i][j] >= FLASH && !flashed[i][j]) {
                    check = true;
                    flashed[i][j] = true;
                    for (int x = i - 1; x <= i + 1; ++x)
                        for (int y = j - 1; y <= j + 1; ++y)
                            lum[x][y]++;
                }
    }
    int n = 0;
    for (int i = 1; i <= DIM; ++i)
        for (int j = 1; j <= DIM; ++j)
            if (lum[i][j] >= FLASH) {
                lum[i][j] = 0;
                ++n;
            }
    return n;
}

static bool all(void)
{
    for (int i = 1; i <= DIM; ++i)
        for (int j = 1; j <= DIM; ++j)
            if (!flashed[i][j])
                return false;
    return true;
}

int main(void)
{
    FILE *f = fopen("input11.txt", "r");
    for (int i = 1; i <= DIM; ++i) {
        for (int j = 1; j <= DIM; ++j)
            lum[i][j] = (unsigned char)(fgetc(f) - '0');
        fgetc(f);  // newline
    }
    fclose(f);

    int step = 0, total = 0;
    while (step < STEPS) {
        ++step;
        total += cascade();
    }
    printf("Part 1: %d\n", total);
    while (!all()) {
        ++step;
        cascade();
    }
    printf("Part 2: %d\n", step);
    return 0;
}
