#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MEMSIZE   256
#define STACKSIZE  14
#define REGCOUNT  ('z' - 'w' + 1)

static const char *lang[] = {"nop", "inp", "add", "mul", "div", "mod", "eql"};
typedef enum {
    NOP = 0, INP, ADD, MUL, DIV, MOD, EQL
} OpCode;

typedef struct {
    OpCode op;
    int arg[2];
    bool imm;
} Instr;

typedef struct {
    int ip, sp, reg[REGCOUNT], stack[STACKSIZE];
    Instr mem[MEMSIZE];
    bool run;
} VM;

static VM alu = {0};

static bool run(VM *vm, int64_t model)
{
    vm->sp = 0;
    while (vm->sp < STACKSIZE) {
        vm->stack[vm->sp++] = model % 9 + 1;
        model /= 9;
    }
    vm->run = true;
    while (vm->run && vm->ip < MEMSIZE) {
        Instr *i = &vm->mem[vm->ip++];
        OpCode op = i->op;
        int r = i->arg[0], val = i->arg[1];
        if (op > INP && !i->imm && val >= 0 && val < REGCOUNT)
            val = vm->reg[val];
        switch (i->op) {
            case NOP: vm->run = false; break;
            case INP: vm->reg[r] = vm->sp ? vm->stack[--(vm->sp)] : 0; break;
            case ADD: vm->reg[r] += val; break;
            case MUL: vm->reg[r] *= val; break;
            case DIV: vm->reg[r] /= val; break;
            case MOD: vm->reg[r] %= val; break;
            case EQL: vm->reg[r] = vm->reg[r] == val; break;
        }
    }
    return vm->reg['z' - 'w'] == 0;
}

static int64_t model(int64_t base10)
{
    int64_t n = 0, d = 1;
    for (int i = 0; i < STACKSIZE; ++i) {
        n += d * (base10 % 9 + 1);
        base10 /= 9;
        d *= 10;
    }
    return n;
}

static int64_t base10(int64_t model)
{
    int64_t n = 0, d = 1;
    while (model) {
        n += d * (model % 10 - 1);
        model /= 10;
        d *= 9;
    }
    return n;
}

int main(void)
{
    FILE *f = fopen("input24.txt", "r");
    int c;
    for (int ip = 0; ip < MEMSIZE && (c = fgetc(f)) != EOF; ++ip) {
        printf("%d:", ip);

        OpCode op = NOP;
        int arg0 = 0, arg1 = 0;
        bool imm = false;

        int d = fgetc(f);
        switch (c) {
            case 'a': op = ADD; break;
            case 'd': op = DIV; break;
            case 'e': op = EQL; break;
            case 'i': op = INP; break;
            case 'm': op = d == 'u' ? MUL : MOD; break;
        }
        fgetc(f);
        printf(" %s", lang[op]);

        if ((c = fgetc(f)) == ' ') {  // there's more?
            arg0 = fgetc(f) - 'w';
            printf(" %c", arg0 + 'w');

            if ((c = fgetc(f)) == ' ') {  // there's more?
                char buf[4] = {0};
                buf[0] = (char)fgetc(f);
                if (buf[0] >= 'w' && buf[0] <= 'z') {
                    arg1 = buf[0] - 'w';
                    printf(" %c", arg1 + 'w');
                    fgetc(f);  // \n
                } else if (buf[0] == '-' || (buf[0] >= '0' && buf[0] <= '9')) {
                    imm = true;
                    int i = 1;
                    while ((c = fgetc(f)) != '\n')
                        buf[i++] = (char)c;
                    buf[i] = '\0';
                    arg1 = atoi(buf);
                    printf(" %d", arg1);
                }
            }
        }
        alu.mem[ip] = (Instr){op, {arg0, arg1}, imm};
        printf("\n");
    }
    fclose(f);

    int64_t maxmodel = 0;
    for (int i = 0; i < STACKSIZE; ++i)
        maxmodel = maxmodel * 10 + 9;
    printf("%lld\n", maxmodel);
    int64_t val = base10(maxmodel);
    while (!run(&alu, val)) {
        printf("%lld %lld\n", val, model(val));
        --val;
    }
    printf("Part 1: %lld\n", model(val));
    return 0;
}
