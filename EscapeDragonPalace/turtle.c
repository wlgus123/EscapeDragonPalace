#include "turtle.h"
#include "monster.h"
#include "map.h"

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

void InitTurtle(Monster* t, unsigned int now) {
    srand((unsigned int)time(NULL));
    t->type = E_MONSTER_TURTLE;
    t->hp = TURTLE_HP;
    t->attack = E_TURTLE_ATTACK;
    t->dir = Right;
    t->alive = true;
    t->isDamaged = false;
    t->lastHitTime = now;
    // moveNum, speed 등은 외부에서 세팅
}

void UpdateTurtle(Monster* t, unsigned int now) {
    if (!t->alive) return;
    if (t->isDamaged && now - t->lastHitTime >= INVINCIBLE_TIME) {
        t->isDamaged = false;
    }
    // 기본 이동
    t->pos.x += (t->dir == Right ? 1 : -1);
    if (t->pos.x < 0) t->pos.x = SCREEN_WIDTH;
    if (t->pos.x > SCREEN_WIDTH) t->pos.x = 0;
    // 스킬 호출: TODO 구현
}

void DrawTurtle(Monster* t) {
    int x = t->pos.x - GetPlusX();
    for (int r = 0; r < TURTLE_HEIGHT; ++r) {
        _DrawText(x, t->pos.y + r, turtleGraphic[t->dir][r]);
    }
}
