#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>

List *getInput(const char *inputLocation) {
  FILE *file = readFile(inputLocation);
  List *out = readInts(file);
  fclose(file);
  return out;
}

void part1(const char *inputLocation) {
  List *ints = getInput(inputLocation);
  int count = 0;
  long *longs = (long *)ints->list;

  for (int i = 0; i < ints->size - 1; i++)
    if (longs[i] < longs[i + 1])
      count++;

  printf("part 1: %d\n", count);
}

void part2(const char *inputLocation) {
  List *ints = getInput(inputLocation);
  int count = 0;
  long *longs = (long *)ints->list;

  for (int i = 0; i < ints->size - 3; i++)
    if (longs[i] < longs[i + 3])
      count++;

  printf("part 2: %d\n", count);
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
