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
  printf("part 1: ");
  List *ints = getInput(inputLocation);
  int count = 0;
  for (int i = 1; i < ints->size; i++) {
    long x = (long) listGet(ints, i-1);
    long y = (long) listGet(ints, i);
    if(x < y) count ++;
  }
  printf("%d\n", count);
}

void part2(const char *inputLocation) {
  printf("part 2: ");
  List *ints = getInput(inputLocation);
  int count = 0;
  long preWindow = 1000000000000000;
  for (int i = 0; i < ints->size-2; i++) {
    long x = (long) listGet(ints, i);
    long y = (long) listGet(ints, i+1);
    long z = (long) listGet(ints, i+2);
    if(preWindow < x + y + z) count ++;
    preWindow = x + y + z;
  }
  printf("%d\n", count);
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
