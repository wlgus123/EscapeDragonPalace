#pragma once
#include "init.h"

// 매크로
#define STAGE_CNT 5 // 스테이지 갯수
#define BG_CNT 5 // 맵 배경 요소 갯수
#define BG_LINE 20 // 맵 배경 줄 수
#define MAP_HEIGHT 25 // 맵 높이
#define GOAL_POS_X 4 // 도착지점 초기위치 X
#define GOAL_POS_Y 7 // 도착지점 초기위치 Y

void Draw();
void DrawMap();
void Input();
void Update();
void UpdateMapPos();