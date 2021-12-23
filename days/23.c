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
Coord POS[23] = {{1, 1},  {2, 1}, {4, 1}, {6, 1}, {8, 1}, {10, 1},
                 {11, 1}, {3, 2}, {5, 2}, {7, 2}, {9, 2}, {3, 3},
                 {5, 3},  {7, 3}, {9, 3}, {3, 4}, {5, 4}, {7, 4},
                 {9, 4},  {3, 5}, {5, 5}, {7, 5}, {9, 5}};

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

int correctPlace(int x, int y, int type) {
  if (y == 1)
    return 0;

  if (type == A)
    return x == 3;
  if (type == B)
    return x == 5;
  if (type == C)
    return x == 7;
  if (type == D)
    return x == 9;
  printf("NOPE %d\n", type);
  return 0;
}

int atHome(Loc amphi, Loc others[8]) {
  if (!correctPlace(amphi.x, amphi.y, amphi.type))
    return 0;
  if (amphi.y == 3)
    return 1;

  for (int i = 0; i < 8; i++) {
    Loc o = others[i];
    if (o.type != amphi.type)
      continue;
    if (o.x == amphi.x && o.y == 3)
      return 1;
  }
  return 0;
}

int validMove(Loc source, Coord target, Loc others[8]) {
  if (source.y == target.y || source.x == target.x)
    return 0;
  if (source.y != 1 && target.y != 1)
    return 0;

  for (int i = 0; i < 8; i++) {
    Loc inter = others[i];
    if (inter.x == target.x && inter.y == target.y)
      return 0;
  }

  if (source.y == 3) {
    for (int i = 0; i < 8; i++) {
      Loc inter = others[i];
      if (inter.x == source.x && inter.y == 2)
        return 0;
    }
  }

  for (int i = 0; i < 8; i++) {
    Loc inter = others[i];
    if (1 == inter.y && source.x < inter.x && target.x > inter.x)
      return 0;
    if (1 == inter.y && source.x > inter.x && target.x < inter.x)
      return 0;
  }
  if (target.y != 1) {
    if (!correctPlace(target.x, target.y, source.type))
      return 0;
    if (target.y == 3)
      return 1;

    for (int i = 0; i < 8; i++) {
      if (others[i].type == source.type && others[i].y == 3 &&
          others[i].x == target.x)
        return 1;
    }

    return 0;
  }

  return 1;
}

int cost(Coord source, Coord target, Amphi type) {
  int mult = type == A ? 1 : type == B ? 10 : type == C ? 100 : 1000;
  return mult * (abs(source.x - target.x) + abs(source.y - target.y));
}

int finished(Loc amphis[8]) {
  for (int i = 0; i < 8; i++) {
    Loc l = amphis[i];
    if (!correctPlace(l.x, l.y, l.type))
      return 0;
  }
  return 1;
}

int step(Loc amphis[8], Loc *depths[4][4], int current, int best) {
  if (finished(amphis)) {
    print(amphis);
    return current;
  }

  for (int i = 0; i < 8; i++) {
    Coord orig = {amphis[i].x, amphis[i].y};
    if (atHome(amphis[i], amphis))
      continue;

    if (amphis[i].y == 1) {
      // amphis is at top row, let's try go to correct shaft
      Amphi ty = amphis[i].type;
      Loc **shaft = depths[ty];
      int i = 0;
      while (shaft[i]) {
        if (shaft[i]->type != ty)
          break;
        i++;
      }
      if (shaft[i])
        continue;

      Coord target = {ty * 2 + 3, i + 2};
      int ncurrent = current + cost(orig, target, amphis[i].type);
      if (ncurrent >= best)
        continue;

      amphis[i].x = target.x;
      amphis[i].y = target.y;
      shaft[i] = &amphis[i];

      print(amphis);

      int c = step(amphis, depths, ncurrent, best);
      if (c < best) {
        printf("Best %d -> %d\n", best, c);
        best = c;
        amphis[i].x = orig.x;
        amphis[i].y = orig.y;
        print(amphis);
      }

      amphis[i].x = orig.x;
      amphis[i].y = orig.y;
      depths[ty][i] = 0;
    } else {
      for (int t = 0; t < 7; t++) {
        if (validMove(amphis[i], POS[t], amphis)) {
          int ncurrent = current + cost(orig, POS[t], amphis[i].type);
          if (ncurrent >= best)
            continue;
          depths[(amphis[i].x - 3) / 2][amphis[i].y - 2] = 0;
          amphis[i].x = POS[t].x;
          amphis[i].y = POS[t].y;
          print(amphis);
          int c = step(amphis, depths, ncurrent, best);
          if (c < best) {
            best = c;
            printf("best %d cost %d \n", best,
                   cost(orig, POS[t], amphis[i].type));
            amphis[i].x = orig.x;
            amphis[i].y = orig.y;
            print(amphis);
          }
          amphis[i].x = orig.x;
          amphis[i].y = orig.y;
          depths[(amphis[i].x - 3) / 2][amphis[i].y - 2] = &amphis[i];
        }
      }
    }
  }

  return best;
}

void part1(const char *inputLocation) {
  Loc poses[8];
  parseInput(inputLocation, poses);
  for (int i = 0; i < 8; i++)
    printf("(%d,%d) %d\n", poses[i].x, poses[i].y, poses[i].type);
  Loc *depths[4][4] = {{0}};
  for (int i = 0; i < 8; i++) {
    depths[(poses[i].x - 3) / 2][poses[i].y - 2] = &poses[i];
  }
  print(poses);
  printf("part 1: %d", step(poses, depths, 0, 1000000));
}

void part2(const char *inputLocation) {
  Coord source = {1, 1};
  Coord target = {3, 3};
  printf("part 2: %d\n", cost(source, target, D));
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
