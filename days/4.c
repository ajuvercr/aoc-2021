#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **malloc_2d_array(int r, int c) {
  int len = sizeof(int *) * r + sizeof(int) * c * r;
  int **arr = malloc(len);
  int *ptr = (int *)(arr + r);
  for (int i = 0; i < r; i++)
    arr[i] = (ptr + c * i);
  return arr;
}

void print(int **sq) {

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%d ", sq[i][j]);
    }
    printf("\n");
  }
    printf("\n");
}
int handleInput(int **sq, int input) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (sq[i][j] == input) {
        sq[i][j] = -1;
        int row = 1, col = 1;
        for (int k = 0; k < 5; k++) {
          row = row && (sq[i][k] < 0);
          col = col && (sq[k][j] < 0);
        }
        if (row || col) {
          return 1;
        }
      }
    }
  }
  return 0;
}


int getUnchecked(int **sq) {
  int out = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (sq[i][j] > 0)
        out += sq[i][j];
    }
  }
  return out;
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
    int **sq = malloc_2d_array(5, 5);
    listAdd(sqrs, sq);
    sq[0][0] = n;

    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        if (i == 0 && j == 0)
          continue;
        fscanf(file, "%d", &n);
        sq[i][j] = n;
      }
    }
  }

  long *list = (long *)inputs->list;
  int ***rsqrs = (int ***)sqrs->list;
  int first = 1;
  long lastInput = -1;
  int** lastSq = 0;
  for (int i = 0; i < inputs->size; i++) {
    long input = list[i];

    for (int j = 0; j < sqrs->size; j++) {
      if (rsqrs[j] && handleInput(rsqrs[j], input)) {
        if (first)
          printf("part 1: %ld\n", getUnchecked(rsqrs[j]) * input);
        first = 0;
        lastInput = input;
        lastSq = rsqrs[j];
        rsqrs[j] = 0;
      }
    }
  }
  printf("part 2: %ld\n", getUnchecked(lastSq) * lastInput);
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
