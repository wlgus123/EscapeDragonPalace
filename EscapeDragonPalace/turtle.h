#pragma once

#define TURTLE_HEIGHT 6
#define TURTLE_WIDTH 24
#define TURTLE_HP 30
#define MAX_WATER_DROPS 10

typedef struct {
    int x, y;
    int dir;
    int hp;
    bool isAttacking;
    unsigned int attackStartTime;
    unsigned int lastAttackTime;
} Turtle;

typedef struct {
    int x, y;
    bool active;
} WaterDrop;

WaterDrop g_WaterDrops[MAX_WATER_DROPS];

static const char* turtleGraphic[2][TURTLE_HEIGHT]= {
    {
    "        ______ ",
    "  ___ _/ \\__/ \\_   /|",
    " (_o / \\ /  \\ / \\_/ |",
    "  \\__ -----------__/",
    "      \\_|_|_|_|_/ ",
    "     /_|_|  /_|_|"
    },
    {
    "       ______"
    "|\\   _/ \\__/ \\_ ___"
    "| \\_/ \\ /  \\ / \\ o_)"
    " \\__----------- __/"
    "    \\_|_|_|_|_/"
    "    |_|_\\  |_|_\\ "
    }
};

void DrawTurtle(int dir, int posX, int posY);
void UpdateTurtleWaterCannon(unsigned int now);
void DrawWaterDrops();