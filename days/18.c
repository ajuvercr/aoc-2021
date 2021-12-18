#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Snailfish {
  struct Snailfish *left;
  struct Snailfish *right;
  struct Snailfish *parent;
  int value;
} Snailfish;

#define SIZE 2048 * 16
Snailfish buffer[SIZE];
int buf_idx = 0;
Snailfish *get() {
  if (buf_idx >= SIZE) {
    printf("NOPE\n");
    exit(1);
  }
  Snailfish *out = &buffer[buf_idx++];
  out->value = 0;
  out->left = 0;
  out->right = 0;
  out->parent = 0;
  return out;
}

void print(Snailfish *fish) {
  if (fish->left) {
    printf("[");
    print(fish->left);
    printf(",");
    print(fish->right);
    printf("]");
  } else {
    printf("%d", fish->value);
  }
}

char *parseFish(char *str, Snailfish *fish) {
  if (str[0] == '[') {
    fish->left = get();
    fish->left->parent = fish;
    fish->right = get();
    fish->right->parent = fish;
    fish->value = 0;

    str = parseFish(str + 1, fish->left);
    str = parseFish(str + 1, fish->right);
    return str + 1;
  } else {
    char buf[2] = "";
    buf[0] = str[0];
    fish->right = 0;
    fish->left = 0;
    fish->value = atoi(buf);
    return str + 1;
  }
}

Snailfish *add(Snailfish *one, Snailfish *two) {
  Snailfish *out = get();
  out->left = one;
  out->left->parent = out;
  out->right = two;
  out->right->parent = out;
  out->value = 0;
  return out;
}

typedef struct {
  int left;
  int right;
} Explode;

int explode(Snailfish *fish, int depth) {
  if (!fish->left)
    return 0;

  if (depth >= 4 && !fish->left->left) {
    // explode!
    int left = fish->left->value;
    int right = fish->right->value;
    fish->left = 0;
    fish->right = 0;
    fish->value = 0;

    Snailfish *current = fish;
    while (current && current->parent && current->parent->left == current) {
      current = current->parent;
    }
    if (current && current->parent) {
      current = current->parent->left;
      while (current->right)
        current = current->right;
      current->value += left;
    }

    current = fish;
    while (current && current->parent && current->parent->right == current) {
      current = current->parent;
    }
    if (current && current->parent) {
      current = current->parent->right;
      while (current->left)
        current = current->left;
      current->value += right;
    }

    return 1;
  }

  return explode(fish->left, depth + 1) || explode(fish->right, depth + 1);
}

int split(Snailfish *fish) {
  if (fish->left) {
    return split(fish->left) || split(fish->right);
  }

  if (fish->value > 9) {
    fish->left = get();
    fish->left->parent = fish;
    fish->right = get();
    fish->right->parent = fish;

    int left = fish->value / 2;
    fish->left->value = left;
    fish->right->value = fish->value - left;
    fish->value = 0;
    return 1;
  }
  return 0;
}

void reduce(Snailfish *fish) {
  while (explode(fish, 0)) {
  }

  if (split(fish)) {
    reduce(fish);
  }
}

long magnitude(Snailfish *fish) {
  if (fish->left) {
    return 3 * magnitude(fish->left) + 2 * magnitude(fish->right);
  } else {
    return fish->value;
  }
}

void part1(const char *inputLocation) {
  List *lines = readLines(openFile(inputLocation));
  Snailfish *root = get();
  parseFish(lines->list[0], root);
  for (int i = 1; i < lines->size; i++) {
    Snailfish *this = get();
    parseFish(lines->list[i], this);
    root = add(root, this);
    reduce(root);
  }
  printf("part 1: %ld\n", magnitude(root));
}

void part2(const char *inputLocation) {
  List *lines = readLines(openFile(inputLocation));
  long biggest = 0;
  for (int i = 0; i < lines->size; i++) {
    for (int j = 0; j < lines->size; j++) {
      if (i == j)
        continue;
      buf_idx = 0;

      Snailfish *one = get();
      parseFish(lines->list[i], one);
      Snailfish *other = get();
      parseFish(lines->list[j], other);

      Snailfish *a = add(one, other);
      reduce(a);
      long la = magnitude(a);
      if (la > biggest) {
        biggest = la;
      }
      buf_idx = 0;

      one = get();
      parseFish(lines->list[i], one);
      other = get();
      parseFish(lines->list[j], other);

      Snailfish *b = add(other, one);
      reduce(b);
      long lb = magnitude(b);
      if (lb > biggest) {
        biggest = lb;
      }
    }
  }

  printf("part 2: %ld\n", biggest);
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
