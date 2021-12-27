#include "../lib/files.h"
#include "../lib/list.h"
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef int Amphi;
#define A 0
#define B 1
#define C 2
#define D 3
#define E -19

int shaftSize = 2;

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

  for (int i = 0; i < 4; i++) {
    shafts[i][4] = 2;
    shafts[i][1] = input[2][i * 2 + 3] - 'A';
    shafts[i][0] = input[3][i * 2 + 3] - 'A';
  }
}

void print(Amphi row[7], Amphi shafts[4][5]) {
  printf("#############\n");
  printf("#%c%c%c%c%c%c%c%c%c%c%c#\n", row[0] + 'A', row[1] + 'A', row[2] + 'A',
         row[3] + 'A', row[4] + 'A', row[5] + 'A', row[6] + 'A', row[7] + 'A',
         row[8] + 'A', row[9] + 'A', row[10] + 'A');
  for (int i = shaftSize - 1; i >= 0; i--) {
    printf(" ##%c#%c#%c#%c##\n", shafts[0][i] + 'A', shafts[1][i] + 'A',
           shafts[2][i] + 'A', shafts[3][i] + 'A');
  }
  printf(" ###########\n");
}

int shaftToRow(int s) { return 2 + 2 * s; }

int shaftReady(int a, Amphi shafts[4][5]) {
  for (int i = 0; i < shafts[a][4]; i++) {
    if (shafts[a][i] == a)
      continue;
    if (shafts[a][i] == E)
      continue;
    return 0;
  }
  return 1;
}

int finised(Amphi shafts[4][5]) {
  for (int i = 0; i < 4; i++) {
    if (shafts[i][4] != shaftSize) {
      return 0;
    }
    if (!shaftReady(i, shafts)) {
      return 0;
    }
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

int roi[7] = {0, 1, 3, 5, 7, 9, 10};
long step(Amphi row[11], Amphi shafts[4][5], long current, long best) {
  if (finised(shafts)) {
    return current;
  }

  for (int q = 0; q < 7; q++) {
    int i = roi[q];

    Amphi x = row[i];

    if (x == E)
      continue;
    if (!shaftReady(x, shafts)) {
      continue;
    }

    int cont = 1;
    int t = shaftToRow(x);
    for (int j = t; j < i; j++)
      cont = cont && row[j] == E;

    for (int j = i + 1; j < t; j++)
      cont = cont && row[j] == E;
    if (!cont) {
      continue;
    }

    // Found valid move!! Let's move

    // what distance?
    long dd = labs(i - t) + shaftSize - (long)shafts[x][4];
    long nb = current + dd * cost(x);
    if (nb > best)
      continue;

    shafts[x][shafts[x][4]] = x;
    shafts[x][4]++;
    row[i] = E;

    long try = step(row, shafts, nb, best);
    if (try < best) {
      best = try;
    }

    row[i] = x;
    shafts[x][4]--;
    shafts[x][shafts[x][4]] = E;
  }

  for (int i = 0; i < 4; i++) {
    if (shaftReady(i, shafts))
      continue;

    int ri = shaftToRow(i); // even
    Amphi x = shafts[i][shafts[i][4] - 1];

    for (int j = ri - 1; j >= -1; j -= 2) {
      if (j == -1)
        j = 0;
      if (row[j] != E)
        break;

      long dd = ri - j + shaftSize - (long)shafts[i][4] + 1;
      long nb = current + dd * cost(x);

      if (nb > best) {
        continue;
      }

      shafts[i][4]--;
      shafts[i][shafts[i][4]] = E;
      row[j] = x;

      long try = step(row, shafts, nb, best);
      if (try < best) {
        best = try;
      }

      row[j] = E;
      shafts[i][shafts[i][4]] = x;
      shafts[i][4]++;
    }

    for (int j = ri + 1; j < 12; j += 2) {
      if (j == 11)
        j = 10;
      if (row[j] != E)
        break;
      long dd = j - ri + shaftSize - (long)shafts[i][4] + 1;

      long nb = current + dd * cost(x);
      if (nb > best)
        continue;

      shafts[i][4]--;
      shafts[i][shafts[i][4]] = E;
      row[j] = x;

      long try = step(row, shafts, nb, best);
      if (try < best) {
        best = try;
      }

      row[j] = E;
      shafts[i][shafts[i][4]] = x;
      shafts[i][4]++;
    }
  }

  return best;
}

void part1(const char *inputLocation) {
  Amphi shafts[4][5];
  Amphi row[11] = {E};
  for (int i = 0; i < 11; i++)
    row[i] = E;
  parseInput(inputLocation, shafts);
  print(row, shafts);
  long out = step(row, shafts, 0, 100000000);
  printf("part 1: %ld\n", out);
}

void part2(const char *inputLocation) {
  Amphi shafts[4][5];
  Amphi row[11] = {E};
  for (int i = 0; i < 11; i++)
    row[i] = E;
  parseInput(inputLocation, shafts);
  for(int i = 0; i < 4; i ++) {
      shafts[i][4] = 4;
      shafts[i][3] = shafts[i][1];
  }
  shaftSize = 4;
  shafts[0][1] = D;
  shafts[0][2] = D;
  shafts[1][1] = B;
  shafts[1][2] = C;
  shafts[2][1] = A;
  shafts[2][2] = B;
  shafts[3][1] = C;
  shafts[3][2] = A;
  long out = step(row, shafts, 0, 100000000);
  printf("part 2: %ld\n", out);
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
