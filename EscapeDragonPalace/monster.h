#pragma once
#include "weapon.h"
#include "map.h"
#include "clam.h"
#include "crab.h"
#include "fish.h"
#include "small_fish.h"
#include "turtle.h"

#define MONSTER_H

#define INVINCIBLE_TIME 1000  // 1초 무적시간

#define Right 0
#define Left 1

#define MONSTER_Y 21 // 몬스터 Y좌표 초기 위치

#define MAX_Monster_COUNT 100


typedef enum MonsterType 
{
    E_MONSTER_FISH,
    E_MONSTER_CRAB,
    E_MONSTER_CLAM,
    E_MONSTER_TURTLE,
    E_MONSTER_SMALLFISH,
    E_MONSTER_TYPE_COUNT
} MonsterType;

// 몬스터 공격력 열거형
typedef enum MonsterAttack
{
    E_FISH_ATTACK = 2,          // 물고기: 한 칸
    E_SMALL_FISH_ATTACK = 1,    // 작은 물고기: 반 칸
    E_CRAB_ATTACK = 1,          // 꽃게: 반 칸 씩 3번
    E_CLAM_ATTACK = 1,          // 조개: 반 칸
    E_TURTLE_ATTACK = 2,        // 자라 일반 스킬: 한 칸
    E_TURTLE_RUSH = 4,          // 자라 돌진 스킬: 두 칸
    E_TURTLE_WATTER_CANNON = 1, // 자라 물대포 스킬: 반 칸
} MonsterAttack;

typedef struct Monster {
    MyPoint pos;        // 위치
    int moveNum;        // 이동 칸 수
    int dir;            // 방향 
    int hp;             // 체력
    float speed;        // 이동 속도
    int attack;         // 공격력
    bool alive;          // 생존여부
    MonsterType type;   // 몬스터 종류
    bool isDamaged;      // 피격 상태(무적 여부)
    unsigned int lastHitTime;  // 마지막 피격 시간
    MapStatus mapStatus; // 맵 스폰할때 사용
} Monster;

extern Monster monsterList[MAX_Monster_COUNT];
extern int numMonster;

void UpdateMonster();
void HitMonster(Monster* monster, Weapon* weapon, unsigned int now);
void InitMonster();
void DrawMonster();

// (자라 함수도 같은 형식으로 추가)