#pragma once
#include "init.h"

// 매크로
#define STAGE_CNT 5 // 스테이지 갯수
#define BG_CNT 5 // 맵 배경 요소 갯수
#define BG_LINE 20 // 맵 배경 줄 수
#define MAP_HEIGHT 25 // 맵 높이
#define GOAL_HEIGHT 5 // 도착지점 높이

void Draw();
void DrawMap();
void Input();
void Update();
void UpdateMapPos();