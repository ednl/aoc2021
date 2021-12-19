#include <stdio.h>
#include <stdbool.h>

static const char ex1[] = "[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]";
// original:      [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]
// 5 ( 4, 3) <-- explode
// 4 (-1, 4)
// 3 (-1, 4)
// 3 ( 7,-1)
// 5 ( 8, 4)
// 4 (-1, 9)
// 2 ( 1, 1)
// after explode: [[[[0,7],4],[7,[[8,4],9]]],[1,1]]
// 4 ( 0, 7)
// 3 (-1, 4)
// 3 ( 7,-1)
// 5 ( 8, 4) <-- explode
// 4 (-1, 9)
// 2 ( 1, 1)
// after explode: [[[[0,7],4],[15,[0,13]]],[1,1]]
// 4 ( 0, 7)
// 3 (-1, 4)
// 3 (15,-1) <-- split
// 4 ( 0,13)
// 2 ( 1, 1)
// after split:   [[[[0,7],4],[[7,8],[0,13]]],[1,1]]
// 4 ( 0, 7)
// 3 (-1, 4)
// 4 ( 7, 8)
// 4 ( 0,13) <-- split
// 2 ( 1, 1)
// after split:   [[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]
// 4 ( 0, 7)
// 3 (-1, 4)
// 4 ( 7, 8)
// 4 ( 0,-1)
// 5 ( 6, 7) <-- explode
// 2 ( 1, 1)
// after explode: [[[[0,7],4],[[7,8],[6,0]]],[8,1]]
// 4 ( 0, 7)
// 3 (-1, 4)
// 4 ( 7, 8)
// 4 ( 6, 0)
// 2 ( 8, 1)

typedef struct {
    int level, val[2];
} Pair;
static Pair pair[100] = {0};

int main(void)
{
    const char *c = ex1;
    int level = 0, i = 0;
    bool first = true;
    while (*c) {
        // [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]
        // 5:  4, 3
        // 4: -1, 4
        // 3: -1, 4
        // 3:  7,-1
        // 5:  8, 4
        // 4: -1, 9
        // 2:  1, 1
        switch (*c) {
            case '[':
                ++level;
                first = true;
                break;
            case ']':
                --level;
                first = true;
                break;
            case ',':
                first = false;
                break;
            default:
                pair[i].level = level;
                pair[i].val[first] = *c - '0';
                // TODO: not good:
                if (!first)
                    ++i;
                break;
        }
        ++c;
    }
    return 0;
}
