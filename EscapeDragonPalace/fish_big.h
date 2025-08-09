#pragma once
#include "monster.h"

#define BIGFISH_WIDTH 15	// 큰 물고기 너비
#define BIGFISH_HEIGHT 3	// 큰 물고기 높이
#define BIGFISH_HP 4		// 큰 물고기 체력
#define BIGFISH_ATTACK 2	// 큰 물고기 공격력
#define BIGFISH_CNT 17	// 큰 물고기 배열 수

// 전역 변수
static char g_BigFishGraphic[2][BIGFISH_HEIGHT][BIGFISH_WIDTH] = {
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

// 큰 물고기
typedef struct BigFish
{
	Monster mon;		// 몬스터 구조체 중첩
	MyPoint pos;		// 위치
	float startPosX;	// 시작 x 위치
	int attack;			// 공격력
	int moveNum;		// 이동 범위
	Direction dir;		// 몬스터 방향
} BigFish;

// 함수
void InitBigFish();		// 초기화
void DrawBigFish();		// 그리기
void UpdateBigFish(unsigned long now);	// 업데이트
void BigFishHitPlayer();
void PlayerHitBigFish();
void SettingBigFish(bool src);