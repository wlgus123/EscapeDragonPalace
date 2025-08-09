#pragma once

#include "init.h"
#include "weapon.h"
#include "item.h"
#include "monster.h"

// --------------------------------------------------

#define RabbitX 25 // Rabbit char 가로 크기
#define RabbitY 3 // Rabbit char 세로 크기
#define RABBIT_SPEED 1	// Rabbit 이동 속도

#define JUMP_POWER -2.5f // 점프 높이 (음수로 갈 수록 점프 높아짐)
#define GRAVITY 0.5f // 중력 가속도
#define MAX_FALL_SPEED 4.0f  // 최대 낙하 속도

#define RabbitXPos 7.0f
#define RabbitYPos 21.0f

#define DURATION 5000

#define SLOWDURATION 3000

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

	unsigned int lastHitTime; // 마지막 피격 시간
}Player;
Player player;


typedef struct SpeedBuff {
	DWORD endTime;      // 종료 시간 (ms)
	bool active;
} SpeedBuff;

// --------------------------------------------------

bool SetMapEnd(bool src);
bool GetKeyW();
bool GetKeyA();
bool GetKeyD();
bool GetKeyS();
bool IsNearItem();
void SetIsNearItem(bool src);
bool StageClear();
bool IsOverlap(Rect player, Rect Obj);
Rect GetPlayerRect();
Rect GetItemRect(Item item);
Rect GetMonsterRect(Monster monster);
Rect GetWeaponRect();
void HitPlayer();
void CheckItemPickup();
void UpdateBuffs();
void ClearRabbitAt(int x, int y);
void DrawRabbitAt(int x, int y, int idx);
void RabbitCAnim();
void GetInput();
bool CheckGround();
void ApplyGravity();
int GetGroundY();
void JumpFN();
void AttackFN();
void moveFN();
void ISOnGoal();
void ClearInputBuffer();
void UpdatePlayer();
void DrawPlayer();
void DrawHealth();
void InitPlayer();
void ClimbLadder();
void DrawBuffNDebuff();





