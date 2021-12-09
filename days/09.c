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

void part2(const char *inputLocation) { 

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
