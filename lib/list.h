#pragma once
#include <stdlib.h>

#ifndef __LIST_START_SIZE
#define __LIST_START_SIZE 16
#endif

typedef struct List {
  int size;
  int allocated;
  void **list;
} List;

List *newList();
void freeList(List *, void (*freeItem)(void *));
int listAdd(List *, void *);
void *listGetWith(List *, void *, int cmpF(void *, void *));
void *listGet(List *, int);
