#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int State;
#define ON 1
#define OFF 0
#define NEG -1

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
  if (out.end < out.start) {
    int z = out.end;
    out.end = out.start;
    out.start = z;
  }
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

Cube cubes[425] = {{{0}}};
int cubc = 0;

void parseCubes(const char *loc) {
  FILE *f = openFile(loc);
  int cont = 1;
  while (cont) {
    parseCube(f, &cubes[cubc++]);
    cont = getc(f) == '\n';
  }
  cubc--;
}

void part1(const char *inputLocation) {
  parseCubes(inputLocation);
  long number = 0;
  State powers[101][101][101] = {{{OFF}}};

  for (int k = 0; k < cubc; k++) {
    Cube *c = cubes + k;
    if (labs(c->x.start) >= 50)
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

typedef struct Holder Holder;

struct Holder {
  Cube *c;
  Holder *next;
};

#define HOLDERC 420 * 422 * 100
#define INTERC 420 * 422 * 100
Holder holders[HOLDERC];
int holder_c = 0;
Cube intersections[INTERC];
int inter_c = 0;

int _min(int x, int y) { return x < y ? x : y; }

int _max(int x, int y) { return x > y ? x : y; }

Range intersectRange(Range *a, Range *b) {
  return (Range){_max(a->start, b->start), _min(a->end, b->end)};
}

Cube *interset(Cube *a, Cube *b) {
  Range x = intersectRange(&a->x, &b->x);
  Range y = intersectRange(&a->y, &b->y);
  Range z = intersectRange(&a->z, &b->z);
  if (x.start > x.end || y.start > y.end || z.start > z.end) {
    return 0;
  }
  if (inter_c == INTERC) {
    printf("nope\n");
    exit(1);
  }

  Cube *out = &intersections[inter_c++];
  out->x = x;
  out->y = y;
  out->z = z;
  out->state = -1 * a->state;
  return out;
}
long delta(Range r) { return ((long)r.end) - ((long)r.start) + 1; }

long volume(Cube *cube) {
  return ((long)cube->state) * delta(cube->x) * delta(cube->y) * delta(cube->z);
  long out = (long)cube->state * (long)(cube->x.end - cube->x.start + 1) *
             (long)(cube->y.end - cube->y.start + 1) *
             (long)(cube->z.end - cube->z.start + 1);
  return out;
}

Holder *nHolder() {
  if (holder_c == HOLDERC) {
    printf("NOpe holder\n");
    exit(1);
  }
  return &holders[holder_c++];
}

// test : 2758514936282235
void part2(const char *inputLocation) {
  cubc = 0;
  parseCubes(inputLocation);
  holder_c = 0;
  Holder *start = 0;

  for (int i = 0; i < cubc; i++) {
    Cube *newCube = &cubes[i];
    Holder *current = start;
    while (current) {
      Cube *intersection = interset(current->c, newCube);

      if (intersection) {
        Holder *n = nHolder();
        n->next = start;
        n->c = intersection;
        start = n;
      }

      current = current->next;
    }

    if (newCube->state == ON) {
      Holder *n = nHolder();
      n->next = start;
      n->c = newCube;
      start = n;
    }
  }

  long total = 0;
  Holder *current = start;
  while (current) {
    total += volume(current->c);
    current = current->next;
  }

  printf("part 2: %ld\n", total);
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
