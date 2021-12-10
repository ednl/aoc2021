#include <stdio.h>
#include <stdlib.h>    // qsort
#include <stdint.h>    // uint64_t
#include <inttypes.h>  // PRIu64

#define STACKSIZE 16  // 15 for my input
#define INCSIZE   48  // 47 for my input
static int stack[STACKSIZE] = {0};
static uint64_t incomplete[INCSIZE] = {0};

// Sort uint64 array in ascending order
static int asc(const void *a, const void *b)
{
    const uint64_t p = *(const uint64_t*)a;
    const uint64_t q = *(const uint64_t*)b;
    return (q < p) - (p < q);
}

int main(void)
{
    int c, id, score = 0, syntaxerror = 0;
    size_t sp = 0, ip = 0;
    FILE *f = fopen("input10.txt", "r");

    while (!feof(f)) {
        switch ((c = fgetc(f))) {
            case '(' : id = 0x01; break;
            case '[' : id = 0x02; break;
            case '{' : id = 0x04; break;
            case '<' : id = 0x08; break;
            case ')' : id = 0x10; score = 3; break;
            case ']' : id = 0x20; score = 57; break;
            case '}' : id = 0x40; score = 1197; break;
            case '>' : id = 0x80; score = 25137; break;
            case '\n': id = 0xff; break;
            default  : id = 0x00; break;
        }

        if (id == 0xff) {                              // newline?
            if (sp) {                                  // incomplete?
                if (ip != INCSIZE) {                   // room on the stack?
                    uint64_t i = 0;                    // determine incomplete score
                    while (sp) {
                        i *= 5;
                        switch (stack[--sp]) {         // pop expected value
                            case 0x10: i += 1; break;  // ')'
                            case 0x20: i += 2; break;  // ']'
                            case 0x40: i += 3; break;  // '}'
                            case 0x80: i += 4; break;  // '>'
                        }
                    }
                    incomplete[ip++] = i;              // save incomplete score
                } else {
                    fprintf(stderr, "Incomplete score stack overflow");
                    return 1;
                }
            }
        } else if (id & 0x00f) {                       // opening bracket?
            if (sp != STACKSIZE) {                     // room on the stack?
                stack[sp++] = id << 4;                 // push expectation
            } else {
                fprintf(stderr, "Bracket stack overflow");
                return 2;
            }
        } else if (id & 0x0f0) {                       // closing bracket?
            if (!sp || stack[--sp] != id) {            // empty stack or no match?
                syntaxerror += score;                  // count corruption
                while (c != '\n' && !feof(f))          // discard rest of line
                    c = fgetc(f);
                sp = 0;                                // start a new line
            }
        }
    }
    fclose(f);
    qsort(incomplete, ip, sizeof *incomplete, asc);

    printf("Part 1: %d\n", syntaxerror);               // 413733
    printf("Part 2: %"PRIu64"\n", incomplete[ip/2]);   // 3354640192
    return 0;
}
