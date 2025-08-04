#include "weapon.h"
#include "clam.h"
#include "crab.h"
#include "fish.h"
#include "map.h"

#define MONSTER_H

#define INVINCIBLE_TIME 1000  // 1초 무적시간

#define Right 1
#define Left -1

#define MONSTER_Y 21 // 몬스터 Y좌표 초기 위치


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
    int isDamaged;      // 피격 상태(무적 여부)
    unsigned int lastHitTime;  // 마지막 피격 시간
    MapStatus monsterStatus; // 맵 스폰할때 사용
} Monster;

void UpdateMonster();

// (자라 함수도 같은 형식으로 추가)