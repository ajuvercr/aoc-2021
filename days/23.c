#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum { A, B, C, D, E } Amphi;

int shaftSize = 2;
Amphi row[7] = {E};

void parseInput(const char *inputLocation, Amphi shafts[4][5]) {
  char input[5][14] = {{0}};
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
  for (int i = 0; i < 4; i++) {
    shafts[i][0] = 2;
    shafts[i][1] = input[2][i * 2 + 3] - 'A';
    shafts[i][2] = input[3][i * 2 + 3] - 'A';
  }
}

void print(Amphi row[7], Amphi shafts[4][5]) {
  printf("############\n");
  printf("#%c%cE%cE%cE%cE%c%c#\n", row[0], row[1], row[2], row[3], row[4],
         row[5], row[6]);
  for (int i = 1; i <= shaftSize; i++) {
    printf(" #%c#%c#%c#%c#\n", shafts[i][0], shafts[i][1], shafts[i][2],
           shafts[i][3]);
  }
  printf(" #########\n");
}

int shaftToRow(int s) { return 2 + 2 * s; }

int shaftReady(int a, Amphi shafts[4][5]) {
  for (int i = 1; i <= shafts[a][0]; i++) {
    if (shafts[a][i] != i && shafts[a][i] != E)
      return 0;
  }
  return 1;
}

int finised(Amphi shafts[4][5]) {
  for (int i = 0; i < 4; i++) {
    if (shafts[i][0] != shaftSize)
      return 0;
    if (!shaftReady(i, shafts))
      return 0;
  }
  return 1;
}

long cost(Amphi x) {
  switch (x) {
  case A:
    return 1;
  case B:
    return 10;
  case C:
    return 100;
  case D:
    return 1000;
  default:
    return 0;
  }
}

long step(Amphi row[7], Amphi shafts[4][5], long best) {
  if (finised(shafts))
    return best;

  for (int i = 0; i < 7; i++) {
    Amphi x = row[i];
    if (x == E)
      continue;
    if (!shaftReady(x, shafts))
      continue;

    int cont = 1;
    int t = shaftToRow(x);
    for (int j = t; j < i; j++)
      cont = cont && row[j] == E;

    for (int j = i + 1; j <= t; j++)
      cont = cont && row[j] == E;
    if (!cont)
      continue;

    // Found valid move!! Let's move

    // what distance?
    long dd = labs(i - t) + shaftSize - (long)shafts[x][0];

    row[i] = E;
    shafts[x][shafts[x][0]] = x;
    shafts[x][0]++;

    long try = step(row, shafts, best + dd * cost(x));
    if (try != -1 && try < best)
      best = try;

    row[i] = x;
    shafts[x][0] --;
    shafts[x][shafts[x][0]] = E;
  }

  for (int i = 0; i < 4; i++) {
    if (shaftReady(i, shafts))
      continue;

    int ri = shaftToRow(i);
    Amphi x = shafts[i][shafts[i][0]];
    for (int j = ri - 1; j >= -1; j -= 2) {
      if (j == -1)
        j = 0;

      if (row[j] != E)
        continue;
      long dd = labs(i - shaftToRow(i)) + shaftSize - (long)shafts[x][0];

      shafts[i][0]--;
      shafts[i][shafts[i][0]] = E;
      row[j] = x;

      long try = step(row, shafts, best + dd * cost(x));
      if (try != -1 && try < best)
        best = try;

      row[j] = E;
      shafts[i][shafts[i][0]] = x;
      shafts[i][0]++;
    }
  }

  return -1;
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
