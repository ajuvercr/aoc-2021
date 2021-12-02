#pragma once
#include <stdio.h>

#include "list.h"

FILE *openFile(const char *filename);

int readLine(FILE *file, char *buff);
List *readLines(FILE *file);

int readWord(FILE *file, char *buff);
List *readWords(FILE *file);
List *readInts(FILE *file);

