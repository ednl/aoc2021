#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LINES    24
#define MAXNODES 16
#define MAXEDGES  8
#define START     1
#define END       2
#define MAXSP   100
#define MAXVISITS 1

typedef struct Node {
    int id, visits, edges, edge[MAXEDGES];
    bool small;
} Node;

static Node node[MAXNODES] = {0};
static int nodes = 0, start = 0, end = 0;
static int inp[LINES * 2] = {0};
static int stack[MAXSP] = {0};

static int make_id(const char *a)
{
    if (!strcmp(a, "start"))
        return START;
    if (!strcmp(a, "end"))
        return END;
    int id = 0;
    const char *c = a;
    while (*c)
        id = (id << 8) | *c++;
    return id;
}

static void print_id(int id)
{
    if (id == START)
        printf("start");
    else if (id == END)
        printf("end");
    else {
        char buf[6] = {0};
        int i = 0;
        while (id && i < 6) {
            buf[i++] = (char)(id & 127);
            id >>= 8;
        }
        while (i)
            printf("%c", buf[--i]);
    }
}

static int find_index(int id)
{
    for (int i = 0; i < nodes; ++i)
        if (node[i].id == id)
            return i;
    return -1;
}

static int add_node(char *name)
{
    int id = make_id(name);
    int i = find_index(id);
    if (i != -1)
            return i;
    if (nodes == MAXNODES) {
        fprintf(stderr, "Too many nodes.");
        return -1;
    }
    i = nodes++;
    node[i].id = id;
    node[i].small = *name >= 'a';
    if (id == START)
        start = i;
    else if (id == END)
        end = i;
    return i;
}

static void add_edge(int from, int to)
{
    if (from != -1 && to != -1 && from != end && to != start) {
        for (int i = 0; i < node[from].edges; ++i)
            if (node[from].edge[i] == to)
                return;
        if (node[from].edges == MAXEDGES) {
            fprintf(stderr, "Too many edges.");
            return;
        }
        node[from].edge[node[from].edges++] = to;
    }
}

static void print_graph(void)
{
    for (int i = 0; i < nodes; ++i) {
        print_id(node[i].id);
        if (node[i].edges) {
            printf(" -> ");
            print_id(node[node[i].edge[0]].id);
            for (int j = 1; j < node[i].edges; ++j) {
                printf(",");
                print_id(node[node[i].edge[j]].id);
            }
        }
        printf("\n");
    }
}

int main(void)
{
    char buf[2][6];
    FILE *f = fopen("input12-ex1.txt", "r");  // Part 1: 10
    // FILE *f = fopen("input12-ex2.txt", "r");  // Part 1: 19
    // FILE *f = fopen("input12-ex3.txt", "r");  // Part 1: 226
    // FILE *f = fopen("input12.txt", "r");
    int n = 0;
    while (fscanf(f, "%5[A-Za-z]-%5[A-Za-z] ", buf[0], buf[1]) == 2) {
        inp[n++] = add_node(buf[0]);
        inp[n++] = add_node(buf[1]);
    }
    fclose(f);
    for (int i = 0; i < n; i += 2) {
        add_edge(inp[i], inp[i + 1]);
        add_edge(inp[i + 1], inp[i]);
    }

    print_graph();

    int paths = 0, sp = 1, maxsp = 0;
    stack[0] = start;
    while (sp) {
        int i = stack[--sp];
        for (int j = 0; j < node[i].edges; ++j) {
            int k = node[i].edge[j];
            if (k == end)
                ++paths;
            else if (!node[k].small)
                stack[sp++] = k;
            else if (node[k].visits < MAXVISITS) {
                node[k].visits++;
                stack[sp++] = k;
            }
        }
        if (sp > maxsp)
            maxsp = sp;
    }
    printf("maxsp=%d\n", maxsp);
    printf("Part 1: %d\n", paths);

    return 0;
}
