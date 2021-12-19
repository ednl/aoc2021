#include <stdio.h>
#include <stdbool.h>

#define STACKSIZE 100000
typedef struct {
    int level, value;
    int prev, next;
} SFN;
static SFN sfn[STACKSIZE] = {0};
static int len = 0, last = 0, sp = 0;

// static void show(void)
// {
//     printf("len=%d last=%d sp=%d\n", len, last, sp);
//     for (int i = 0; i != -1; i = sfn[i].next)
//         printf("%3d", sfn[i].value);
//     printf("\n");
//     for (int i = 0; i != -1; i = sfn[i].next)
//         printf("%3d", sfn[i].level);
//     printf("\n");
// }

// Replace pairs that are too high level with number 0 one level lower
static bool explode(void)
{
    bool found = false;
    const int toohigh = 5;
    for (int i = 0, j, k; i != -1; i = sfn[i].next) {
        if (sfn[i].level == toohigh) {
            sfn[i].level--;
            if ((j = sfn[i].prev) != -1)
                sfn[j].value += sfn[i].value;      // shift left value to the left
            sfn[i].value = 0;                      // reset left value to zero
            if ((j = sfn[i].next) != -1) {
                if ((k = sfn[j].next) == -1) {
                    last = i;                      // adjust endpoint marker
                } else {
                    sfn[k].value += sfn[j].value;  // shift right value to the right
                    sfn[k].prev = i;               // remove right value from linked list (backward)
                }
                sfn[i].next = k;                   // remove right value from linked list (forward)
                --len;                             // number of digits decreased
                found = true;
            } else {
                fprintf(stderr, "Explode: corrupt data\n");
                return false;
            }
        }
    }
    return found;
}

static bool split(void)
{
    for (int i = 0, v; i != -1; i = sfn[i].next) {
        if ((v = sfn[i].value) > 9) {
            if (sp == STACKSIZE) {
                fprintf(stderr, "Split: stack overflow\n");
                return false;
            }
            int j = sp++;                             // allocate new space
            int k = sfn[i].next;                      // save pointer to next node from current
            sfn[i].next = j;                          // insert node (forward)
            sfn[j].level = ++(sfn[i].level);          // level up
            sfn[j].value = v - (sfn[i].value >>= 1);  // distribute value
            sfn[j].prev = i;
            sfn[j].next = k;
            if (k == -1)
                last = j;                             // adjust endpoint marker
            else
                sfn[k].prev = j;                      // insert node (backward)
            ++len;                                    // number of digits increased
            return true;
        }
    }
    return false;
}

// Magnitude of normalised SFN
static int magnitude(void)
{
    for (int level = 4; level > 0; --level) {
        for (int i = 0; i != -1; i = sfn[i].next) {
            if (sfn[i].level == level) {
                int j = sfn[i].next;
                if (j != -1 && sfn[j].level == level) {
                    int k = sfn[j].next;
                    sfn[i].level--;
                    sfn[i].value = 3 * sfn[i].value + 2 * sfn[j].value;
                    sfn[i].next = k;      // remove node (forward)
                    if (k == -1)
                        last = i;         // adjust endpoint marker
                    else
                        sfn[k].prev = i;  // remove node (backward)
                    --len;                // number of digits decreased
                } else {
                    fprintf(stderr, "Magnitude: corrupt data\n");
                    return 0;
                }
            }
        }
    }
    return sfn[0].value;
}

static int read(const char * const fname)
{
    int c, level = 0;
    bool firstline = true;
    FILE *f = fopen(fname, "r");
    while ((c = fgetc(f)) != EOF) {
        switch (c) {
            case '[': ++level; break;
            case ']': --level; break;
            case ',': break;
            case '\n':
                level = 0;                         // reset level counter
                if (!firstline) {                  // SFN has been added to previous sum
                    sfn[last].next = -1;           // terminate list before doing anything with it
                    for (int i = 0; i != -1; i = sfn[i].next)
                        sfn[i].level++;            // level up all nodes because a + b = [a,b]
                    while (explode() || split());  // normalise SFN
                    sfn[last].next = sp;           // point to next free space (last may have changed)
                } else {
                    firstline = false;             // end of first SFN
                }
                break;
            default:
                if (sp == STACKSIZE) {
                    fprintf(stderr, "Stack overflow\n");
                    return 0;
                }
                sfn[sp] = (SFN){level, c - '0', last, sp + 1};  // new entry at first free space
                last = sp++;  // sp+1 and sp++ because before normalisation all nodes are sequential
                ++len;  // number of digits increased
                break;
        }
    }
    fclose(f);
    sfn[last].next = -1;  // terminate
    return sp;
}

int main(void)
{
    read("input18.txt");
    printf("Part 1: %d\n", magnitude());
    return 0;
}
