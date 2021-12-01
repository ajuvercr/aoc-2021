#include "map.h"
#include "list.h"

Map *newMap(CmpF cmpF) {
  Map *out = malloc(sizeof(Map));
  out->cmpf = cmpF;
  out->list = newList();
  return out;
}

void freeMap(Map *map, void (*freeKey)(key), void (*freeValue)()) {
  for (int i = 0; i < map->list->size; i++) {
    void **item = listGet(map->list, i);
    if (freeKey)
      freeKey(item[0]);
    if (freeValue)
      freeValue(item[1]);
    free(item);
  }
  freeList(map->list, 0);
  free(map);
}

int mapCmp(void **v1, void **v2) { return ((CmpF)v2[2])(v1[0], v2[0]); }

int mapAdd(Map *map, key key, value value) {
  void *total[] = {key, value, (void*) map->cmpf};
  if (!listGetWith(map->list, total, (CmpF)mapCmp)) {
    void **item = malloc(sizeof(void *) * 3);
    item[0] = key;
    item[1] = value;
    item[2] = (void*) map->cmpf;
    listAdd(map->list, item);
    return 1;
  }
  return 0;
}

void *mapGet(Map *map, key key) {
  void *total[3] = {key, 0, (void*) map->cmpf};
  void **item = (void **)listGetWith(map->list, total, (CmpF)mapCmp);
  if (item)
    return item[1];
  return 0;
}

int mapSize(Map *map) { return map->list->size; }
