#include "../lib/ebnf_parser.h"
#include "../lib/files.h"
#include "../lib/help.h"
#include "../lib/list.h"
#include "../lib/map.h"
#include <stdio.h>
#include <string.h>

void printSq(char *sq) {
  int len = strlen(sq);
  int size = int_sqrt(len);
  char sub[size + 1];
  sub[size] = '\0';
  for (int i = 0; i < len; i += size) {
    memcpy(sub, &sq[i], size);
    printf("%s\n", sub);
  }
}

void flip(char *dest, char *src, int size) {
  for (int i = 0; i < size * size; i++) {
    int row = i / size;
    int col = i % size;
    dest[i] = src[(size - col - 1) + row * size];
  }
  dest[size * size] = '\0';
}

void turn(char *dest, char *src, int size) {
  for (int i = 0; i < size * size; i++) {
    int row = i / size;
    int col = i % size;
    dest[i] = src[(size - row - 1) + col * size];
  }
  dest[size * size] = '\0';
}

void removeSlashes(char *dest, const char *src) {
  int length = strlen(src);
  int offset = 0;
  for (int i = 0; i < length; i++) {
    if (src[i] == '/') {
      offset++;
      continue;
    }
    dest[i - offset] = src[i];
  }
  dest[length - offset] = '\0';
}

typedef struct Permuts {
  char *inner[8];
} Permuts;

Permuts getPermutations(char *a) {
  Permuts out = {{0}};
  out.inner[0] = a;

  int len = strlen(a);
  out.inner[0] = malloc((len + 1) * sizeof(char));
  strcpy(out.inner[0], a);

  out.inner[1] = malloc((len + 1) * sizeof(char));
  out.inner[2] = malloc((len + 1) * sizeof(char));
  out.inner[3] = malloc((len + 1) * sizeof(char));
  out.inner[4] = malloc((len + 1) * sizeof(char));
  out.inner[5] = malloc((len + 1) * sizeof(char));
  out.inner[6] = malloc((len + 1) * sizeof(char));
  out.inner[7] = malloc((len + 1) * sizeof(char));

  int size = int_sqrt(len);
  turn(out.inner[1], out.inner[0], size);
  turn(out.inner[2], out.inner[1], size);
  turn(out.inner[3], out.inner[2], size);
  flip(out.inner[4], out.inner[0], size);
  flip(out.inner[5], out.inner[1], size);
  flip(out.inner[6], out.inner[2], size);
  flip(out.inner[7], out.inner[3], size);

  /*
  printf("turn 1\n");
  printSq(out.inner[0]);
  printf("turn 2\n");
  printSq(out.inner[1]);
  printf("turn 3\n");
  printSq(out.inner[2]);
  printf("turn 4\n");
  printSq(out.inner[3]);
  printf("flip 1\n");
  printSq(out.inner[4]);
  printf("flip 2\n");
  printSq(out.inner[5]);
  printf("flip 3\n");
  printSq(out.inner[6]);
  printf("flip 4\n");
  printSq(out.inner[7]);
*/

  return out;
}

int cmpString(char *a, char *b) { return !strcmp(a, b); }

int main() {
  const char *location = "days/28.test.txt";
  FILE *file = readFile(location);
  List *words = readWords(file);

  Map *map = newMap((CmpF)cmpString);

  for (int i = 3; i < words->size; i += 3) {
    char *key = listGet(words, i);
    char *value = listGet(words, i + 2);
    removeSlashes(key, key);
    removeSlashes(value, value);
    Permuts perms = getPermutations(key);

    for(int i = 0; i < 8; i++)
        mapAdd(map, perms.inner[i], value);
    printSq(mapGet(map, key));
  }

  freeMap(map, free, 0);
  freeList(words, free);
  fclose(file);
  return 0;
}
