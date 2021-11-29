#pragma once

#include "list.h"

typedef void *key;
typedef void *value;
typedef int (*CmpF)(void *, void*);
typedef struct Map {
    CmpF cmpf;
    List *list;
} Map;

Map *newMap(CmpF);
void freeMap(Map*, void (*freeKey)(key), void (*freeValue)(value));
int mapAdd(Map *, key, value);
void *mapGet(Map *, key);
int mapSize(Map *);
