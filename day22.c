#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define N 101  // size of dimensions part 1
#define M  20  // cube count part 1
static bool cube[N << 14] = {0};

#define N2 7000  // max 6861 for my input
#define M2 1300  // max 1252 for my input
typedef struct {
    int x0, x1, y0, y1, z0, z1, count;
} Cube;
static Cube cub[N2] = {0};
static Cube upd[N2] = {0};

int main(void)
{
    FILE *f = fopen("input22.txt", "r");
    char buf[4];
    int x0, x1, y0, y1, z0, z1;
    for (int n = 0; n < M && fscanf(f, "%3s x=%d..%d,y=%d..%d,z=%d..%d ", buf, &x0, &x1, &y0, &y1, &z0, &z1) == 7; ++n) {
        x0 += 50; x1 += 51;
        y0 += 50; y1 += 51;
        z0 += 50; z1 += 51;
        for (int x = x0; x < x1; ++x) {
            int i = x << 14;
            for (int y = y0; y < y1; ++y) {
                int j = i | (y << 7);
                for (int z = z0; z < z1; ++z)
                    cube[j | z] = !buf[2];
            }
        }
    }
    fclose(f);
    int64_t on = 0;
    for (int i = 0; i < N << 14; ++i)
        on += cube[i];
    printf("Part 1: %lld\n", on);  // 547648

    f = fopen("input22.txt", "r");
    int n = 0, m;
    while (fscanf(f, "%3s x=%d..%d,y=%d..%d,z=%d..%d ", buf, &x0, &x1, &y0, &y1, &z0, &z1) == 7) {
        m = 0;
        for (int i = 0; i < n; ++i) {
            if (cub[i].count) {
                Cube *c = &cub[i];  // convenience reference to existing cube
                Cube d = (Cube){  // intersection of existing and new cube
                    x0 >= c->x0 ? x0 : c->x0, x1 <= c->x1 ? x1 : c->x1,
                    y0 >= c->y0 ? y0 : c->y0, y1 <= c->y1 ? y1 : c->y1,
                    z0 >= c->z0 ? z0 : c->z0, z1 <= c->z1 ? z1 : c->z1,
                    -c->count};  // opposite count to existing
                if (d.x0 <= d.x1 && d.y0 <= d.y1 && d.z0 <= d.z1) {  // intersection not empty?
                    int j = 0;
                    while (j < m && (  // find same cube in current update array
                        d.x0 != upd[j].x0 || d.x1 != upd[j].x1 ||
                        d.y0 != upd[j].y0 || d.y1 != upd[j].y1 ||
                        d.z0 != upd[j].z0 || d.z1 != upd[j].z1)) {
                        ++j;
                    }
                    if (j < m) {
                        // add to existing
                        upd[j].count += d.count;
                    } else {
                        // add new
                        if (j == M2) {
                            fprintf(stderr, "Cube upd overflow\n");
                            return 1;
                        }
                        upd[m++] = d;
                    }
                }
            }
        }
        if (buf[2] == '\0') {  // "on" => add new cube to upd array
            int j = 0;
            while (j < m && (  // find same cube in current update array
                x0 != upd[j].x0 || x1 != upd[j].x1 ||
                y0 != upd[j].y0 || y1 != upd[j].y1 ||
                z0 != upd[j].z0 || z1 != upd[j].z1)) {
                ++j;
            }
            if (j < m) {
                // add to existing
                upd[j].count++;
            } else {
                // add new
                if (j == M2) {
                    fprintf(stderr, "Cube upd overflow\n");
                    return 2;
                }
                upd[m++] = (Cube){x0, x1, y0, y1, z0, z1, 1};
            }
        }
        // Add upd array to main cube array
        int nn = n;
        for (int i = 0; i < m; ++i) {  // for every cube in the upd array
            Cube *c = &upd[i];
            int j = 0;
            while (j < n && (  // find same cube in main cube array
                c->x0 != cub[j].x0 || c->x1 != cub[j].x1 ||
                c->y0 != cub[j].y0 || c->y1 != cub[j].y1 ||
                c->z0 != cub[j].z0 || c->z1 != cub[j].z1)) {
                ++j;
            }
            if (j < n) {
                // add to existing
                cub[j].count += c->count;
            } else {
                // add new
                if (nn == N2) {
                    fprintf(stderr, "Cube main overflow\n");
                    return 2;
                }
                cub[nn++] = *c;
            }
        }
        n = nn;
    }
    fclose(f);
    on = 0;
    for (int i = 0; i < n; ++i) {
        Cube *c = &cub[i];
        on += (int64_t)c->count * (c->x1 - c->x0 + 1) * (c->y1 - c->y0 + 1) * (c->z1 - c->z0 + 1);
    }
    printf("Part 2: %lld\n", on);  // 1206644425246111
    return 0;
}
