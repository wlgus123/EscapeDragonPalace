#pragma once

#define FISH_WIDTH 15
#define FISH_HEIGHT 3
#define FISH_HP 3

// 전역 변수
static char fishGraphic[2][FISH_HEIGHT][FISH_WIDTH] = {
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

// 물고기 함수
void UpdateFish(unsigned int now);
void DrawFish();
void HitFish(unsigned int now, int damage);