#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int octos[10][10];

void readInput(const char *locc) {
  FILE *f = openFile(locc);

  char buff[2] = {0};
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      fscanf(f, "%c", buff);
      octos[i][j] = atoi(buff);
    }
    fscanf(f, "%c", buff);
  }

  fclose(f);
}

void printBoard() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("%d", octos[i][j]);
    }
    printf("\n");
  }
}

void checkOcto(int todo[1024][2], int i, int j, int *at) {
  if (octos[i][j] > 9) {
    todo[*at][0] = i;
    todo[*at][1] = j;
    *at += 1;
    octos[i][j] = -1024;
  }
}

int step() {
  int todo[1024][2];
  int at = 0;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      octos[i][j]++;
      checkOcto(todo, i, j, &at);
    }
  }

  for (int z = 0; z < at; z++) {
    int i = todo[z][0];
    int j = todo[z][1];

    for (int di = -1; di < 2; di++) {
      for (int dj = -1; dj < 2; dj++) {
        int x = i + di;
        int y = j + dj;
        if (x < 0 || y < 0 || x > 9 || y > 9)
          continue;
        octos[x][y]++;
        checkOcto(todo, x, y, &at);
      }
    }
  }

  for (int z = 0; z < at; z++) {
    int i = todo[z][0];
    int j = todo[z][1];
    octos[i][j] = 0;
  }

  return at;
}

void part1(const char *inputLocation) {
  readInput(inputLocation);

  int c = 0;
  for (int i = 0; i < 100; i++)
    c += step();
  printf("part 1: %d\n", c);
}

void part2(const char *inputLocation) {
  readInput(inputLocation);

  int flashes = 0;
  int i = 0;
  while ((flashes = step()) != 100)
    i++;

  printf("part 2: %d\n", i);
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
