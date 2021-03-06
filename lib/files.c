#include "./files.h"
#include "list.h"
#include <stdio.h>


FILE *openFile(const char *filename) {
    return fopen(filename, "r");
}

int readLine(FILE *file, char *buff) {
    return fscanf(file, "%s\n", buff);
}

List *readLines(FILE *file) {
    List *out = newList();
    char *buff = malloc(512);
    while(readLine(file, buff) > 0) {
        listAdd(out, buff);
        buff = malloc(512);
    }

    return out;
}

int readWord(FILE *file, char *buff) {
    int v = fscanf(file, "%s", buff);
    return v;

}

List *readWords(FILE *file) {
    List *out = newList();
    char *buff = malloc(256);
    while(readWord(file, buff) > 0) {
        listAdd(out, buff);
        buff = malloc(256);
    }
    free(buff);

    return out;
}

List *readInts(FILE *file) {
    List *out = newList();
    long x = 0;
    while(fscanf(file, "%ld", &x) > 0) {
        listAdd(out, (int*)x);
    }

    return out;
}
