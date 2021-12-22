#include "../lib/files.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S_SIZE 36
#define OVERLAP 12

typedef int Orientation;

typedef struct Beacon {
  int x;
  int y;
  int z;
} Beacon;

typedef struct Scanner Scanner;

typedef struct {
  Beacon *inner;
  int a;
  int b;
  int size;
} Delta;

int cmp_delta(Beacon *a, Beacon *b) {
  return a->x == -b->x && a->y == -b->y && a->z == -b->z;
}

struct Scanner {
  Beacon beacons[32];
  int b_count;
  Delta deltas[32 * 16];
  int d_count;
  Orientation orientation;
  int used;
  int ox, oy, oz;
};

int A[3][3][3] = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
                  {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},
                  {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}}};
int B[4][3][3] = {
    {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
    {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
    {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
    {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
};

int C[2][3][3] = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
                  {{0, 0, -1}, {0, -1, 0}, {-1, 0, 0}}};

void mul(int *xp, int *yp, int *zp, int m[3][3]) {
  int x = *xp;
  int y = *yp;
  int z = *zp;
  *xp = x * m[0][0] + y * m[0][1] + z * m[0][2];
  *yp = x * m[1][0] + y * m[1][1] + z * m[1][2];
  *zp = x * m[2][0] + y * m[2][1] + z * m[2][2];
}

void rotateXYZ(Orientation o, int *xp, int *yp, int *zp) {
  int a = o % 3;
  o = o / 3;
  int b = o % 4;
  o = o / 4;
  int c = o % 2;
  mul(xp, yp, zp, A[a]);
  mul(xp, yp, zp, B[b]);
  mul(xp, yp, zp, C[c]);
}



Beacon rotateBeacon(Orientation o, Beacon *i) {
    Beacon b = { i->x, i->y, i->z};
  rotateXYZ(o, &b.x, &b.y, &b.z);
  return b;
}

void parseBeacon(char *line, Beacon *beacon) {
  int i = 0;
  while (line[i] != ',')
    i++;
  line[i] = '\0';
  beacon->x = atoi(line);
  line += i + 1;
  i = 0;

  while (line[i] != ',')
    i++;
  line[i] = '\0';
  beacon->y = atoi(line);
  line += i + 1;
  beacon->z = atoi(line);
}

void parseInput(const char *loc, Scanner scanners[S_SIZE]) {
  FILE *f = openFile(loc);
  List *lines = readLines(f);
  char **ll = (char **)lines->list;
  int size = lines->size;
  fclose(f);

  int l = 0;
  for (int i = 0; i < S_SIZE; i++) {
    Scanner *current = &scanners[i];
    l += 4; // ignore scanner name

    while (l < size && ll[l][1] != '-') {
      parseBeacon(ll[l], &current->beacons[current->b_count++]);
      l++;
    }
  }
  freeList(lines, free);
}

void setDeltas(Scanner *scanner) {
  int a = 0;
  for (int i = 0; i < scanner->b_count; i++) {
    Beacon l = scanner->beacons[i];
    for (int j = i + 1; j < scanner->b_count; j++) {
      if (i == j)
        continue;
      Beacon k = scanner->beacons[j];
      Beacon *d = malloc(sizeof(Beacon) * 25);
      scanner->deltas[a].inner = d;
      d[0].x = l.x - k.x;
      d[0].y = l.y - k.y;
      d[0].z = l.z - k.z;
      scanner->deltas[a].size = abs(d[0].x) + abs(d[0].y) + abs(d[0].z);
      for (int i = 1; i < 25; i++) {
        d[i].x = d[0].x;
        d[i].y = d[0].y;
        d[i].z = d[0].z;
        rotateXYZ(i, &d[i].x, &d[i].y, &d[i].z);
      }

      scanner->deltas[a].a = i;
      scanner->deltas[a].b = j;
      a++;
    }
  }
  scanner->d_count = a;
}

int filterSize(Scanner *scanner, int size, int buffer[1024]) {
  int out = 0;
  Delta *deltas = scanner->deltas;

  for (int i = 0; i < scanner->d_count; i++)
    if (deltas[i].size == size)
      buffer[out++] = i;

  return out;
}

Delta *contains(Scanner *scanner, Beacon *beacon, int count, int buffer[1024]) {
  Delta *deltas = scanner->deltas;

  for (int j = 0; j < count; j++)
    if (cmp_delta(&deltas[buffer[j]].inner[scanner->orientation], beacon))
      return &deltas[buffer[j]];

  return 0;
}

Beacon BEACONS[2048];
int BC = 0;

void maybeAddPoint(Scanner *scanner, Beacon *beacon) {
  int x = scanner->ox + beacon->x;
  int y = scanner->oy + beacon->y;
  int z = scanner->oz + beacon->z;

  for (int i = 0; i < BC; i++) {
    if (BEACONS[i].x == x && BEACONS[i].y == y && BEACONS[i].z == z)
      return;
  }

  BEACONS[BC].x = x;
  BEACONS[BC].y = y;
  BEACONS[BC].z = z;

  BC++;
}

int tryAlign(Scanner *root, Scanner *scanner) {
  int buffer[1024];
  int orientation = -1;
  int overlaps[25] = {0};
  int points[25] = {0};
  Beacon centers[25] = {{0}};

  for (int i = 0; i < scanner->d_count; i++) {
    Delta *orig = &scanner->deltas[i];

    int count = filterSize(root, orig->size, buffer);

    for (int o = 0; o < 25; o++) {
      Beacon *d = &orig->inner[o];

      Delta *found = contains(root, d, count, buffer);
      if (found) {
        if (!overlaps[o]) {
          Beacon *oa = &root->beacons[found->a];
          Beacon *ca = &scanner->beacons[orig->b];

          centers[o] = rotateBeacon(o, ca);
          centers[o].x += -root->ox -oa->x;
          centers[o].y += -root->oy-oa->y;
          centers[o].x += -root->ox  -oa->z;
          /* int x = ca->x;
          int y = ca->y;
          int z = ca->z;
          rotateXYZ(o, &x, &y, &z);
          centers[o].x = root->ox + oa->x - x;
          centers[o].y = root->oy + oa->y - y;
          centers[o].z = root->oz + oa->z - z; */
        }

        long a = 1 << orig->a;
        long b = 1 << orig->b;
        if (!(points[o] & a)) {
          points[o] = points[o] | a;
          overlaps[o]++;
          if (overlaps[o] >= 12) {
            orientation = o;
            break;
          }
        }

        if (!(points[o] & b)) {
          points[o] = points[o] | b;
          overlaps[o]++;
          if (overlaps[o] >= 12) {
            orientation = o;
            break;
          }
        }
      }
    }
    if (orientation > -1)
      break;
  }

  if (orientation > -1) { // found match!
    scanner->ox = centers[orientation].x;
    scanner->oy = centers[orientation].y;
    scanner->oz = centers[orientation].z;
    scanner->orientation = orientation;
    scanner->used = 1;

    for (int i = 0; i < scanner->b_count; i++) {
      scanner->beacons[i] = rotateBeacon(orientation, scanner->beacons + i);
      maybeAddPoint(scanner, scanner->beacons + i);
    }

    return 1;
  }
  return 0;
}

void part1(const char *inputLocation) {
  Scanner scanners[S_SIZE] = {{{{0}}}};
  parseInput(inputLocation, scanners);
  for (int i = 0; i < S_SIZE; i++)
    setDeltas(&scanners[i]);

  for (int i = 0; i < scanners[0].b_count; i++)
    maybeAddPoint(scanners, scanners[0].beacons + i);
  scanners[0].ox = 0;
  scanners[0].oy = 0;
  scanners[0].oz = 0;
  scanners[0].orientation = 0;
  scanners[0].used = 1;

  int indecis[S_SIZE];
  int at = 0;
  indecis[at++] = 0;

  for (int i = 0; i < S_SIZE; i++) {
    Scanner *root = &scanners[indecis[i]];

    for (int j = 0; j < S_SIZE; j++) {
      if (scanners[j].used == 1)
        continue;
      if (tryAlign(root, &scanners[j]))
        indecis[at++] = j;
    }
  }

  int maxDist = 0;
  for (int i = 0; i < S_SIZE; i++) {
    for (int j = 0; j < S_SIZE; j++) {
      int dx = abs(scanners[i].ox - scanners[j].ox);
      int dy = abs(scanners[i].oy - scanners[j].oy);
      int dz = abs(scanners[i].oz - scanners[j].oz);
      int dist = dx + dy + dz;
      if (dist > maxDist)
        maxDist = dist;
    }
  }

  for (int i = 0; i < S_SIZE; i++)
    for (int j = 0; j < scanners[i].d_count; j++)
      free(scanners[i].deltas[j].inner);

  printf("part 1: %d\n", BC);
  printf("part 2: %d\n", maxDist);
}

void part2(const char *inputLocation) {}

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
