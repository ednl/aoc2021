#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define N      1396       // hex chars in input
#define HLEN   4          // hex char: 4 bits
#define M     (N * HLEN)  // bits in input
#define VTLEN  3          // version/type: 3 bits
#define SUBLEN 11
#define BITLEN 15

static bool bit[M] = {0};
static int versionsum = 0;

// type  4 = V,T,literal value (=zero or more of "1"+4bits, one final "0"+4bits)
// type !4 = V,T,length bit+info,1 or more subpackets
// length bit = 0: info = 15 bits total length of all subpackets
// length bit = 1: info = 11 bits number of subpackets

// Starts at index, consumes len bits, returns number
static uint64_t getnumber(const int index, const int len)
{
    uint64_t n = 0;
    for (int i = index; i < index + len && i < M; ++i)
        n = (n << 1) | bit[i];
    return n;
}

// Sets *val, returns number of bits consumed
static int getliteral(uint64_t * const val, const int index)
{
    *val = 0;
    int i = index, nibbles = 0;
    bool more = true;
    while (more && nibbles < 16 && i < M - HLEN) {  // 16 * 4 = 64
        more = bit[i++];  // 1-bit prefix of hex char (0=final, 1=there's more)
        *val = (*val << HLEN) | getnumber(i, HLEN);
        i += HLEN;
        ++nibbles;
    }
    return i - index;
}

// Read packet and subpackets, updates global versionsum, returns number of bit consumed
static int getpacket(const int index)
{
    int i = index;
    unsigned char version = 0, typeid = 8;  // 8 = invalid

    if (i < M - VTLEN) {
        version = (unsigned char)getnumber(i, VTLEN);
        printf("%4d: version = %u\n", i, version);
        i += VTLEN;
        versionsum += (int)version;
    }
    if (i < M - VTLEN) {
        typeid = (unsigned char)getnumber(i, VTLEN);
        printf("%4d: type = %u\n", i, version);
        i += VTLEN;
    }
    uint64_t val;
    if (typeid == 4) {
        i += getliteral(&val, i);
        printf("%4d: val = %llu\n", i, val);
    } else if (typeid < 8) {
        if (bit[i++]) {
            // Length type ID = 1 (11 bits = number of subpackets)
            int subs = (int)getnumber(i, SUBLEN);
            printf("%4d: subs = %d\n", i, subs);
            i += SUBLEN;
            for (int j = 0; j < subs; ++j)
                i += getpacket(i);
        } else {
            // Length type ID = 0 (15 bits = length of all subpackets)
            int bits = (int)getnumber(i, BITLEN);
            printf("%4d: bits = %d\n", i, bits);
            i += BITLEN;
            for (const int start = i; i < start + bits; i += getpacket(i));
        }
    }
    return i - index;
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
        uint64_t mask = 1 << (bitlen - 1);
        while (mask) {
            bit[k++] = val & mask;
            mask >>= 1;
        }
    }
    fclose(f);
    printf("k=%d k/4=%d\n", k, k / 4);

    int index = 0;
    while (index < M)
        index += getpacket(index);
    printf("Part 1: %d\n", versionsum);  // ?

    return 0;
}
