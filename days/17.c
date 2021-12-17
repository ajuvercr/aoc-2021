#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int X[2] = {0};
int Y[2] = {0};

void parseInput(const char *loc) {
  FILE *f = openFile(loc);
  char buff[256];
  int x1, x2, y1, y2;
  fscanf(f, "%s %s %c%c%d%c%c%d%c%c%c%c%d%c%c%d", buff, buff, buff, buff, &x1,
         buff, buff, &x2, buff, buff, buff, buff, &y1, buff, buff, &y2);
  X[0] = x1;
  X[1] = x2;
  Y[0] = y1;
  Y[1] = y2;
  fclose(f);
}

int calcHeight(int dy) {
  int out = 0;
  while (dy)
    out += dy--;
  return out;
}

int hits(int dx, int dy) {
  int ox, oy;
  int x = 0;
  int y = 0;
  while (y >= Y[0] && x <= X[1]) {
    ox = x;
    oy = y;
    x += dx;
    y += dy;
    if (dx > 0)
      dx--;
    if (dx < 0)
      dx++;
    dy--;
  }
  return ox >= X[0] && oy <= Y[1];
}

void part1(const char *inputLocation) {
  parseInput(inputLocation);
  printf("part 1: %d\n", calcHeight(-1 * Y[0] - 1));
}

void part2(const char *inputLocation) {
  parseInput(inputLocation);
  int out = 0;
  for (int i = 0; i <= X[1]; i++)
    for (int j = Y[0]; j < -1 * Y[0]; j++)
      out += hits(i, j);
  printf("part 2: %d\n", out);
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
