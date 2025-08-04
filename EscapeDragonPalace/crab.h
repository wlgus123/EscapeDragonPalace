#pragma once

#define CRAB_WIDTH 13
#define CRAB_HEIGHT 3
#define CRAB_HP 5

// 전역 변수
const char* crabGraphic[CRAB_HEIGHT] = {
    "(\\/) (\\/)",
    "  \\o_o/ ",
    " =(___)= "
};

// 꽃게 함수
void UpdateCrab(unsigned int now);
void DrawCrab();
void HitCrab(unsigned int now, int damage);