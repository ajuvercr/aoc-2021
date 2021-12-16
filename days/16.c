#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Packet {
  int version;
  int type;
  unsigned long value;
  struct Packet *children;
  int child_count;
} Packet;

#define SP(x, v)                                                               \
  case (x):                                                                    \
    strcpy(t, v);                                                              \
    break;

unsigned long binToInt(const char *bin, int size) {
  unsigned long out = 0;

  for (int i = 0; i < size; i++) {
    out <<= 1;
    out |= bin[i] == '1';
  }

  return out;
}

char *spread(const char *inp) {
  char *out = malloc(sizeof(char) * (strlen(inp) * 4 + 1));

  for (int i = 0; i < strlen(inp); i++) {
    char *t = out + i * 4;
    switch (inp[i]) {
      SP('0', "0000")
      SP('1', "0001")
      SP('2', "0010")
      SP('3', "0011")
      SP('4', "0100")
      SP('5', "0101")
      SP('6', "0110")
      SP('7', "0111")
      SP('8', "1000")
      SP('9', "1001")
      SP('A', "1010")
      SP('B', "1011")
      SP('C', "1100")
      SP('D', "1101")
      SP('E', "1110")
      SP('F', "1111")
    }
  }
  out[strlen(inp) * 4] = '\0';

  return out;
}

int parsePacket(const char *bin, Packet *packet);
int parseHeader(const char *bin, Packet *packet) {
  packet->version = binToInt(bin, 3);
  packet->type = binToInt(bin + 3, 3);
  return 6;
}

int parseLiteral(const char *bin, Packet *packet) {
  char buff[1024];
  int parts = 0;
  char cont = 1;
  while (cont) {
    for (int i = 0; i < 4; i++)
      buff[parts * 4 + i] = bin[parts * 5 + i + 1];
    cont = bin[parts * 5] == '1';
    parts += 1;
  }
  packet->value = binToInt(buff, parts * 4);
  return parts * 5;
}

int parseID0Operator(const char *bin, Packet *packet, int bits) {
  packet->children = malloc(sizeof(Packet) * 16);
  int l = 0;
  int c = 0;
  while (l < bits)
    l += parsePacket(bin + l, &packet->children[c++]);
  packet->child_count = c;
  return bits;
}

int parseId1Operator(const char *bin, Packet *packet, int count) {
  packet->children = malloc(sizeof(Packet) * count);
  int l = 0;
  for (int i = 0; i < count; i++)
    l += parsePacket(bin + l, &packet->children[i]);

  packet->child_count = count;
  return l;
}

int parseOperator(const char *bin, Packet *packet) {
  if (bin[0] == '0') {
    int c = binToInt(bin + 1, 15);
    return 16 + parseID0Operator(bin + 16, packet, c);
  } else {
    int c = binToInt(bin + 1, 11);
    return 12 + parseId1Operator(bin + 12, packet, c);
  }
}

int parsePacket(const char *bin, Packet *packet) {
  int l = 0;
  l += parseHeader(bin + l, packet);
  switch (packet->type) {
  case 4:
    l += parseLiteral(bin + l, packet);
    break;
  default:
    l += parseOperator(bin + l, packet);
    break;
  }
  return l;
}

int countVersion(Packet *packet) {
  int out = packet->version;
  for (int i = 0; i < packet->child_count; i++) {
    out += countVersion(&packet->children[i]);
  }

  return out;
}

void parseInput(const char *inputLocation, Packet *p) {
  FILE *f = openFile(inputLocation);
  char buff[2048];
  int i = fread(buff, 1, 2048, f);
  buff[i - 1] = '\0';
  char *t = spread(buff);
  // char *t = spread("CE00C43D881120");

  parsePacket(t, p);
  free(t);
}

unsigned long interpret(Packet p) {
  switch (p.type) {
  case 0: {
    unsigned long out = 0;
    for (int i = 0; i < p.child_count; i++)
      out += interpret(p.children[i]);
    return out;
  } break;
  case 1: {
    unsigned long out = 1;
    for (int i = 0; i < p.child_count; i++)
      out *= interpret(p.children[i]);
    return out;
  } break;
  case 2: {
    unsigned long out = ~0;
    for (int i = 0; i < p.child_count; i++) {
      unsigned long v = interpret(p.children[i]);
      if (v < out)
        out = v;
    }
    return out;
  }
  case 3: {
    unsigned long out = 0;
    for (int i = 0; i < p.child_count; i++) {
      unsigned long v = interpret(p.children[i]);
      if (v > out)
        out = v;
    }
    return out;
  }
case 4:
  return p.value;
case 5:
  return interpret(p.children[0]) > interpret(p.children[1]);
case 6:
  return interpret(p.children[0]) < interpret(p.children[1]);
case 7:
  return interpret(p.children[0]) == interpret(p.children[1]);
default:
  printf("unkown type %d\n", p.type);
}

return 0;
}

void part1(const char *inputLocation) {
  Packet p = {0};
  parseInput(inputLocation, &p);
  printf("part 1: %d\n", countVersion(&p));
}

void testAll();
void part2(const char *inputLocation) {
  Packet p = {0};
  parseInput(inputLocation, &p);
  printf("part 2: %ld\n", interpret(p));
}

void test(char *inp, unsigned long v) {
  Packet p = {0};
  char *t = spread(inp);
  parsePacket(t, &p);
  printf("%ld == %ld\n", interpret(p), v);
}

void testAll() {
  test("C200B40A82", 3);
  test("04005AC33890", 54);
  test("880086C3E88112", 7);
  test("CE00C43D881120", 9);
  test("D8005AC2A8F0", 1);
  test("F600BC2D8F", 0);
  test("9C005AC2F8F0", 0);
  test("9C0141080250320F1802104A08", 1);
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
