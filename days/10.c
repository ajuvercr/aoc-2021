#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char inv(char c) {
  switch (c) {
  case '(':
    return ')';
  case '[':
    return ']';
  case '{':
    return '}';
  case '<':
    return '>';
  }
  return 0;
}
int v(char c) {
  switch (c) {
  case ')':
    return 3;
  case ']':
    return 57;
  case '}':
    return 1197;
  case '>':
    return 25137;
  }
  printf("unexpected %c\n", c);
  return 0;
}
int v2(char c) {
  switch (c) {
  case ')':
    return 1;
  case ']':
    return 2;
  case '}':
    return 3;
  case '>':
    return 4;
  }
  printf("unexpected %c\n", c);
  return 0;
}

int handleLine(const char* line, char *stack) {
    int at = -1;
    int j = 0;
    char c = line[j++];
    int cont = 1;
    while (cont && c != '\0') {
      if (stack[at] == c)
        at--;
      else {
        if (c == '(' || c == '{' || c == '[' || c == '<') {
          stack[++at] = inv(c);
        } else {
            return -1 * v(c);
        }
      }
      c = line[j++];
    }
    stack[at + 1] = 0;
    return at;
}

void part1(const char *input) {
  int output = 0;
  List *list = readLines(openFile(input));
  for (int i = 0; i < list->size; i++) {
    const char *line = listGet(list, i);
    char stack[256];
    int ll = handleLine(line, stack);
    if(ll < 0) output += -1 * ll;
  }
  printf("part 1: %d\n", output);
}

void part2(const char *input) { 
  long o2[256] = {0};
  int complete = 0;
  List *list = readLines(openFile(input));
  for (int i = 0; i < list->size; i++) {
    const char *line = listGet(list, i);
    char stack[256];
    int at = handleLine(line, stack);
    if(at > 0){
      long oi = 0;
      for (char v = stack[at]; at >= 0; v = stack[--at]) {
        oi = oi * 5 + v2(v);
      }

      int i = 0;
      while (i < complete && o2[i] > oi)
        i++;
      if (i == complete)
        o2[i] = oi;
      else {
          for(int j = complete; j >=i ; j --) o2[j] = o2[j-1];
        o2[i] = oi;
      }

      complete += 1;
    }
  }
  printf("part 2: %ld\n", o2[complete / 2]);
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

