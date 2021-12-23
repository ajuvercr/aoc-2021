#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum { A, B, C, D } Amphi;

typedef struct Coord {
  int x;
  int y;
} Coord;

typedef struct Loc {
  int x;
  int y;
  Amphi type;
} Loc;
Coord POS[15] = {{1, 1},  {2, 1},  {4, 1}, {6, 1}, {8, 1},
                 {10, 1}, {11, 1}, {3, 2}, {5, 2}, {7, 2},
                 {9, 2},  {3, 3},  {5, 3}, {7, 3}, {9, 3}};

// 15416 too high
char input[5][14] = {{0}};

void parseInput(const char *inputLocation, Loc poses[8]) {
  FILE *f = openFile(inputLocation);
  fread(input[0], 1, 14, f);
  input[0][13] = '\0';
  fread(input[1], 1, 14, f);
  input[1][13] = '\0';
  fread(input[2], 1, 14, f);
  input[2][13] = '\0';
  fread(input[3], 1, 12, f);
  input[3][11] = '\0';
  fread(input[4], 1, 12, f);
  input[4][11] = '\0';
  fclose(f);

  int j = 0;
  for (int i = 0; i < 15; i++) {
    Coord test = POS[i];
    char c = input[test.y][test.x];
    Loc *l = &poses[j];
    int good = 0;
    if (c == 'A') {
      good = 1;
      l->type = A;
    }
    if (c == 'B') {
      good = 1;
      l->type = B;
    }
    if (c == 'C') {
      good = 1;
      l->type = C;
    }

    if (c == 'D') {
      good = 1;
      l->type = D;
    }

    if (good) {
      l->x = test.x;
      l->y = test.y;
      j++;
    }
  }
}

void print(Loc poses[8]) {
  for (int i = 0; i < 8; i++)
    input[poses[i].y][poses[i].x] = 'A' + poses[i].type;
  for (int i = 0; i < 5; i++) {
    printf("%s\n", input[i]);
  }
  for (int i = 0; i < 8; i++)
    input[poses[i].y][poses[i].x] = '.';
}

int correctPlace(int x, int y, Amphi type) {
  if (y == 1)
    return 0;
  switch (type) {
  case A:
    return x == 3;
  case B:
    return x == 5;
  case C:
    return x == 7;
  case D:
    return x == 9;
  }
  return 0;
}

int validMove(Loc source, Coord target, Loc others[8]) {
  if (source.y == target.y || source.x == target.x)
    return 0;

  if(target.y != 1 && !correctPlace(target.x, target.y, source.type)) return 0;

  if (correctPlace(source.x, source.y, source.type)) {
    if (source.y == 3)
      return 0;

    for (int i = 0; i < 8; i++) {
      if (others[i].type == source.type && others[i].y == 3 &&
          others[i].x == source.x)
        return 0;
    }
  }

  for (int i = 0; i < 8; i++) {
    Loc inter = others[i];
    if (inter.x == target.x && inter.y == target.y)
      return 0;
    if (source.x < inter.x && target.x > inter.x)
      return 0;
    if (source.x > inter.x && target.x < inter.x)
      return 0;
  }
  return 1;
}

int cost(Loc source, Coord target) {
  int mult = source.type == A   ? 1
             : source.type == B ? 10
             : source.type == C ? 100
                                : 1000;
  return mult * ((source.x - target.x) + abs(source.y - target.y));
}

int finished(Loc amphis[8]) {
  for (int i = 0; i < 8; i++) {
    Loc l = amphis[8];
    if (!correctPlace(l.x, l.y, l.type))
      return 0;
  }
  return 1;
}

int step(Loc amphis[8]) {
  if (finished(amphis)) {
    printf("IS FINSIHED\n");
    return 0;
  }

  int out = -1;
  for (int i = 0; i < 8; i++) {
    Coord orig = {amphis[i].x, amphis[i].y};
    for (int t = 0; t < 15; t++) {
      // printf("move %d (%d,%d)\n", t, POS[t].x, POS[t].y);
      if (validMove(amphis[i], POS[t], amphis)) {
        printf("move %d (%d,%d)\n", t, POS[t].x, POS[t].y);
        amphis[i].x = POS[t].x;
        amphis[i].y = POS[t].y;
        print(amphis);
        int c = step(amphis);
        if (c >= 0) {
          printf("GOT STEP %d\n", c);
          int nout = c + cost(amphis[i], orig);
          if (out == -1 || nout < out) {
            out = nout;
          }
        }
        amphis[i].x = orig.x;
        amphis[i].y = orig.y;
      }
    }
  }

  return out;
}

void part1(const char *inputLocation) {
  Loc poses[8];
  parseInput(inputLocation, poses);
  for (int i = 0; i < 8; i++)
    printf("(%d,%d) %c\n", poses[i].x, poses[i].y, 'A' + poses[i].type);
  print(poses);
  printf("part 1: %d", step(poses));
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
