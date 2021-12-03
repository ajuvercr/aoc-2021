#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int toDec(const char *input) {
  int out = 0;

  for (int i = 0; i < strlen(input); i++) {
    out = out << 1;
    if (input[i] == '1')
      out = out | 1;
  }

  return out;
}

void part1(const char *inputLocation) {
  FILE *file = openFile(inputLocation);
  char buff[256];
  fscanf(file, "%s\n", buff);

  int l = strlen(buff);
  int counts[2][l];
  for (int j = 0; j < l; j++) {
    counts[0][j] = 0;
    counts[1][j] = 0;
  }
  do {
    for (int j = 0; j < l; j++) {
      if (buff[j] == '0')
        counts[0][j]++;
      else
        counts[1][j]++;
    }
  } while (fscanf(file, "%s\n", buff) > 0);

  char gamma[l + 1];
  char beta[l + 1];
  gamma[l] = '\0';
  beta[l] = '\0';
  for (int j = 0; j < l; j++) {
    if (counts[0][j] > counts[1][j]) {
      gamma[j] = '1';
      beta[j] = '0';
    } else {
      gamma[j] = '0';
      beta[j] = '1';
    }
  }

  printf("part 1: %d\n", toDec(gamma) * toDec(beta));
  fclose(file);
}

void part2(const char *inputLocation) {
  List *numbers = readWords(openFile(inputLocation));
  int l = strlen(numbers->list[0]);
  int s = numbers->size;
  char **words = (char **)numbers->list;
  char oxygen[256] = "";
  char co2[256] = "";

  for (int j = 0; j < l; j++) {
    int o_count[2] = {0};
    int c_count[2] = {0};

    for (int i = 0; i < s; i++) {
      int o_good = 1;
      int c_good = 1;
      for (int k = 0; k < j; k++) {
        o_good = o_good & (oxygen[k] == words[i][k]);
        c_good = c_good & (co2[k] == words[i][k]);
      }
      if (o_good) {
        if (words[i][j] == '0')
          o_count[0]++;
        else
          o_count[1]++;
      }
      if (c_good) {
        if (words[i][j] == '0')
          c_count[0]++;
        else
          c_count[1]++;
      }
    }

    oxygen[j] = o_count[0] > o_count[1] ? '0' : '1';

    if (c_count[0] + c_count[1] == 1)
      co2[j] = c_count[0] > c_count[1] ? '0' : '1';
    else
      co2[j] = c_count[1] < c_count[0] ? '1' : '0';
  }

  printf("part 2: %d\n", toDec(oxygen) * toDec(co2));
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
