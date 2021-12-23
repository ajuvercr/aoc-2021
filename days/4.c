#include "../lib/files.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int intPair[2];

typedef struct {
  int rows[5];
  int columns[5];
  Map *map;
  int total;
} Bingo;

int intCmp(long a, long b) { return a == b; }

Bingo *newBoard() {
  Bingo *out = malloc(sizeof(Bingo));
  for (int i = 0; i < 5; i++) {
    out->rows[i] = 0;
    out->columns[i] = 0;
  }
  out->total = 0;
  out->map = newMap((CmpF)intCmp);
  return out;
}

void freeBingo(Bingo *bingo) {
  freeMap(bingo->map, 0, 0);
  free(bingo);
}

int handleInput(Bingo *board, int input) {
    long a = (long) mapGet(board->map, (void *) (long) input);
    if(!a) return 0;

    if(a == -1) a = 0;

    int at[2] = {(int) (a), (int) (a >> 32)};

    board->total -= input;
    board->rows[at[0]] ++;
    board->columns[at[1]] ++;

    return board->rows[at[0]] == 5 || board->columns[at[1]] == 5;
}

void part1(const char *inputLocation) {
  FILE *file = openFile(inputLocation);
  List *inputs = newList();
  long input = 0;
  char sep = '\0';
  while (fscanf(file, "%ld%c", &input, &sep) > 0) {
    listAdd(inputs, (void *)input);
    if (sep != ',')
      break;
  }

  List *sqrs = newList();
  int n = 0;
  while (fscanf(file, "%d", &n) > 0) {
    Bingo *board = newBoard();

    listAdd(sqrs, board);

    long x = -1;
    board->total += n;
    mapAdd(board->map, (void *)(long)n, (void *)x);

    for (long i = 0; i < 5; i++) {
      for (long j = 0; j < 5; j++) {
        if (i == 0 && j == 0)
          continue;

        fscanf(file, "%d", &n);

        long v = (i | (j << 32));

        board->total += n;
        mapAdd(board->map, (void *)(long)n, (void *)v);
      }
    }
  }

  long *list = (long *)inputs->list;
  Bingo **rsqrs = (Bingo **)sqrs->list;
  int first = 1;
  long lastInput = -1;
  Bingo *lastSq = 0;
  for (int i = 0; i < inputs->size; i++) {
    long input = list[i];

    for (int j = 0; j < sqrs->size; j++) {
      if (rsqrs[j] && handleInput(rsqrs[j], input)) {
        if (first) {
          printf("part 1: %ld\n", rsqrs[j]->total * input);
        }
        first = 0;
        lastInput = input;
        lastSq = rsqrs[j];
        rsqrs[j] = 0;
      }
    }
  }
  printf("part 2: %ld\n", lastSq->total * lastInput);
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
