#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

void parseInt(const char *loc, unsigned int board[SIZE * 5][SIZE * 5][2]) {
  FILE *f = openFile(loc);
  char buff[2] = "";
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      buff[0] = getc(f);
      board[i][j][0] = atol(buff);
      board[i][j][1] = ~0 - 1000;
    }
    getc(f);
  }
}

#define CUR board[i][line]
void walkLine(unsigned int board[SIZE * 5][SIZE * 5][2], int line, int size) {
  for (int i = 0; i < size; i++) {
    int m = CUR[0];
    if (line > 0) {
      int v = board[i][line - 1][1] + m;
      if (board[i][line][1] > v)
        board[i][line][1] = v;
    }
    if (line + 1 < size) {
      int v = board[i][line + 1][1] + m;
      if (board[i][line][1] > v)
        board[i][line][1] = v;
    }
    if (i > 0) {
      int v = board[i - 1][line][1] + m;
      if (board[i][line][1] > v)
        board[i][line][1] = v;
    }
    if (i + 1 < size) {
      int v = board[i + 1][line][1] + m;
      if (board[i][line][1] > v)
        board[i][line][1] = v;
    }
  }
}

void part1(const char *inputLocation) {
  unsigned int board[SIZE * 5][SIZE * 5][2];
  parseInt(inputLocation, board);
  board[0][0][0] = 0;
  board[0][0][1] = 0;

  unsigned int cur = 0;
  while (cur != board[SIZE - 1][SIZE - 1][1]) {
    cur = board[SIZE - 1][SIZE - 1][1];
    for (int i = 0; i < SIZE; i++)
      walkLine(board, i, SIZE);
  }
  printf("part 1: %d\n", board[SIZE - 1][SIZE - 1][1]);
}

void part2(const char *inputLocation) {
  unsigned int board[SIZE * 5][SIZE * 5][2];
  parseInt(inputLocation, board);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (i == 0 && j == 0)
        continue;
      int ti, tj;
      if (i == 0) {
        ti = i;
        tj = j - 1;
      } else {
        ti = i - 1;
        tj = j;
      }

      for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
          int v = board[ti * SIZE + x][tj * SIZE + y][0] + 1;
          if (v > 9)
            v = 1;
          board[i * SIZE + x][j * SIZE + y][0] = v;
          board[i * SIZE + x][j * SIZE + y][1] = ~0 - 1000;
        }
      }
    }
  }

  board[0][0][0] = 0;
  board[0][0][1] = 0;

  unsigned int cur = 0;
  int T = SIZE * 5 - 1;
  while (cur != board[T][T][1]) {
    cur = board[T][T][1];
    for (int i = 0; i < SIZE * 5; i++)
      walkLine(board, i, SIZE * 5);
  }
  printf("part 2: %d\n", board[T][T][1]);
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
