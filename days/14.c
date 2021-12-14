#include "../lib/files.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 700

short shorten(char alpha, char beta) {
  return ((short)(alpha) << 5) | (short)(beta);
}

void longen(short in, char *alpha, char *beta) {
  *alpha = (0x1f & (in >> 5));
  *beta = (0x1f & in);
}

char parseInput(const char *loc, unsigned long pairs[SIZE], char map[SIZE]) {
  FILE *f = openFile(loc);

  char buf[256] = {0};

  fscanf(f, "%s\n", buf);
  int len = strlen(buf);
  for (int i = 1; i < len; i++) {
    short pair = shorten(buf[i - 1] - 'A', buf[i] - 'A');
    pairs[pair]++;
  }

  char last_char = buf[0];

  while (fgets(buf, 9, f)) {
    short pair = shorten(buf[0] - 'A', buf[1] - 'A');
    map[pair] = buf[6] - 'A';
  }

  fclose(f);
  return last_char;
}

void expand(char map[SIZE], unsigned long pairs1[SIZE],
            unsigned long pairs2[SIZE]) {
  char alpha, beta;
  for (short i = 0; i < SIZE; i++) {
    if (pairs1[i]) {
      char insert = map[i];
      longen(i, &alpha, &beta);

      short one = shorten(alpha, insert);
      short two = shorten(insert, beta);
      pairs2[one] += pairs1[i];
      pairs2[two] += pairs1[i];
    }
  }
}

unsigned long calcOutput(unsigned long pairs[SIZE], int last_char) {
  unsigned long numbers[256] = {0};
  unsigned long min = ~0, max = 0;

  numbers[last_char - 'A'] = 1;

  for (unsigned int i = 0; i < SIZE; i++) {
    if (pairs[i]) {
      unsigned int a =  (0x1f & i);
      numbers[a] += pairs[i];
    }
  }

  for (int i = 0; i < 256; i++) {
    if (numbers[i]) {
      if (numbers[i] > max)
        max = numbers[i];
      if (numbers[i] < min)
        min = numbers[i];
    }
  }

  return max - min;
}

void part1(const char *inputLocation) {
  unsigned long pairs1[SIZE] = {0};
  unsigned long pairs2[SIZE] = {0};
  char map[SIZE] = {0};
  char last_char = parseInput(inputLocation, pairs1, map);

  for (int i = 0; i < 10; i++) {
    expand(map, pairs1, pairs2);
    memmove(pairs1, pairs2, sizeof(long) * SIZE);
    memset(pairs2, 0, sizeof(long) * SIZE);
  }
  printf("part 1: %ld\n", calcOutput(pairs1, last_char));
}

void part2(const char *inputLocation) {
  unsigned long pairs1[SIZE] = {0};
  unsigned long pairs2[SIZE] = {0};
  char map[SIZE] = {0};
  char last_char = parseInput(inputLocation, pairs1, map);

  for (int i = 0; i < 40; i++) {
    expand(map, pairs1, pairs2);
    memmove(pairs1, pairs2, sizeof(long) * SIZE);
    memset(pairs2, 0, sizeof(long) * SIZE);
  }
  printf("part 2: %ld\n", calcOutput(pairs1, last_char));
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
