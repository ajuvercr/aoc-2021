#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S 10000

char board[150][150];
int width;
int height;

typedef struct {
  int x;
  int y;
} Pair;

Pair parseInput(const char *loc, Pair lefts[S], Pair downs[S]) {
  FILE *f = openFile(loc);
  int i = 0;
  while (fscanf(f, "%s", board[i]) > 0) {
    if (!i) {
      width = strlen(board[0]);
    }

    board[i][width] = '\0';
    i++;
  }
  height = i;

  int lc = 0;
  int dc = 0;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      char c = board[j][i];
      if (c == '>') {
        lefts[lc].x = i;
        lefts[lc].y = j;
        lc++;
      }
      if (c == 'v') {
        downs[dc].x = i;
        downs[dc].y = j;
        dc++;
      }
    }
  }

  return (Pair){lc, dc};
}

Pair apply(Pair x, Pair y) {
  return (Pair) { (x.x + y.x) % width, (x.y + y.y) % height};
}

int semiStep(Pair ps[S], int s, Pair delta) {
    int idxc = 0;
    int idx[S];

    for(int i = 0; i < s; i++) {
        Pair o = apply(ps[i], delta);
        if(board[o.y][o.x] == '.')
            idx[idxc++] = i;
    }

    for(int i = 0; i < idxc; i++) {
      int j = idx[i];
      Pair s = ps[j];
      Pair o = apply(s, delta);
      char c = board[s.y][s.x];
      board[s.y][s.x] = '.';
      board[o.y][o.x] = c;
      ps[j].x = o.x;
      ps[j].y = o.y;
    }

    return idxc > 0;
}

int step(Pair lefts[S], Pair downs[S], Pair sizes) {
  int out = 0;
  Pair leftD = {1, 0};
  Pair downD = {0, 1};

  out = out | semiStep(lefts, sizes.x, leftD);
  out = out | semiStep(downs, sizes.y, downD);

  return out;
}

void printBoard() {
  for (int i = 0; i < height; i++) {
    printf("%s\n", board[i]);
  }
}

void part1(const char *inputLocation) {
  Pair lefts[S];
  Pair rights[S];
  Pair o = parseInput(inputLocation, lefts, rights);
  int out = 1;
  printBoard();
  while(step(lefts, rights, o)) out ++;
  printBoard();
  printf("part 1: %d\n", out);
}

void part2(const char *inputLocation) { printf("part 2: 50 stars\n"); }

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
