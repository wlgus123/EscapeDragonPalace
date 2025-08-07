#include "turtle.h"
#include "monster.h"
#include "map.h"

Monster g_Turtle;
const char* turtleGraphic[2][TURTLE_HEIGHT] = {
    {
        "        ______ ",
        "  ___ _/ \\__/ \\_   /|",
        " (_o / \\ /  \\ / \\_/ |",
        "  \\__ -----------__/",
        "      \\_|_|_|_|_/ ",
        "     /_|_|  /_|_|"
    },
    {
        "       ______",
        "|\\   _/ \\__/ \\_ ___",
        "| \\_/ \\ /  \\ / \\ o_)",
        " \\__----------- __/",
        "    \\_|_|_|_|_/",
        "    |_|_\\  |_|_\\ "
    }
};

static unsigned int randRange(unsigned int min, unsigned int max) {
    return min + rand() % (max - min + 1);
}

void InitTurtle(unsigned int now) {
    srand((unsigned int)time(NULL));
    g_Turtle.type = E_MONSTER_TURTLE;
    g_Turtle.hp = TURTLE_HP;
    g_Turtle.attack = E_TURTLE_ATTACK;
    g_Turtle.dir = Right;
    g_Turtle.alive = true;
    g_Turtle.isDamaged = false;
    g_Turtle.lastHitTime = now;
    // moveNum, speed 등은 외부에서 세팅
}

void UpdateTurtle(unsigned int now) {
    if (!g_Turtle.alive) return;
    if (g_Turtle.isDamaged && now - g_Turtle.lastHitTime >= INVINCIBLE_TIME) {
        g_Turtle.isDamaged = false;
    }
    // 기본 이동
    g_Turtle.pos.x += (g_Turtle.dir == Right ? 1 : -1);
    if (g_Turtle.pos.x < 0) g_Turtle.pos.x = SCREEN_WIDTH;
    if (g_Turtle.pos.x > SCREEN_WIDTH) g_Turtle.pos.x = 0;
    // 스킬 호출: TODO 구현
}

void DrawTurtle() {
    int x = g_Turtle.pos.x - GetPlusX();
    for (int r = 0; r < TURTLE_HEIGHT; ++r) {
        _DrawText(x, g_Turtle.pos.y + r, turtleGraphic[g_Turtle.dir][r]);
    }
}
