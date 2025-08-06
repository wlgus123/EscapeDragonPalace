#include "weapon.h"
#include "map.h"
#include "clam.h"
#include "crab.h"
#include "fish.h"

#define MONSTER_H

#define INVINCIBLE_TIME 1000  // 1초 무적시간

#define Right 0
#define Left 1

#define MONSTER_Y 21 // 몬스터 Y좌표 초기 위치

#define MAX_Monster_COUNT 100


typedef enum MonsterType {
    MONSTER_FISH,
    MONSTER_CRAB,
    MONSTER_CLAM,
    MONSTER_TURTLE,
    MONSTER_TYPE_COUNT
} MonsterType;

typedef struct Monster {
    MyPoint pos;        // 위치
    int dir;            // 방향 
    int hp;             // 체력
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