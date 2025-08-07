#pragma once

#define TURTLE_HEIGHT 6
#define TURTLE_WIDTH 23
#define TURTLE_HP 80

extern const char* turtleGraphic[2][TURTLE_HEIGHT];

// ========== 자라 보스 API ==========
// monsterList에 추가 예시:
// Monster turtleBoss = { .pos = {startX, startY}, .moveNum = moveSteps, .type = E_MONSTER_TURTLE, .hp = TURTLE_HP, .speed = defaultSpeed, .attack = E_TURTLE_ATTACK, .alive = true, .mapStatus = currentMap };
// InitTurtle(&turtleBoss, now);

void InitTurtle(unsigned int now);
void UpdateTurtle(unsigned int now);
void DrawTurtle();
