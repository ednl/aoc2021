#include <stdio.h>
#include <stdbool.h>

#define W 139
#define H 137
static char a[2][H][W] = {0};

int main(void)
{
    // Read input file
    FILE *f = fopen("input25.txt", "r");
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j)
            a[0][i][j] = (char)fgetc(f);
        fgetc(f);
    }
    fclose(f);

    bool moved = false;
    // Move east
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            if (a[0][i][j] == '>') {
                int k = (j + 1) % W;
                if (a[0][i][k] == '.') {
                    a[1][i][j] = '.';
                    a[1][i][k] = '>';
                    ++j;
                    moved = true;
                } else {
                    a[1][i][j] = '>';
                }
            } else
                a[1][i][j] = a[0][i][j];
    for (int i = 0; i < H; ++i)
        a[0][i][0] = a[0][i][W];

    return 0;
}
