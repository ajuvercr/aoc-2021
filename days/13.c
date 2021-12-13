#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  enum { Empty, X, Y } type;
  int loc;
} Fold;

#define SIZE 2000
char input[SIZE][SIZE] = {0};
int X_SIZE = SIZE, Y_SIZE = SIZE;
int fold_c = 0;
Fold folds[32];

void readInput(const char *loc) {
  for (int i = 0; i < SIZE; i++)
    memset(input[i], '.', SIZE);
  X_SIZE = SIZE;
  Y_SIZE = SIZE;
  fold_c = 0;

  FILE *f = openFile(loc);

  int x, y;
  while (fscanf(f, "%d,%d\n", &x, &y) > 0)
    input[y][x] = '#';

  char buff[256];
  while (fscanf(f, " %s %s ", buff, &buff[32]) > 0) {
    fscanf(f, "%c=%d\n", &buff[0], &x);
    folds[fold_c].loc = x;
    folds[fold_c].type = buff[0] == 'x' ? Y : X;
    fold_c++;
  }
  fclose(f);
}

void printInput(int s, int x) {
  for (int j = 0; j < s; j++)
    printf("%.*s\n", x, input[j]);
}

void foldUp(int y) {
    for(int i = 0; i < y; i++) {
        int target = y + (y-i);
        for(int j = 0; j < X_SIZE; j ++) {
           char c = input[target][j];
           if(c == '#') {
               input[target][j] = '.';
               input[i][j] = '#';
           }
        }
    }
    Y_SIZE >>= 1;
}

void foldLeft(int x) {
    for(int i = 0; i < x; i++) {
        int target = x + (x-i);
        for(int j = 0; j < Y_SIZE; j ++) {
           char c = input[j][target];
           if(c == '#') {
               input[j][target] = '.';
               input[j][i] = '#';
           }
        }
    }
    X_SIZE >>= 1;
}

void fold(Fold fold) {
  if(fold.type == X) foldUp(fold.loc);
  if(fold.type == Y) foldLeft(fold.loc);
}

int countVisible() {
    int out = 0;
   for(int i = 0 ; i < X_SIZE; i ++)
       for(int j = 0; j < Y_SIZE; j ++)
           if(input[i][j] == '#') out ++;
   return out;
}

void part1(const char *inputLocation) {
  readInput(inputLocation);
  fold(folds[0]);
  printf("part 1: %d\n", countVisible());
}

void part2(const char *inputLocation) { 
    readInput(inputLocation);
    for(int i = 0; i < fold_c; i++) fold(folds[i]);
    printInput(6, 8 * 5 - 1);
    printf("part 2: %d\n", countVisible()); 
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
