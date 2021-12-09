#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DIM        100
#define MAXHEIGHT    9
#define MAXBASINS  256
#define STACKSIZE  100

typedef struct {
    int x, y;
} Point;

static unsigned char height[DIM + 2][DIM + 2] = {0};  // extend grid to simplify edge cases
static bool done[DIM + 2][DIM + 2] = {0};  // keep track of already processed locations
static Point todo[STACKSIZE] = {0};  // keep track of locations still to be processed
static int basin[MAXBASINS] = {0};  // size of each basin found

// Floodfill the basin, return basin size
static int fillbasin(int x, int y)
{
    int sp = 1, size = 0;     // sp = stack pointer
    todo[0] = (Point){x, y};  // prefill stack with first location
    while (sp) {
        Point p = todo[--sp];
        if (!done[p.x][p.y]) {
            done[p.x][p.y] = true;
            if (height[p.x][p.y] < MAXHEIGHT) {
                if (sp >= STACKSIZE - 4) {
                    printf("Stack overflow!");
                    return -1;
                }
                if (!done[p.x + 1][p.y]) todo[sp++] = (Point){p.x + 1, p.y};
                if (!done[p.x - 1][p.y]) todo[sp++] = (Point){p.x - 1, p.y};
                if (!done[p.x][p.y + 1]) todo[sp++] = (Point){p.x, p.y + 1};
                if (!done[p.x][p.y - 1]) todo[sp++] = (Point){p.x, p.y - 1};
                ++size;
            }
        }
    }
    return size;
}

// Sort int array in reverse order
static int larger(const void *a, const void *b)
{
    const int p = *(const int*)a;
    const int q = *(const int*)b;
    return (p < q) - (q < p);
}

int main(void)
{
    FILE *f = fopen("input09.txt", "r");
    for (int i = 1; i <= DIM; ++i) {
        for (int j = 1; j <= DIM; ++j)
            height[i][j] = (unsigned char)(fgetc(f) - '0');
        fgetc(f);  // newline
    }
    fclose(f);

    // Border
    for (int i = 0; i < DIM + 2; ++i)
        height[i][0] = height[0][i] = height[i][DIM + 1] = height[DIM + 1][i] = MAXHEIGHT + 1;

    int risk = 0;
    size_t count = 0;
    for (int i = 1; i <= DIM; ++i) {
        for (int j = 1; j <= DIM; ++j) {
            if (height[i][j] < height[i - 1][j] &&
                height[i][j] < height[i][j - 1] &&
                height[i][j] < height[i + 1][j] &&
                height[i][j] < height[i][j + 1]) {
                risk += height[i][j] + 1;
                basin[count++] = fillbasin(i, j);
                if (count == MAXBASINS) {
                    printf("Too many damn basins!");
                    return 1;
                }
            }
        }
    }
    qsort(basin, count, sizeof *basin, larger);

    printf("Part 1: %d\n", risk);  // 506
    printf("Part 2: %d\n", basin[0] * basin[1] * basin[2]);  // 931200
    return 0;
}
