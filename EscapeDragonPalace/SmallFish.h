#pragma once

#define SMALLFISH_WIDTH 8
#define SMALLFISH_HEIGHT 1
#define SMALLFISH_HP 2

// 전역 변수
static char g_SmallFishGraphic[2][SMALLFISH_HEIGHT][SMALLFISH_WIDTH] = {
	{
		"><((('>"
	},// 오른쪽 그림 0
	{
		"<')))><"
	}, // 왼쪽 그림 1
};

// 물고기 함수
void DrawSmallFish(int dir, MyPoint pos);	// 물고기 그리기
bool GetSmallFishAlive();	// 물고기 생존여부 반환
bool GetSmallFishIsDamaged();	// 물고기 데미지 여부 반환
MyPoint GetSmallFishPos(); // 물고기 위치 반환
int GetSmallFishDir();	// 물고기 방향 반환