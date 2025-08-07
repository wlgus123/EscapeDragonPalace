#pragma once
#include "monster.h"

#define TURTLE_HEIGHT 6
#define TURTLE_WIDTH 24
#define TURTLE_HP 30

extern const char* turtleGraphic[2][TURTLE_HEIGHT];

void UpdateTurtle(Monster* turtle, unsigned int now);
MyPoint GetTurtlePos();
int GetTurtleDir();
void DrawTurtle();