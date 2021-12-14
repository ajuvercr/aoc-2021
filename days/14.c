#include "../lib/files.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256000

short shorten(char alpha, char beta) {
  return ((short)(alpha - 'A') << 5) | (short)(beta - 'A');
}

void longen(short in, char *alpha, char *beta) {
  *alpha = 'A' + (0x1f & (in >> 5));
  *beta = 'A' + (0x1f & in);
}

char BUFFER[400] = {0};
char last_char;

int cmpF(char *a, char *b) { return !strcmp(a, b); }

Map *parseInput(const char *loc, unsigned long pairs[1024]) {
  FILE *f = openFile(loc);

  Map *map = newMap((CmpF)cmpF);

  char buf[256] = {0};
  fscanf(f, "%s\n", buf);
  int len = strlen(buf);
  for (int i = 1; i < len; i++) {
    short pair = shorten(buf[i - 1], buf[i]);
    pairs[pair]++;
  }
  last_char = buf[len - 1];

  int i = 0;
  char c = '\0';
  while (fgets(&BUFFER[i * 3], 3, f)) {
    fgets(buf, 5, f);
    c = fgetc(f);
    fgetc(f);
    mapAdd(map, &BUFFER[i * 3], (void *)(long)c);
    i++;
  }

  fclose(f);

  return map;
}

void expand(Map *map, unsigned long pairs1[1024], unsigned long pairs2[1024]) {
  char buffer[3] = {0};
  char alpha, beta;
  for (short i = 0; i < 1024; i++) {
    if (pairs1[i]) {
      longen(i, &alpha, &beta);

      buffer[0] = alpha;
      buffer[1] = beta;
      char insert = (char)(long)mapGet(map, buffer);

      if (insert) {
        short one = shorten(alpha, insert);
        short two = shorten(insert, beta);
        pairs2[one] += pairs1[i];
        pairs2[two] += pairs1[i];
      } else {
        printf("oh no\n");
        pairs2[i] = pairs1[i];
      }
    }
  }
}

unsigned long calcOutput(unsigned long pairs[1024]) {
  unsigned long numbers[256] = {0};
  char a, b;
  unsigned long min = ~0;
  unsigned long max = 0;
  numbers[(int) last_char] = 1;

  for (int i = 0; i < 1024; i++) {
    if (pairs[i]) {
      longen(i, &a, &b);
      numbers[(int)a] += pairs[i];
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
  unsigned long pairs1[1024] = {0};
  unsigned long pairs2[1024] = {0};
  Map *m = parseInput(inputLocation, pairs1);

  for (int i = 0; i < 10; i++) {
    expand(m, pairs1, pairs2);
    memmove(pairs1, pairs2, sizeof(long) * 1024);
    memset(pairs2, 0, sizeof(long) * 1024);
  }
  printf("part 1: %ld\n", calcOutput(pairs1));

  freeMap(m, 0, 0);
}

void part2(const char *inputLocation) {
  unsigned long pairs1[1024] = {0};
  unsigned long pairs2[1024] = {0};
  Map *m = parseInput(inputLocation, pairs1);

  for (int i = 0; i < 40; i++) {
    expand(m, pairs1, pairs2);
    memmove(pairs1, pairs2, sizeof(long) * 1024);
    memset(pairs2, 0, sizeof(long) * 1024);
  }
  printf("part 2: %ld\n", calcOutput(pairs1));

  freeMap(m, 0, 0);
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
