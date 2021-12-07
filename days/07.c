#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000

void readInput(const char *loc, int *locs) {
  FILE *f = openFile(loc);
  char c;
  int i = 0;
  while (fscanf(f, "%d%c", &locs[i], &c) > 0)
    i++;
}

int part = 1;

int bar(int v) {
  if (v < 0)
    v = -1 * v;
  if (part == 1)
    return v;
    return v * (v+1) / 2;
}

int requiredFuel(const int *values, int loc) {
  int total = 0;

  for (int i = 0; i < SIZE; i++)
    total += bar(loc - values[i]);
  return total;
}

int tryAt(const int *values, int loc) {
  int v1 = 0, v2 = 0, v3 = 0;
  for (int i = 0; i < SIZE; i++) {
    v1 += bar(loc - 1 - values[i]);
    v2 += bar(loc + 0 - values[i]);
    v3 += bar(loc + 1 - values[i]);
  }
  if (v2 <= v1 && v2 <= v3)
    return 0;
  if (v1 > v2)
    return 1;
  return -1;
}

typedef int tryF(const void *, int);
int foo(const void *inp, tryF f) {
  int min = 0;
  int max = SIZE;
  int at = SIZE / 2;

  int t = f(inp, at);

  while (t != 0) {
    if (t < 0) {
      max = at;
    } else {
      min = at + 1;
    }
    at = min + (max - min) / 2;
    t = f(inp, at);
  }

  return at;
}

int calculateMean(const int *values) {
  int total = 0;
  for (int i = 0; i < SIZE; i++)
    total += values[i];

  int out = total / SIZE;
  float t1 = total / (float)SIZE;

  if (t1 - (float)out >= 0.5)
    return out + 1;
  return out;
}

void part1(const char *inputLocation) {
  int locations[SIZE] = {0};
  readInput(inputLocation, locations);
  int at = foo(locations, (tryF *)tryAt);
  int fuel = requiredFuel(locations, at);
  printf("part 1: %d\n", fuel);
}

void part2(const char *inputLocation) {
  part = 2;
  int locations[SIZE] = {0};
  readInput(inputLocation, locations);
  int at = foo(locations, (tryF *)tryAt);
  int fuel = requiredFuel(locations, at);
  printf("part 2: %d\n", fuel);
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
