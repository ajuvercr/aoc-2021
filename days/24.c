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
  while (parseInst(f, &out.instrs[out.inst_c])) {
    out.inst_c++;
  }

  return out;
}

void moveInpInstrs(Prog *p) {
  int li = -1;
  Inst lx;
  for (int i = 0; i < p->inst_c; i++) {
    Inst x = p->instrs[i];
    if (x.ty == Inp) {
      li = i;
      lx = x;
      continue;
    }
    if (li != -1 && (x.alpha == lx.alpha || x.beta.sym == lx.alpha)) {

      p->instrs[i - 1] = lx;
      li = -1;
    }
    if (li != -1) {
      p->instrs[i - 1] = x;
    }
  }
}

long ret(Prog *prog, Op op) {
  if (op.sym == L)
    return op.v;
  else
    return prog->regs[op.sym - 1];
}

long retS(Prog *prog, Sym sym) { return prog->regs[sym - 1]; }

void set(Prog *prog, Sym sym, long v) { prog->regs[sym - 1] = v; }

int apply(Prog *prog, Inst inst) {
  long a = retS(prog, inst.alpha);
  long b = ret(prog, inst.beta);
  switch (inst.ty) {
  case Inp:
    set(prog, inst.alpha, *prog->input);
    prog->input++;
    break;
  case Add:
    set(prog, inst.alpha, a + b);
    break;
  case Mul:
    set(prog, inst.alpha, a * b);
    break;
  case Div: {
    if (b == 0)
      return 0;
    set(prog, inst.alpha, retS(prog, inst.alpha) / b);
  } break;
  case Mod: {
    if (a < 0)
      return 0;
    if (b <= 0)
      return 0;
    set(prog, inst.alpha, a % b);
  } break;
  case Eql:
    set(prog, inst.alpha, a == b);
    break;
  }
  return 1;
}

// (Program authors should be especially cautious; attempting to execute div
// with b=0 or attempting to execute mod with a<0 or b<=0 will cause the program
// to crash and might even damage the ALU. These operations are never intended
// in any serious ALU program.)
int runProg(Prog *prog, int from) {
  for (int i = from; i < prog->inst_c; i++) {
    Inst inst = prog->instrs[i];
    if (inst.ty == Inp)
      return i;
    if (!apply(prog, inst))
      return -1;
  }
  return prog->inst_c;
}

#define OPTIONS_S 42400
typedef struct Options {
  long vs[OPTIONS_S][3];
  int sizes;
} Options;

void maybeAddOption(long *v, Options *o) {
  if (o->sizes == OPTIONS_S) {
    printf("NOPE\n");
    exit(1);
  }
  for (int j = 0; j < o->sizes; j++)
    if (v[1] == o->vs[j][0] && v[2] == o->vs[j][1] && v[3] == o->vs[j][2])
      return;

  for (int i = 1; i < 4; i++) {
    o->vs[o->sizes][i - 1] = v[i];
  }
  o->sizes += 1;
}

// typedef enum { L, W, X, Y, Z } Sym;
int doSegment(Prog *prog, int start, Options *oi, Options *oo) {
  int d = 0;
  for (int w = 1; w < 10; w++) {
    for (int i = 0; i < oi->sizes; i++) {
      long *regs = oi->vs[i];
      prog->regs[0] = w;
      prog->regs[1] = regs[0];
      prog->regs[2] = regs[1];
      prog->regs[3] = regs[2];
      d = runProg(prog, start + 1);
      if (d == -1)
        continue;
      for (int j = 0; j < 4; j++)
        maybeAddOption(prog->regs, oo);
    }
  }

  return d;
}

void print(long numbers[14]) {
  for (int i = 0; i < 14; i++)
    printf("%ld", numbers[i]);
  printf("\n");
}
void printOptions(Options *o) {
  int ss = o->sizes;
  printf("options %d\n", ss);
}

void part1(const char *inputLocation) {
  Prog p = parseProg(openFile(inputLocation));
  moveInpInstrs(&p);
  Options options[48];
  options[0].sizes = 1;
  for (int i = 0; i < 4; i++) {
    options[0].vs[0][i] = 0;
  }
  int s = 0;
  int i = 0;
  while (s != p.inst_c) {
    printf("s %d\n", s);
    printOptions(&options[i]);
    s = doSegment(&p, s, &options[i], &options[i + 1]);
    i++;
  }
  printOptions(&options[i]);
  printOptions(&options[7]);
  printf("part 1: ");
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
