#include "../lib/files.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256000

char BUFFER[400] = {0};

typedef struct Polymer Polymer;

struct Polymer {
  Polymer *next;
  char value;
};

int poly_count;
Polymer polymers[SIZE];

int cmpF(char *a, char *b) { return !strcmp(a, b); }

Map *parseInput(const char *loc) {
  FILE *f = openFile(loc);

  Map *map = newMap((CmpF)cmpF);

  char buf[256] = {0};
  fscanf(f, "%s\n", buf);
  printf("tart %s\n", buf);
  poly_count = strlen(buf);
  for (int i = 0; i < poly_count; i++) {
    Polymer *p = polymers + i;
    p->value = buf[i];
    if (i < poly_count - 1)
      p->next = &polymers[i + 1];
  }

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

void expand(Map *map) {
  char buffer[3] = {0};
  Polymer *first = polymers;
  Polymer *second = first->next;

  int current_size = poly_count;
  for (int i = 0; i < current_size - 1; i++) {
    buffer[0] = first->value;
    buffer[1] = second->value;
    char insert = (char)(long)mapGet(map, buffer);

    if (insert) {
      Polymer *new = polymers + poly_count;
      poly_count++;
      first->next = new;
      new->next = second;
      new->value = insert;
    }

    first = second;
    second = first->next;
  }
}

int calcOutput() {
  int numbers[256] = {0};
  Polymer *current = polymers;
  while (current) {
    numbers[current->value]++;
    current = current->next;
  }
  int min = 1000000000;
  int max = 0;

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
  Map *m = parseInput(inputLocation);

  for (int i = 0; i < 10; i++)
    expand(m);
  printf("part 1: %d\n", calcOutput());

  freeMap(m, 0, 0);
}

void part2(const char *inputLocation) { printf("part 2: "); }

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
