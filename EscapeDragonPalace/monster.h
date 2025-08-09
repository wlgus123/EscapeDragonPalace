#pragma once
#include "init.h"
#include "weapon.h"
#include "map.h"

// 메크로
#define MONSTER_CNT 3	// 몬스더 종류 수
#define INVINCIBLE_TIME 10000  // 무적시간 (1초)

// 몬스터 구조체
typedef struct Monster
{
	bool alive;					// 생존 여부
	int hp;						// 몬스터 체력
	float speed;				// 이동 속도
	bool isDamaged;				// 피격 상태 (무적 여부)
	unsigned long lastHitTime;	// 마지막 피격 시간
} Monster;

// 스킬 구조체
typedef struct Skill
{
	bool isAttack;		// 스킬 사용 중인지 여부
	int attackCnt;		// 스킬 구현 횟수
	int attack;			// 스킬 공격력
	long coolTime;		// 스킬 쿨타임 (ms)
} Skill;

// 몬스터 타입 열거형
typedef enum MonsterType
{
	E_MONSTER_FISH,
	E_MONSTER_CRAB,
	E_MONSTER_CLAM,
	E_MONSTER_TURTLE,
	E_MONSTER_SMALLFISH,
	E_MONSTER_TYPE_COUNT
} MonsterType;

// 함수
void InitMonster();		// 초기화
void UpdateMonster();	// 업데이트
void DrawMonster();		// 그리기
void MonsterHitPlayer(); 	// 피격 처리
void PlayerHitMonster();	// 플레이어 공격 처리