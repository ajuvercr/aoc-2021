#include "../lib/files.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int x, int y) { return x < y ? x : y; }
int max(int x, int y) { return x > y ? x : y; }

int cmp(int x[2], int y[2]) { return x[0] == y[0] && x[1] == y[1]; }
void print(int points[10][10]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("%d ", points[j][i]);
    }
    printf("\n");
  }
}
int signum(int v) {
    if(v < 0) return -1;
    if(v>0) return 1;
    return 0;
}

#define L 1000

int calculateOutput(const char *inputLocation, int doDiagonal) {
  int points[L][L] = {0};
  FILE *f = openFile(inputLocation);
  int x1, y1, x2, y2;
  char buf[256];
  while (fscanf(f, "%d%c", &x1, &buf[0]) > 0) {
    fscanf(f, "%d", &y1);
    fscanf(f, "%s", buf);

    fscanf(f, "%d%c", &x2, &buf[0]);
    fscanf(f, "%d", &y2);

    if (x1 == x2 || y1 == y2 || doDiagonal) {
      int dx = signum(x1-x2);
      int dy = signum(y1-y2);
      int x = x2;
      int y = y2;

      int mv = max(max(x1, x2) - min(x1, x2), max(y1, y2) - min(y1, y2));

      for (int i = 0; i <= mv; i++) {
        points[y][x]++;
        x += dx;
        y += dy;
      }
    }
  }


  int count = 0;

  for (int i = 0; i < L; i++) {
    for (int j = 0; j < L; j++) {
      if (points[i][j] > 1)
        count++;
    }
  }
  return count;
}

void part1(const char *inputLocation) {
  printf("part 1: %d\n", calculateOutput(inputLocation, 0));
}

void part2(const char *inputLocation) {
  printf("part 2: %d\n", calculateOutput(inputLocation, 1));
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
