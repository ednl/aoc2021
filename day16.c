#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>

#define N      1396     // hex chars in input
#define NIBBLE    4     // hex char: 4 bits
#define M (N * NIBBLE)  // bits in input
#define VTLEN     3     // version/type: 3 bits
#define SUBLEN   11
#define BITLEN   15

static bool bit[M] = {0};
static int versionsum = 0;

static int64_t getnumber(unsigned int *index, unsigned int len)
{
    int64_t n = 0;
    while (len--)
        n = (n << 1) | bit[(*index)++];
    return n;
}

static int64_t getliteral(unsigned int *index)
{
    int64_t val = 0;
    bool more;
    do {
        more = bit[(*index)++];
        val = (val << NIBBLE) | getnumber(index, NIBBLE);
    } while (more);
    return val;
}

static inline int64_t min(int64_t a, int64_t b)
{
    return a < b ? a : b;
}

static inline int64_t max(int64_t a, int64_t b)
{
    return a > b ? a : b;
}

// Read packet and subpackets, updates global versionsum, returns number of bit consumed
static int64_t getpacket(unsigned int *index)
{
    versionsum += (int)getnumber(index, VTLEN);
    int8_t typeid = (int8_t)getnumber(index, VTLEN);
    if (typeid == 4)
        return getliteral(index);

    bool lengthid = bit[(*index)++];
    int maxsubs, maxbits;
    if (lengthid) {
        maxsubs = (int)getnumber(index, SUBLEN);
        maxbits = INT_MAX;
    } else {
        maxsubs = INT_MAX;
        maxbits = (int)getnumber(index, BITLEN);
    }
    int64_t val = 0;
    if (typeid < 4) {
        switch (typeid) {
            case 1: val = 1; break;  // prod
            case 2: val = INT64_MAX; break;  // min
            case 3: val = INT64_MIN; break;  // max
        }
        while (maxsubs > 0 && maxbits > 0) {
            int i = (int)*index;
            int64_t a = getpacket(index);
            switch (typeid) {
                case 0: val += a; break;  // sum
                case 1: val *= a; break;  // prod
                case 2: val = min(val, a); break;  // min
                case 3: val = max(val, a); break;  // max
            }
            maxsubs--;
            maxbits -= (int)*index - i;
        }
    } else {
        int64_t a = getpacket(index);
        int64_t b = getpacket(index);
        switch (typeid) {
            case 5: val = a > b; break;  // gt
            case 6: val = a < b; break;  // lt
            case 7: val = a == b; break;  // eq
        }
    }
    return val;
}

int main(void)
{
    FILE *f = fopen("input16.txt", "r");
    char hex[17] = {0};
    int k = 0;
    while (fscanf(f, "%16s", hex) == 1) {
        char *end;
        uint64_t val = strtoull(hex, &end, 16);
        ssize_t bitlen = (end - hex) * 4;
        uint64_t mask = (1ULL << (bitlen - 1));
        while (mask) {
            bit[k++] = (val & mask) != 0;
            mask >>= 1;
        }
    }
    fclose(f);

    unsigned int i = 0;
    int64_t part2 = getpacket(&i);
    printf("Part 1: %d\n", versionsum);  // 936
    printf("Part 2: %lld\n", part2);     // ?
    return 0;
}
