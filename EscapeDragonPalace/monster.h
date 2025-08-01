#include "init.h"

#ifndef MONSTER_H
#define MONSTER_H

#define FISH_WIDTH 15
#define FISH_HEIGHT 3
#define FISH_HP 3

#define CRAB_WIDTH 11
#define CRAB_HEIGHT 3
#define CRAB_HP 5

#define INVINCIBLE_TIME 1000  // 1초 무적시간

typedef enum {
    MONSTER_FISH,
    MONSTER_CRAB,
    MONSTER_CLAM,
    MONSTER_TURTLE,
    MONSTER_TYPE_COUNT
} MonsterType;

typedef struct {
    int x, y;           // 위치
    int dir;            // 방향 (1: 오른쪽, -1: 왼쪽)
    int hp;             // 체력
    int alive;          // 생존여부
    MonsterType type;   // 몬스터 종류
    int isDamaged;      // 피격 상태(무적 여부)
    unsigned int lastHitTime;  // 마지막 피격 시간
} Monster;

// 물고기 함수
void InitFish(Monster* m, int x, int y);
void UpdateFish(Monster* m, unsigned int now);
void DrawFish(const Monster* m);
void HitFish(Monster* m, unsigned int now, int damage);
/* 꽃게하고 수정할거임 - 진환
// 꽃게 함수
void InitCrab(Monster* m, int x, int y);
void UpdateCrab(Monster* m, unsigned int now, int playerX, int playerY);
void DrawCrab(const Monster* m);
void HitCrab(Monster* m, unsigned int now, int damage);
*/
// (조개, 자라 함수도 같은 형식으로 추가)

#endif