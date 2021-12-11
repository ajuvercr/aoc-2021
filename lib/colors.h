#pragma once

// Color red: \u001b[31m
// 256 color: \u001b[38;5;${ID}m
//
// Backg red: \u001b[41m
// 256 backg: \u001b[48;5;${id}m
//
// move left: \u001b[1000D
// move up  : \u001b[" + str(count) + "A"

#include <stdio.h>

void setColor(int i);
void setBgColor(int i);

void reset_color();

void moveTermLeft();
void moveTermUp(int i);
