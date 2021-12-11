#include "../lib/colors.h"
#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FOR(v, max) for (int(v) = 0; (v) < (max); (v)++)
#define FORM(v, min, max) for (int(v) = (min); (v) < (max); (v)++)

int octos[10][10];

void readInput(const char *locc) {
  FILE *f = openFile(locc);

  char buff[2] = {0};
  FOR(i, 10) {
    FOR(j, 10) {
      fscanf(f, "%c", buff);
      octos[i][j] = atoi(buff);
    }
    fscanf(f, "%c", buff);
  }

  fclose(f);
}

void printBoard() {
  FOR(i, 10) {
    FOR(j, 10) {
      int v = octos[i][j];
      if (v == 0)
        v = -3;
      setBgColor(232 + v * 2);
      printf("  ");
    }
    reset_color();
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
  FOR(i, 10) {
    FOR(j, 10) {
      octos[i][j]++;
      checkOcto(todo, i, j, &at);
    }
  }

  FOR(z, at) {
    int i = todo[z][0];
    int j = todo[z][1];

    FORM(x, i - 1, i + 2) {
      FORM(y, j - 1, j + 2) {
        if (x < 0 || y < 0 || x > 9 || y > 9)
          continue;
        octos[x][y]++;
        checkOcto(todo, x, y, &at);
      }
    }
  }

  FOR(z, at) {
    int i = todo[z][0];
    int j = todo[z][1];
    octos[i][j] = 0;
  }

  return at;
}

void part1(const char *inputLocation) {
  readInput(inputLocation);

  int c = 0;
  FOR(i, 100) { c += step(); }
  printf("part 1: %d\n", c);
}

void part2(const char *inputLocation) {
  readInput(inputLocation);

  int flashes = 0;
  int i = 0;
  while ((flashes = step()) != 100) {
#ifdef ANIM
    if (i) {
      moveTermLeft();
      moveTermUp(10);
    }
    printBoard();
    usleep(100000);
#endif
    i++;
  }
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
