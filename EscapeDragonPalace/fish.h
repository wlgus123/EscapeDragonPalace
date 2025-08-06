#pragma once

#define FISH_WIDTH 15
#define FISH_HEIGHT 3
#define FISH_HP 3

// 전역 변수
static char g_FishGraphic[2][FISH_HEIGHT][FISH_WIDTH] = {
	{
		"    _______  ",
		" (\\/　((　o\\",
		" (/\\_______/ "
	},// 오른쪽 그림 0
		{
		"  _______   ",
		" /o　))　\\/)",
		" \\_______/\\)"
	}, // 왼쪽 그림 1
};

// 물고기 함수
void UpdateFish(unsigned int now);	// 물고기 업데이트
void DrawFish(int dir, int posX, int posY);	// 물고기 그리기
//void HitFish(unsigned int now, int damage);	// 물고기 데미지 입히기