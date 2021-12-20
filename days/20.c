#include "../lib/files.h"
#include "../lib/list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define margin 55
#define size (100 + margin * 2)

char *algo = 0;
char image[2][1024][size + 1] = {{{'.'}}};

void parseInput(const char *loc) {
  FILE *f = openFile(loc);

  size_t len = 0;
  getline(&algo, &len, f); // get algo
  getc(f);                 // skip newline

  for (int i = 0; i < margin; i++) {
    for (int j = 0; j < size; j++) {
      image[0][i][j] = '.';
    }
    image[0][i][size] = '\0';
  }
  int i = margin;
  while (fgets(image[0][i] + margin, size, f) != NULL) {
    for (int k = 0; k < margin; k++)
      image[0][i][k] = '.';
    for (int k = size - margin; k < size; k++)
      image[0][i][k] = '.';
    image[0][i][size] = '\0';
    i++;
  }

  for (int i = size - margin; i < size; i++) {
    for (int j = 0; j < size; j++) {
      image[0][i][j] = '.';
    }
    image[0][i][size] = '\0';
  }
}

int getIndex(const char *in) {
  int out = 0;
  int i = 0;
  while (in[i] != '\0') {
    out <<= 1;
    if (in[i++] == '#')
      out += 1;
  }
  return out;
}

char enhance(int i, int j, int k) {
  char buffer[10] = "";

  int at = 0;
  for (int di = -1; di < 2; di++) {
    int x = i + di;
    for (int dj = -1; dj < 2; dj++) {
      int y = j + dj;
      if (x < 0 || y < 0 || x >= size || y >= size)
        buffer[at++] = k == 0 ? '.' : '#';
      else
        buffer[at++] = image[k][x][y];
    }
  }
  buffer[at] = '\0';

  return algo[getIndex(buffer)];
}

void enhanceImage(int index) {
  int k = (index + 1) % 2;

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      // printf("(%d,%d,%c) ", i, j, image[index][i][j]);
      image[k][i][j] = enhance(i, j, index);
    }
  }
}

int countOn(int index) {
  int out = 0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (image[index][i][j] == '#')
        out++;
    }
  }
  return out;
}

void print(int index) {
  for (int i = 0; i < size; i++) {
    printf("%s\n", image[index][i]);
  }
}

void part1(const char *inputLocation) {
  parseInput(inputLocation);
  enhanceImage(0);
  enhanceImage(1);
  printf("part 1: %d\n", countOn(0));
}

void part2(const char *inputLocation) {
  parseInput(inputLocation);
  for (int i = 0; i < 50; i++) {
    enhanceImage(i % 2);
  }
  printf("part 2: %d\n", countOn(0));
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
