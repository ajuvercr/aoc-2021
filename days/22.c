#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { OFF, ON } State;

typedef struct {
  int start;
  int end;
} Range;

typedef struct {
  Range x;
  Range y;
  Range z;
  State state;
} Cube;

Range parseRange(FILE *f) {
  char buff[2];
  Range out;
  fscanf(f, "%d%c%c%d", &out.start, buff, &buff[1], &out.end);
  return out;
}

State parseState(FILE *f) {
  char buff[3];
  fread(buff, 1, 3, f);
  State out;
  if (buff[2] == 'f') {
    out = OFF;
    getc(f);
  } else {
    out = ON;
  }
  return out;
}

void parseCube(FILE *f, Cube *cube) {
  char buff[256];
  cube->state = parseState(f);
  fread(buff, 1, 2, f);
  cube->x = parseRange(f);
  fread(buff, 1, 3, f);
  cube->y = parseRange(f);
  fread(buff, 1, 3, f);
  cube->z = parseRange(f);
}

Cube cubes[420];
int cubc = 0;

void parseCubes(const char *loc) {
  FILE *f = openFile(loc);
  int cont = 1;
  while (cont) {
    parseCube(f, &cubes[cubc++]);
    cont = getc(f) == '\n';
  }
}

void part1(const char *inputLocation) {
  parseCubes(inputLocation);
  long number = 0;
  State powers[101][101][101] = {{{OFF}}};

  for (int k = 0; k < cubc; k++) {
    Cube *c = cubes + k;
    printf("state %d\n", c->state);

    if (abs(c->x.start) >= 50)
      break;

    for (int x = c->x.start + 50; x <= c->x.end + 50; x++) {
      for (int y = c->y.start + 50; y <= c->y.end + 50; y++) {
        for (int z = c->z.start + 50; z <= c->z.end + 50; z++) {
          powers[x][y][z] = c->state;
        }
      }
    }
  }
    for (int x = 0; x <= 100; x++) {
      for (int y = 0; y <= 100; y++) {
        for (int z = 0; z <= 100; z++) {
          number += powers[x][y][z];
        }
      }
    }

  printf("part 1: %ld\n", number);
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
