#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int X(char x) { return x - 'a'; }
int Y(char x) {
  int i = 0;
  while (!(x & 1)) {
    x = x >> 1;
    i++;
  }
  return i;
}

typedef struct {
  char input[10][8];
  char output[4][8];
} Input;

List *getInput(const char *loc) {
  List *out = newList();
  FILE *f = openFile(loc);

  char buff[256];
  while (fscanf(f, "%s", buff) > 0) {
    Input *inp = malloc(sizeof(Input));

    stpcpy(inp->input[0], buff);
    for (int i = 1; i < 10; i++) {
      fscanf(f, "%s", inp->input[i]);
      inp->input[i][7] = '\0';
    }
    fscanf(f, "%s", buff);
    for (int i = 0; i < 4; i++) {
      fscanf(f, "%s", inp->output[i]);
      inp->output[i][7] = '\0';
    }

    listAdd(out, inp);
  }
  fclose(f);

  return out;
}

unsigned char LEDS[10] = {
    // aaa   bbbbbb   cccccc   dddddd   eeeeee   ffffff   gggggg
    1 << 0 | 1 << 1 | 1 << 2 | 0 << 3 | 1 << 4 | 1 << 5 | 1 << 6, // 0
    0 << 0 | 0 << 1 | 1 << 2 | 0 << 3 | 0 << 4 | 1 << 5 | 0 << 6, // 1
    1 << 0 | 0 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 0 << 5 | 1 << 6, // 2
    1 << 0 | 0 << 1 | 1 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 1 << 6, // 3
    0 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 0 << 6, // 4
    1 << 0 | 1 << 1 | 0 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 1 << 6, // 5
    1 << 0 | 1 << 1 | 0 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6, // 6
    1 << 0 | 0 << 1 | 1 << 2 | 0 << 3 | 0 << 4 | 1 << 5 | 0 << 6, // 7
    1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6, // 8
    1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 1 << 6, // 9
};

char tochar(char *thing, int l) {
  char out = 0;
  for (int i = 0; i < l; i++)
    out = out | 1 << X(thing[i]);
  return out;
}

char allIn(char a, char b) { return (a & b) == b; }

int toDigit(char digit[8], char mapping[8]) {
  unsigned char target = 0;
  for (int i = 0; i < 8; i++) {
    if (digit[i] == '\0')
      break;
    target |= 1 << X(mapping[X(digit[i])]);
  }

  for (int i = 0; i < 10; i++) {
    if (LEDS[i] == target)
      return i;
  }

  return -1;
}

int determineValue(char input[10][8], char output[4][8]) {
  char s2[1];
  char s3[1];
  char s4[1];
  char s7[1];
  int a5 = 0, a6 = 0;
  char s5[3];
  char s6[3];

  for (int i = 0; i < 10; i++) {
    switch (strlen(input[i])) {
    case 2:
      s2[0] = tochar(input[i], 2);
      break;
    case 3:
      s3[0] = tochar(input[i], 3);
      break;
    case 4:
      s4[0] = tochar(input[i], 4);
      break;
    case 7:
      s7[0] = tochar(input[i], 7);
      break;
    case 5:
      s5[a5++] = tochar(input[i], 5);
      break;
    case 6:
      s6[a6++] = tochar(input[i], 6);
      break;
    }
  }

  // input character output what it is irl
  char mapping[9] = {0};

  char ma, mb, mc, md, me, mf, mg;
  ma = s2[0] ^ s3[0];
  mapping[Y(ma)] = 'a';
  int c6 = allIn(s6[0], s2[0]) ? (allIn(s6[1], s2[0]) ? 2 : 1) : 0;

  mc = s7[0] ^ s6[c6];
  mapping[Y(mc)] = 'c';

  mf = s2[0] ^ mc;
  mapping[Y(mf)] = 'f';

  int c3 = allIn(s5[0], s2[0]) ? 0 : (allIn(s5[1], s2[0]) ? 1 : 2);
  char bore = (~(1 << 7)) ^ s5[c3];

  mb = s4[0] & bore;
  mapping[Y(mb)] = 'b';

  me = bore ^ mb;
  mapping[Y(me)] = 'e';

  md = (s4[0] ^ s2[0]) ^ mb;
  mapping[Y(md)] = 'd';

  mg = s7[0] ^ (s4[0] | s3[0] | bore);
  mapping[Y(mg)] = 'g';

  int out =  (1000 * toDigit(output[0], mapping) +
          100 * toDigit(output[1], mapping) + 10 * toDigit(output[2], mapping) +
          toDigit(output[3], mapping));

  return out;
}

void part1(const char *inputLocation) {
  List *inps = getInput(inputLocation);
  Input **l = (Input **)inps->list;
  int out = 0;

  for (int j = 0; j < inps->size; j++) {
    for (int i = 0; i < 4; i++) {
      int len = strlen(l[j]->output[i]);
      if (len == 2 || len == 7 || len == 4 || len == 3)
        out++;
    }
  }
  printf("part 1: %d\n", out);
  freeList(inps, free);
}

void part2(const char *inputLocation) {
  List *inps = getInput(inputLocation);
  Input **l = (Input **)inps->list;
  long output = 0;
  for (int i = 0; i < inps->size; i++) {
    int o = determineValue(l[i]->input, l[i]->output);
    output += o;
  }
  printf("part 2: %ld\n", output);
  freeList(inps, free);
}

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
