#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __LIST_START_SIZE 2
#include "../lib/ebnf_parser.h"

#include "../lib/list.h"
#include "../lib/map.h"

int cmpInt(int *a, int *b) { return *a > *b; }
int eqInt(int *a, int *b) {
  printf("a %d b %d\n", *a, *b);
  return *a == *b;
}
Cursor space = { "' ' | '\t' | '\n' ", 0};
Cursor ws = { "(space ws) | '' ", 0};
Cursor digit = {"'0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'", 0};
Cursor number = {"digit number | ''", 0};
Cursor addition = { "number ws '+' ws  number", 0};
Cursor subtraction = {"ws number ws ('-' | '+') ws number", 0};
Cursor term = {"addition | subtraction", 0};

const char *input = "12+ 2";

int invCmpStr(const char *v1, const char *v2) { return !strcmp(v1, v2); }
int main() {
  Map *rules = newMap((CmpF)invCmpStr);
  mapAdd(rules, "ws", CreateParser(&ws));
  mapAdd(rules, "digit", CreateParser(&digit));
  mapAdd(rules, "number", CreateParser(&number));
  mapAdd(rules, "addition", CreateParser(&addition));
  mapAdd(rules, "subtraction", CreateParser(&subtraction));
  mapAdd(rules, "term", CreateParser(&term));

  printRule(mapGet(rules, "ws"), 0);

  Cursor here = {input, 0}; 
  Parset *parset = parse(rules, mapGet(rules, "subtraction"), &here);
  printParset(parset, input);

  char buff[256] = {'\0'};
  Parset *current = parset->child->child;
  printf("%s %p\n", current->symbol, (void*)current->beta);
  while(current->beta) {
    writeMatched(current, buff);
    printf("%s", buff);
    current = current->beta;
  }

  return 0;
}
