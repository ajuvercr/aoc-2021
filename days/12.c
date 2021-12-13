#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_S 15

int getIndex(List *l, char *x) {
  for (int i = 0; i < l->size; i++) {
    if (strcmp(listGet(l, i), x) == 0)
      return i;
  }
  return -1;
}

char map[MAP_S][MAP_S] = {0};

void printMap() {
  for (int i = 0; i < MAP_S; i++) {
    for (int j = 0; j < MAP_S; j++) {
      printf("%d", map[i][j]);
    }
    printf("\n");
  }
}

List *buildMap(const char *loc) {
  FILE *f = openFile(loc);
  List *lines = readLines(f);

  List *out = newList();
  listAdd(out, "start");
  listAdd(out, "end");

  char **ll = (char **)lines->list;
  for (int i = 0; i < lines->size; i++) {
    char *start = ll[i];
    int j = 0;
    while (start[j] != '-')
      j++;
    start[j] = '\0';
    char *end = start + j + 1;

    int start_i = getIndex(out, start);
    if (start_i < 0) {
      start_i = out->size;
      listAdd(out, start);
    }

    int end_i = getIndex(out, end);
    if (end_i < 0) {
      end_i = out->size;
      listAdd(out, end);
    }

    map[start_i][end_i] = 1;
    map[end_i][start_i] = 1;
  }

  freeList(lines, 0);
  fclose(f);
  return out;
}

void dfs(char **caves, int current, int *done, int smallIsK, int *successful) {
  if (current == 1) {
    *successful += 1;
    return;
  }

  int old = done[current];

  if (done[current]) {
    if (smallIsK && current != 0)
      smallIsK = 0;
    else
      return;
  }

  char c = caves[current][0];
  if (c >= 'a' && c <= 'z')
    done[current] = 1;

  for (int i = 0; i < MAP_S; i++)
    if (map[current][i])
      dfs(caves, i, done, smallIsK, successful);

  done[current] = old;
}

void part1(const char *inputLocation) {
  List *caves = buildMap(inputLocation);
  char **ll = (char **)caves->list;
  int done[MAP_S] = {0};
  int succesful = 0;
  dfs(ll, 0, done, 0, &succesful);

  printf("part 1: %d\n", succesful);
  freeList(caves, 0);
}

void part2(const char *inputLocation) {
  List *caves = buildMap(inputLocation);
  char **ll = (char **)caves->list;
  int done[MAP_S] = {0};
  int succesful = 0;
  dfs(ll, 0, done, 1, &succesful);

  printf("part 2: %d\n", succesful);
  freeList(caves, 0);
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
