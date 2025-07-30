#pragma once

#include "weapon.h"
#include "item.h"
#include "screens.h"

// --------------------------------------------------

bool MapEnd();
bool GetKeyW();
bool GetKeyA();
bool GetKeyD();
bool GetKeyS();
bool IsNearItem();
void SetIsNearItem(bool src);
bool StageClear();
bool IsOverlap();
void CheckItemPickup();
void UpdateSpeedBuffs();
void ClearRabbitAt();
void DrawRabbitAt();
void RabbitCAnim();
void GetInput();
void ApplyGravity();
bool CheckGround();
void JumpFN();
void AttackFN();
void moveFN();
bool ISOnGoal();
void ClearInputBuffer();
void UpdatePlayer();
void DrawPlayer();
void DrawHealth();
void InitPlayer();

// --------------------------------------------------

#define RabbitX 25 // Rabbit char 가로 크기
#define RabbitY 3 // Rabbit char 세로 크기

#define JUMP_POWER -2.5f // 점프 높이 (음수로 갈 수록 점프 높아짐)
#define GRAVITY 0.5f // 중력 가속도
#define MAX_FALL_SPEED 4.0f  // 최대 낙하 속도

#define MAX_BUFFS 10

// --------------------------------------------------

typedef struct Player
{
	MyPoint Pos; // 플레이어 위치
	float Speed; // 플레이어 이동 속도
	int Health; // 플레이어 체력

	Weapon* HeldWeapon; // 무기 (0 = 장검, 1 = 단검, 2 = 창)

	float VelY; // Y축 속도 (점프, 낙하 등)
	bool IsJumping; // 점프 중인지 여부
	int Direction; // 플레이어 방향 (0 = 오른쪽, 1 = 왼쪽)

	bool IsAttacking;      // 공격 중인지 여부
	DWORD attackStartTime; // 공격 시작 시간
	int attackDuration; // 공격 지속 시간 (ms)

	int AttackFrame;       // 공격 애니메이션 프레임 카운터
	int AttackFrameMax;    // 현재 무기에 맞는 공격 애니메이션 프레임 길이
}Player;
Player player;


typedef struct SpeedBuff {
	float amount;       // 증가량
	DWORD endTime;      // 종료 시간 (ms)
	bool active;
} SpeedBuff;


typedef struct Rect {
	float x, y, w, h;
} Rect;

bool IsOverlap(Rect player, Rect Obj);
Rect GetPlayerRect();
Rect GetItemRect(Item item);
void CheckItemPickup();
void UpdateSpeedBuffs();

SpeedBuff speedBuffs[MAX_BUFFS];




