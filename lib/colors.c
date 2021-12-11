#include "colors.h"
#include <stdio.h>

void reset_color() { printf("\033[0m"); }
void setColor(int i) { printf("\033[38;5;%dm", i); }
void setBgColor(int i) { printf("\033[48;5;%dm", i); }
void moveTermUp(int i) { printf("\033[%dA", i); }
void moveTermLeft() { printf("\033[1000D"); }
