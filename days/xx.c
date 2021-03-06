#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1(const char *inputLocation) {
    printf("part 1: ");
}

void part2(const char *inputLocation) {
    printf("part 2: ");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please specify input file: %s <inputfile> [part]\n", argv[0]);
        exit(1);
    }

    char *inputfile = argv[1];
    int part = argc > 2 ? atoi(argv[2]) : 0;

    if(part == 0 || part == 1) {
        part1(inputfile);
    }

    if(part == 0 || part == 2) {
        part2(inputfile);
    } 
    return 0;
}
