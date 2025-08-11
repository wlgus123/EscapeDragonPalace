#pragma once
#include "monster.h"

// 메크로
#define CRAB_WIDTH 13   // 꽃게 너비
#define CRAB_HEIGHT 3   // 꽃게 높이
#define CRAB_HP 5		// 꽃게 체력
#define CRAB_ATTACK 1   // 꽃게 공격력
#define CRAB_CNT 17     // 꽃게 배열 수
#define TILE_SIZE 1 //바닥 크기
#define CRAB_EYE 15 //꽃게 인식 범위

// 전역 변수
const static char g_CrabGraphic[2][CRAB_HEIGHT][CRAB_WIDTH] = {
    {
        "(\\/) (\\/)",
         "  \\o_o/ ",
         " =(___)= "
    }, // 기본

    {
        "(|)   (|)",
         "  \\o_o/ ",
         " =(___)= "
    } // 공격
};


// 꽃게 구조체
typedef struct Crab
{
    Monster mon;		// 몬스터 구조체 중첩
    Skill skill;		// 스킬 구조체 중첩
    MyPoint pos;		// 위치
    float startPosX;	// 시작 x 위치
    int moveNum;		// 이동 범위
    Direction dir;    // 몬스터 방향
	bool state;  // 꽃게 상태 (추격 여부)
} Crab;


// 함수
void InitCrab();    // 초기화
void DrawCrab();    // 그리기
void UpdateCrab(unsigned long now);  // 업데이트
void PlayerHitCrab(); // 플레이어 공격 처리
void CrabHitPlayer(); // 플레이어 공격 처리
void BleedPlayer(); // 플레이어 출혈 처리
void ResetCrab();   // 꽃게 정보 초기화