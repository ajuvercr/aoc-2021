#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

typedef struct {
  int inner[SIZE][SIZE];
} Input;

Input parseInput(const char *loc) {
  FILE *f = openFile(loc);
  Input out;

  char buff[256] = {0};
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      fscanf(f, "%c", buff);
      out.inner[i][j] = atoi(buff);
    }
    fscanf(f, "%c", buff);
  }

  return out;
}

List *findLowPoints(Input *inp) {
  List *out = newList();

  int mi, Mi, mj, Mj;
  for (long i = 0; i < SIZE; i++) {
    for (long j = 0; j < SIZE; j++) {
      int v = inp->inner[i][j];

      mi = i - 1;
      mj = j - 1;
      Mi = i + 1;
      Mj = j + 1;

      if ((mi < 0 || inp->inner[mi][j] > v) &&
          (Mi >= SIZE || inp->inner[Mi][j] > v) &&
          (mj < 0 || inp->inner[i][mj] > v) &&
          (Mj >= SIZE || inp->inner[i][Mj] > v)) {
        listAdd(out, (void *)i);
        listAdd(out, (void *)j);
      }
    }
  }

  return out;
}

void part1(const char *inputLocation) {
  Input inp = parseInput(inputLocation);
  int out = 0;
  List *minPoints = findLowPoints(&inp);
  for (int i = 0; i < minPoints->size; i += 2) {
    out +=
        1 +
        inp.inner[(long)listGet(minPoints, i)][(long)listGet(minPoints, i + 1)];
  }

  printf("part 1: %d\n", out);
}

int getMaxIndex(int *counts, int at) {
  int maxI = 0;
  for (int i = 1; i < at; i++)
    if (counts[i] > counts[maxI])
      maxI = i;
  return maxI;
}

void part2(const char *inputLocation) {

  Input inp = parseInput(inputLocation);
  List *minPoints = findLowPoints(&inp);
  int at = -1;
  for (int i = 0; i < minPoints->size; i += 2) {
    inp.inner[(long)listGet(minPoints, i)][(long)listGet(minPoints, i + 1)] =
        at--;
  }

  int changed = 1;
  while (changed) {
    changed = 0;

    int mi, Mi, mj, Mj;
    for (long i = 0; i < SIZE; i++) {
      for (long j = 0; j < SIZE; j++) {
        int v = inp.inner[i][j];

        if (v < 0 || v == 9)
          continue;
        mi = i - 1;
        mj = j - 1;
        Mi = i + 1;
        Mj = j + 1;

        int basin = 0;
        if (mi >= 0 && inp.inner[mi][j] < 0)
          basin = inp.inner[mi][j];
        if (Mi < SIZE && inp.inner[Mi][j] < 0)
          basin = inp.inner[Mi][j];
        if (mj >= 0 && inp.inner[i][mj] < 0)
          basin = inp.inner[i][mj];
        if (Mj < SIZE && inp.inner[i][Mj] < 0)
          basin = inp.inner[i][Mj];

        if (basin) {
          inp.inner[i][j] = basin;
          changed = 1;
        }
      }
    }
  }

  at = -1 * at;
  int counts[at];
  for (int i = 0; i < at; i++)
    counts[i] = 0;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      int v = inp.inner[i][j];
      if (v < 0)
        counts[-1 * v]++;
    }
  }

  int maxI = getMaxIndex(counts, at);
  int v1 = counts[maxI];
  counts[maxI] = 0;

  maxI = getMaxIndex(counts, at);
  int v2 = counts[maxI];
  counts[maxI] = 0;

  maxI = getMaxIndex(counts, at);
  int v3 = counts[maxI];

  printf("part 2: %d\n", v1 * v2 * v3);
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
