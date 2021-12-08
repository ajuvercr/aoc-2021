#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X(x) ((x) - 'a')
#define Y(x) (__builtin_ctz(x))

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

  return out;
}

char LEDS[10] = {
    //  aaaaaa   bbbbbb   cccccc   dddddd   eeeeee   ffffff   gggggg
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

char *binString(unsigned char n) {
  static char bin[1024];
  static int at = 0;
  int x;

  for (x = 0; x < 8; x++) {
    bin[x + at] = n & 0x1 ? '1' : '0';
    n >>= 1;
  }
  bin[at + 8] = '\0';

  char *out = bin + at;
  at += 9;
  return out;
}
char allIn(char a, char b) {
  printf("%s has all %s == %d\n", binString(a), binString(b), (a & b) == b);
  return (a & b) == b;
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

  printf("2 %s 3 %s 4 %s 7 %s 5 %s %s %s 6 %s %s %s\n", binString(s2[0]),
         binString(s3[0]), binString(s4[0]), binString(s7[0]), binString(s5[0]),
         binString(s5[1]), binString(s5[2]), binString(s6[0]), binString(s6[1]),
         binString(s6[2]));

  printf("first 1: %d\n", Y(4));

  // input character output what it is irl
  char mapping[9] = {0};

  char ma, mb, mc, md, me, mf, mg;
  ma = s2[0] ^ s3[0];
  mapping[Y(ma)] = 'a';
  int c6 = allIn(s6[0], s2[0]) ? (allIn(s6[1], s2[0]) ? 2 : 1) : 0;

  printf("step 3: %d %s\n", c6, binString(s7[0] ^ s6[c6]));
  mc = s7[0] ^ s6[c6];
  mapping[Y(mc)] = 'c';

  mf = s2[0] ^ mc;
  mapping[Y(mf)] = 'f';

  int c3 = allIn(s5[0], s2[0]) ? 0 : (allIn(s5[1], s2[0]) ? 1 : 2); 
  char bore = (~(1 << 7)) ^ s5[c3];


  printf("mapping '%s'\n", mapping);

  return 0;
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
}

void part2(const char *inputLocation) {
  List *inps = getInput(inputLocation);
  Input **l = (Input **)inps->list;
  determineValue(l[0]->input, l[0]->output);
  printf("part 2: ");
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
