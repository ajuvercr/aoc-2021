#include "../lib/files.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Input
{
    long inner[9];
} Input;

Input getInput(FILE *f)
{
    Input out = {{0}};
    char s;
    int l;
    while (fscanf(f, "%d%c", &l, &s) > 0)
    {
        out.inner[l]++;
    }

    return out;
}

void step(long *input)
{
    long extra = input[0];

    for (int i = 1; i < 9; i++)
    {
        input[i - 1] = input[i];
    }
    input[6] += extra;
    input[8] = extra;
}

void part1(const char *inputLocation)
{
    FILE *f = openFile(inputLocation);
    long *input = getInput(f).inner;

    for (int i = 0; i < 80; i++)
        step(input);

    long out = 0;
    for (int i = 0; i < 9; i++)
        out += input[i];

    printf("part 1: %ld\n", out);
}

void part2(const char *inputLocation)
{
    FILE *f = openFile(inputLocation);
    long *input = getInput(f).inner;

    for (int i = 0; i < 256; i++)
        step(input);

    long out = 0;
    for (int i = 0; i < 9; i++)
        out += input[i];

    printf("part 2: %ld\n", out);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please specify input file: %s <inputfile> [part]\n", argv[0]);
        exit(1);
    }

    char *inputfile = argv[1];
    int part = argc > 2 ? atoi(argv[2]) : 0;

    if (part == 0 || part == 1)
    {
        part1(inputfile);
    }

    if (part == 0 || part == 2)
    {
        part2(inputfile);
    }
    return 0;
}
