#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { Inp, Add, Mul, Div, Mod, Eql } Ty;

typedef enum { L, W, X, Y, Z } Sym;

typedef struct {
  Sym sym;
  long v;
} Op;

typedef struct {
  Ty ty;
  Sym alpha;
  Op beta;
} Inst;

typedef struct {
  Inst instrs[255];
  int inst_c;
  long regs[4];
  long *input;
  int inp[14];
} Prog;

int parseInst(FILE *f, Inst *inst) {
  char buff[24] = {'\0'};
  int v = fread(buff, 1, 5, f);
  if (!v)
    return 0;
  buff[3] = '\0';

  Ty ty = !strcmp(buff, "inp")   ? Inp
          : !strcmp(buff, "add") ? Add
          : !strcmp(buff, "mul") ? Mul
          : !strcmp(buff, "div") ? Div
          : !strcmp(buff, "mod") ? Mod
                                 : Eql;
  inst->ty = ty;
  inst->alpha = buff[4] - 'w' + 1;
  Op beta = {L, 0};
  inst->beta = beta;

  getc(f);

  if (ty == Inp)
    return 1;

  char buff2[24] = {'\0'};
  fscanf(f, "%s", buff2);
  if (buff2[0] >= 'a' && buff2[0] <= 'z') {
    beta.sym = buff2[0] - 'w' + 1;
    beta.v = 0;
  } else {
    beta.sym = L;
    beta.v = atol(buff2);
  }
  inst->beta = beta;
  getc(f);
  return 1;
}

Prog parseProg(FILE *f) {
  Prog out;
  out.inst_c = 0;
  int *inps = out.inp;
  while (parseInst(f, &out.instrs[out.inst_c])) {
    if (out.instrs[out.inst_c].ty == Inp) {
      *inps = out.inst_c;
      inps++;
    }

    out.inst_c++;
  }

  return out;
}

long ret(Prog *prog, Op op) {
  if (op.sym == L)
    return op.v;
  else
    return prog->regs[op.sym - 1];
}

long retS(Prog *prog, Sym sym) { return prog->regs[sym - 1]; }

void set(Prog *prog, Sym sym, long v) { prog->regs[sym - 1] = v; }

void apply(Prog *prog, Inst inst) {

  switch (inst.ty) {
  case Inp:
    set(prog, inst.alpha, *prog->input);
    prog->input++;
    break;
  case Add:
    set(prog, inst.alpha, retS(prog, inst.alpha) + ret(prog, inst.beta));
    break;
  case Mul:
    set(prog, inst.alpha, retS(prog, inst.alpha) * ret(prog, inst.beta));
    break;
  case Div:
    set(prog, inst.alpha, retS(prog, inst.alpha) / ret(prog, inst.beta));
    break;
  case Mod:
    set(prog, inst.alpha, retS(prog, inst.alpha) % ret(prog, inst.beta));
    break;
  case Eql:
    set(prog, inst.alpha, retS(prog, inst.alpha) == ret(prog, inst.beta));
    break;
  }
}

void runProg(Prog *prog, int from) {
  printf("starting at %d\n", from);
  for (int i = from; i < prog->inst_c; i++) {
    Inst inst = prog->instrs[i];
    if (inst.ty == Inp)
      return;
    apply(prog, inst);
  }
}

int tryDigit(Prog *prog, int currentDigit, long digits[14]) {
  if (currentDigit == 14)
    return 1;
  printf("current digit %d start at %d\n", currentDigit,
         prog->inp[currentDigit]);
  long regs[4];
  for (int i = 0; i < 4; i++)
    regs[i] = prog->regs[i];

  for (long i = 9; i > 0; i--) {
    for (int i = 0; i < 4; i++)
      prog->regs[i] = regs[i];

    prog->input = &i;
    apply(prog, prog->instrs[prog->inp[currentDigit]]);
    runProg(prog, prog->inp[currentDigit] + 1);

    printf("reg[3] = %ld\n", prog->regs[3]);
    if (prog->regs[3] == 0) {
      printf("HERE\n");
      if (tryDigit(prog, currentDigit + 1, digits)) {
        digits[currentDigit] = i;
        return 1;
      }
    }
  }
  return 0;
}

int tryProg(Prog *prog, long *num) {
  for (int i = 0; i < 4; i++)
    prog->regs[i] = 0;
  prog->input = num;
  runProg(prog, 0);
  return prog->regs[3] == 0;
}

void dec(long numbers[14]) {
  for (int i = 13; i >= 0; i--) {
    numbers[i]--;
    if (numbers[i] == 0)
      numbers[i] = 9;
    else
      return;
  }
}

void print(long numbers[14]) {
  for (int i = 0; i < 14; i++)
    printf("%ld", numbers[i]);
  printf("\n");
}

void part1(const char *inputLocation) {
  Prog p = parseProg(openFile(inputLocation));
  long numbers[14] = {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
  tryDigit(&p, 0, numbers);
  printf("part 1: ");
  print(numbers);
}

void part2(const char *inputLocation) { printf("part 2: "); }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify input file: %s <inputfile> [part]\n", argv[0]);
    exit(1);
  }

  char *inputfile = argv[1];
  int part = argc > 2 ? atoi(argv[2]) : 0;

  if (part == 0 || part == 1) {
    part1(inputfile);
  }

  if (part == 0 || part == 2) {
    part2(inputfile);
  }
  return 0;
}
