#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1(const char *inputLocation) {
  FILE *file = openFile(inputLocation);

  int x = 0;
  int z = 0;
  char buff[255] = {0};
  while (readWord(file, buff) > 0) {
    int amount = 0;
    fscanf(file, "%d", &amount);

    if (!strcmp(buff, "forward"))
      x += amount;
    if (!strcmp(buff, "down"))
      z += amount;
    if (!strcmp(buff, "up"))
      z -= amount;
  }

  printf("part 1: %d\n", x * z);
}

void part2(const char *inputLocation) {
  FILE *file = openFile(inputLocation);

  int x = 0;
  int z = 0;
  int aim = 0;
  char buff[255] = {0};
  while (readWord(file, buff) > 0) {
    int amount = 0;
    fscanf(file, "%d", &amount);

    if (!strcmp(buff, "up"))
      aim -= amount;
    if (!strcmp(buff, "down"))
      aim += amount;
    if (!strcmp(buff, "forward")) {
      x += amount;
      z += aim * amount;
    }
  }

  printf("part 2: %d\n", x * z);
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
