#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char input[10][8];
  char output[4][8];
} Input;

List *getInput(const char *loc) {
  List *out = newList();
  FILE *f = openFile(loc);

  char buff[256];
  while (fscanf(f, "%s", buff) > 0) {
    Input *inp = malloc(sizeof(Input));

    stpcpy(inp->input[0], buff);
    for (int i = 1; i < 10; i++) {
      fscanf(f, "%s", inp->input[i]);
      inp->input[i][7] = '\0';
    }
    fscanf(f, "%s", buff);
    for (int i = 0; i < 4; i++) {
      fscanf(f, "%s", inp->output[i]);
      inp->output[i][7] = '\0';
    }

    listAdd(out, inp);
  }

  return out;
}

long LEDS[10] = {
//  aaaaaa   bbbbbb   cccccc   dddddd   eeeeee   ffffff   gggggg
    1 << 0 | 1 << 1 | 1 << 2 | 0 << 3 | 1 << 4 | 1 << 5 | 1 << 6, // 0
    0 << 0 | 0 << 1 | 1 << 2 | 0 << 3 | 0 << 4 | 1 << 5 | 0 << 6, // 1
    1 << 0 | 0 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 0 << 5 | 1 << 6, // 2
    1 << 0 | 0 << 1 | 1 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 1 << 6, // 3
    0 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 0 << 6, // 4
    1 << 0 | 1 << 1 | 0 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 1 << 6, // 5
    1 << 0 | 1 << 1 | 0 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6, // 6
    1 << 0 | 0 << 1 | 1 << 2 | 0 << 3 | 0 << 4 | 1 << 5 | 0 << 6, // 7
    1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6, // 8
    1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 0 << 4 | 1 << 5 | 1 << 6, // 9
};

int determineValue(char **input, char **output) {



   return 0; 
}

void part1(const char *inputLocation) {
  List *inps = getInput(inputLocation);
  Input **l = (Input **)inps->list;
  int out = 0;

  for (int j = 0; j < inps->size; j++) {
    for (int i = 0; i < 4; i++) {
        int len = strlen(l[j]->output[i]);
        if(len == 2 || len == 7 || len == 4 || len == 3) out ++;
    }
  }
  printf("part 1: %d\n", out);
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
