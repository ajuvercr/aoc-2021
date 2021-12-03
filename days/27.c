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
Cursor digit = {"'0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'", 0};
Cursor number = {"digit number | ''", 0};
Cursor addition = {"number '+' number", 0};
Cursor subtraction = {"number '-' number", 0};
Cursor term = {"addition | subtraction", 0};

const char *input = "1 + 2";

int invCmpStr(const char *v1, const char *v2) { return !strcmp(v1, v2); }
int main() {
  Map *rules = newMap((CmpF)invCmpStr);
  mapAdd(rules, "digit", CreateParser(&digit));
  mapAdd(rules, "number", CreateParser(&number));
  mapAdd(rules, "addition", CreateParser(&addition));
  mapAdd(rules, "subtraction", CreateParser(&subtraction));
  mapAdd(rules, "term", CreateParser(&term));

  Cursor here = {input, 0}; 
  printRule(mapGet(rules, "term"), 0);
  Parset *parset = parse(rules, mapGet(rules, "term"), &here);
  printf("HERE\n");
  printParset(parset, input);
  

  return 0;
}
