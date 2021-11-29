#include <stdio.h>
#include <stdlib.h>
#define __LIST_START_SIZE 2
#include "../lib/ebnf_parser.h"

#include "../lib/list.h"
#include "../lib/map.h"

int cmpInt(int *a, int *b) { return *a > *b; }
int eqInt(int *a, int *b) {
  printf("a %d b %d\n", *a, *b);
  return *a == *b;
}
const char *lang = "\"some \\\"lit\" \"bla2\"";
int main() {
  Cursor cursor = {lang, 0};

  Rule *rule = CreateParser(&cursor);
  printf("type %d\n", rule->type);
  printRule(rule, 0);
  FreeRule(rule);

  int a = 1, b = 2, c = 3, d = 4, e = 5;

  List *list = newList();
  listAdd(list, &a);
  listAdd(list, &b);
  listAdd(list, &c);
  printf("list size %d\n", list->size);

  printf("list item %d\n", *(int *)listGet(list, 0));
  printf("list %d > %d\n", b, *(int *)listGetWith(list, &b, (CmpF)cmpInt));

  printf("cmp %p %p %p\n", eqInt, &a, &b);
  Map *map = newMap((CmpF)eqInt);
  printf("added %d\n", mapAdd(map, &a, &b));
  printf("added %d\n", mapAdd(map, &b, &c));

  int x = 42;
  printf("map %d -> %d\n", a, *(int *)mapGet(map, &a));
  return 0;
}
