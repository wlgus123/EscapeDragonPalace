#include "init.h"
#include "crab.h"
#include "fish_big.h"
#include "fish_small.h"

// 전역 변수
unsigned long now;

// 몬스터 초기화
void InitMonster()
{
	now = _GetTickCount();
	InitBigFish();		// 큰 물고기 초기화
	InitSmallFish();	// 작은 물고기 초기화
	InitCrab();			// 꽃게 초기화
}

// 몬스터 업데이트
void UpdateMonster()
{
	UpdateBigFish(now);	// 큰 물고기 업데이트
	UpdateSmallFish(now);	// 작은 물고기 업데이트
	UpdateCrab(now);		// 꽃게 업데이트
}

// 몬스터 그리는 함수
void DrawMonster() 
{
	DrawBigFish();		// 큰 물고기 그리기
	DrawSmallFish();	// 작은 물고기 그리기
	DrawCrab();			// 꽃게 그리기
}

// 몬스터 피격 처리 함수
void MonsterHitPlayer()
{
	CrabHitPlayer();
	//HitCrab(now);
	SmallFishHitPlayer();
	BigFishHitPlayer();
}


void PlayerHitMonster() {
	PlayerHitBigFish();
}