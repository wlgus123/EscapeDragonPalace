#include "init.h"
#include "crab.h"
#include "fish_big.h"
#include "fish_small.h"

// 몬스터 초기화
void InitMonster()
{
	InitBigFish();		// 큰 물고기 초기화
	InitSmallFish();	// 작은 물고기 초기화
	InitCrab();			// 꽃게 초기화
}

// 몬스터 alive false 처리
void ResetMonster()
{
	ResetBigFish();		// 큰 물고기 false처리
	ResetSmallFish();	// 작은 물고기 false처리
	ResetCrab();		// 꽃게 false처리
}

// 몬스터 업데이트
void UpdateMonster()
{
	UpdateBigFish();	// 큰 물고기 업데이트
	UpdateSmallFish();	// 작은 물고기 업데이트
	UpdateCrab();		// 꽃게 업데이트
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
	SmallFishHitPlayer();
	BigFishHitPlayer();
}



void PlayerHitMonster() {
	PlayerHitBigFish();
	PlayerHitSmallFish();
	PlayerHitCrab();
}