#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  long one;
  long two;
} Outcome;

Outcome parseInput(const char *loc) {
  char buff[256];
  FILE *f = openFile(loc);
  Outcome out;

  fscanf(f, "%s %s %s %s %ld", buff, buff, buff, buff, &out.one);
  fscanf(f, "%s %s %s %s %ld", buff, buff, buff, buff, &out.two);

  fclose(f);
  return out;
}

long handleDice(int *d) {
  long dice = *d;
  long add;
  if (dice < 97) {
    add = 3 * dice + 3;
    dice += 3;
  } else {
    add = dice++;
    if (dice >= 100)
      dice = 0;
    add += dice++;
    if (dice >= 100)
      dice = 0;
    add += dice++;
    if (dice >= 100)
      dice = 0;
  }
  *d = dice;
  return add;
}

long game(long p1, long p2) {
  int dice = 0;
  int i = 0;
  long sp1 = 0;
  long sp2 = 0;
  p1--;
  p2--;
  while (1) {
    i++;
    long add = handleDice(&dice);
    p1 = (p1 + add + 3) % 10;
    sp1 += p1 + 1;

    if (sp1 >= 1000)
      return 3 * i * sp2;
    i++;

    add = handleDice(&dice);
    p2 = (p2 + add + 3) % 10;
    sp2 += p2 + 1;
    if (sp2 >= 1000)
      return 3 * i * sp1;
  }
}

void part1(const char *inputLocation) {
  Outcome players = parseInput(inputLocation);
  printf("part 1: %ld\n", game((int)players.one, (int)players.two));
}

int done[11][21][11][21] = {{{{0}}}};
Outcome outcomes[11][21][11][21] = {{{{{-1}}}}};

Outcome game2(int p1, int sp1, int p2, int sp2) {
  if (done[p1][sp1][p2][sp2])
    return outcomes[p1][sp1][p2][sp2];

  Outcome here = {0};

  for (int i1 = 1; i1 <= 3; i1++) {
    for (int i2 = 1; i2 <= 3; i2++) {
      for (int i3 = 1; i3 <= 3; i3++) {

        int _p1 = p1 + i1 + i2 + i3;
        while (_p1 > 10)
          _p1 -= 10;
        int _sp1 = sp1 + _p1;

        if (_sp1 >= 21) {
          here.one += 1;
          continue;
        }

        for (int j1 = 1; j1 <= 3; j1++) {
          for (int j2 = 1; j2 <= 3; j2++) {
            for (int j3 = 1; j3 <= 3; j3++) {

              int _p2 = p2 + j1 + j2 + j3;
              while (_p2 > 10)
                _p2 -= 10;
              int _sp2 = sp2 + _p2;

              if (_sp2 >= 21) {
                here.two += 1;
                continue;
              }

              Outcome cont = game2(_p1, _sp1, _p2, _sp2);
              here.one += cont.one;
              here.two += cont.two;
            }
          }
        }
      }
    }
  }

  outcomes[p1][sp1][p2][sp2] = here;
  done[p1][sp1][p2][sp2] = 1;
  return here;
}

void part2(const char *inputLocation) {
  Outcome players = parseInput(inputLocation);
  Outcome o = game2((int)players.one, 0, (int)players.two, 0);
  printf("part 2: %ld\n", o.one > o.two ? o.one : o.two);
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
