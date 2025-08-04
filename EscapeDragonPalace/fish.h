#pragma once
#include "init.h"

#define FISH_WIDTH 15
#define FISH_HEIGHT 3
#define FISH_HP 3

// 전역 변수
char fishGraphic[2][FISH_HEIGHT][FISH_WIDTH] = {
	{
		"  _______   ",
		" /o　))　\\/)",
		" \\_______/\\)"
	}, // 왼쪽 그림 0
	{
		"    _______  ",
		" (\\/　((　o\\",
		" (/\\_______/ "
	},// 오른쪽 그림 1
};

Monster g_Fish = { 5, MONSTER_Y, Right, FISH_HP, 1, MONSTER_FISH, 0, 0 };


// 물고기 함수
void UpdateFish(unsigned int now);
void DrawFish();
void HitFish(unsigned int now, int damage);