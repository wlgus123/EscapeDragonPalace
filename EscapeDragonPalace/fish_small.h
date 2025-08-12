#pragma once
#include "monster.h"

#define SMALLFISH_WIDTH 8	// 작은 물고기 너비
#define SMALLFISH_HP 1		// 작은 물고기 체력
#define SMALLFISH_ATTACK 1	// 작은 물고기 공격력 (반 칸)
#define SMALLFISH_CNT 8	// 작은 물고기 배열 수

// 전역 변수
static char g_SmallFishGraphic[SMALLFISH_WIDTH] = "<')))><";

// 작은 물고기
typedef struct SmallFish
{
	Monster mon;		// 몬스터 구조체 중첩
	MyPoint pos;		// 위치
	int attack;		    // 공격력
	bool isRush;
} SmallFish;

// 함수
void InitSmallFish();	// 초기화
void DrawSmallFish();	// 그리기
void UpdateSmallFish();	// 업데이트
void SmallFishHitPlayer();	// 플레이어 공격
void PlayerHitSmallFish();	// 작은 물고기 피격 처리
void ResetSmallFish();
