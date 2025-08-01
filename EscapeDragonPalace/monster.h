#include "init.h"
#include "Rabbit.h"
#include "weapon.h"

#define MONSTER_H

#define FISH_WIDTH 15
#define FISH_HEIGHT 3
#define FISH_HP 3

#define CRAB_WIDTH 13
#define CRAB_HEIGHT 3
#define CRAB_HP 5

#define INVINCIBLE_TIME 1000  // 1초 무적시간

#define Right 1
#define Left 0

#define MONSTER_Y 21 // 몬스터 Y좌표 초기 위치


typedef enum MonsterType {
    MONSTER_FISH,
    MONSTER_CRAB,
    MONSTER_CLAM,
    MONSTER_TURTLE,
    MONSTER_TYPE_COUNT
} MonsterType;

typedef struct Monster {
    int x, y;           // 위치
    int dir;            // 방향 (1: 오른쪽, -1: 왼쪽)
    int hp;             // 체력
    int alive;          // 생존여부
    MonsterType type;   // 몬스터 종류
    int isDamaged;      // 피격 상태(무적 여부)
    unsigned int lastHitTime;  // 마지막 피격 시간
} Monster;

void UpdateMonster();
// 물고기 함수
void InitFish(int x, int y);
void UpdateFish(unsigned int now);
void DrawFish();
void HitFish(unsigned int now, int damage);

// 꽃게 함수
void InitCrab(int x, int y);
void UpdateCrab(unsigned int now);
void DrawCrab();
void HitCrab(unsigned int now, int damage);
// (조개, 자라 함수도 같은 형식으로 추가)