#include "list.h"
#include <stdlib.h>

List *newList() {
  List *out = malloc(sizeof(List));
  out->size = 0;
  out->allocated = __LIST_START_SIZE;
  out->list = calloc(out->allocated, sizeof(void *));
  return out;
}

void freeList(List *list, void (*freeItem)(void *)) {
  if (freeItem) {
    for (int i = 0; i < list->size; i++) {
      freeItem(list->list[i]);
    }
  }
  free(list->list);
  free(list);
}

int listAdd(List *list, void *item) {
  if (list->size+1 == list->allocated) {
    list->allocated += __LIST_START_SIZE;
    list->list = realloc(list->list, sizeof(void*) * list->allocated);
  }
  list->list[list->size] = item;
  return list->size++;
}

typedef int (*CmpF)(void*, void*);
void *listGetWith(List *list, void *item, CmpF cmpF) {
   for(int i = 0; i < list->size; i++) {
        if(cmpF(item, list->list[i])) return list->list[i];
   }
   return 0;
}

void *listGet(List *list, int index) {
    return list->list[index];
}

