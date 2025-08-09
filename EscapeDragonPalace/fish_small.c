#include "init.h"
#include "fish_small.h"
#include "Rabbit.h"

Monster g_SmallFishMon;	// 작은 물고기 몬스터 구조체 공통 설정
SmallFish g_SmallFishList[STAGE_CNT][SMALLFISH_CNT];	// 작은 물고기 포인트 배열
int g_SmallFishListIdx[STAGE_CNT] = { 0, };

// 작은 물고기 업데이트
void UpdateSmallFish(unsigned long now)
{
	// 현재 맵의 몬스터 데이터 불러오기
	SmallFish* tempSmallFish = g_SmallFishList[GetMapStatus()];
	for (int idx = 0; idx < g_SmallFishListIdx[GetMapStatus()]; idx++)
	{
		// 몬스터가 죽었을 경우 넘어가기
		if (!tempSmallFish[idx].mon.alive) continue;

		// 몬스터가 화면 안에 있을 경우
		if (tempSmallFish[idx].pos.x - GetPlusX() > 0 && tempSmallFish[idx].pos.x - GetPlusX() < SCREEN_WIDTH) {
			// 플레이어와 몬스터의 Y 좌표가 같을 때
			if (tempSmallFish[idx].pos.y == player.Pos.y + 2) {
				if(tempSmallFish[idx].pos.x - GetPlusX() + 8 >= player.Pos.x + 12) {
					tempSmallFish[idx].isRush = true;	// 돌진 상태로 변경
				}
			}

		}
		if (tempSmallFish[idx].isRush) {
			// 몬스터 이동
			tempSmallFish[idx].pos.x -= g_SmallFishMon.speed;

			// 몬스터가 왼쪽 화면 밖으로 넘어갔을 경우 죽음 처리
			if (tempSmallFish[idx].pos.x <= 0)
			{
				tempSmallFish[idx].mon.alive = false;
				tempSmallFish[idx].isRush = false;	// 돌진 상태 해제	
			}
		}
	}
}

// 작은 물고기 그리기
void DrawSmallFish()
{
	_SetColor(E_Teal);	// 항상 파란색

	// 현재 맵 데이터 임시로 불러오기
	SmallFish* tempSmallFish = g_SmallFishList[GetMapStatus()];
	for (int idx = 0; idx < g_SmallFishListIdx[GetMapStatus()]; idx++)
	{
		// 몬스터가 죽었을 경우 넘어가기
		if (!tempSmallFish[idx].mon.alive) continue;

		int tempX = tempSmallFish[idx].pos.x - GetPlusX();
		for (int x = 0; x < SMALLFISH_WIDTH; x++)
		{
			if (g_SmallFishGraphic[x] != ' ')
			{
				// 화면 범위 내에 있을 경우 그리기
				if (0 <= tempX + x && SCREEN_WIDTH > tempX + x)
				{
					_DrawText(tempX + x, tempSmallFish[idx].pos.y, (char[]) { g_SmallFishGraphic[x], 0 });
				}
			}
		}
	}
}

// 플레이어 > 작은 물고기 공격하는 함수
void PlayerHitSmallFish()
{
	SmallFish* smallFishList = g_SmallFishList[GetMapStatus()];
	int fishCount = g_SmallFishListIdx[GetMapStatus()];
	DWORD now = GetTickCount();
	Rect PlayerWeaponPos = GetWeaponRect();

	for (int idx = 0; idx < fishCount; idx++)
	{
		SmallFish* pFish = &smallFishList[idx];
		int posX = pFish->pos.x - GetPlusX();
		int posY = pFish->pos.y;
		Rect MonsterPos = { posX, posY, 7, 1 };

		if (player.IsAttacking)
		{
			if (!pFish->mon.alive) continue;

			if (!IsOverlap(PlayerWeaponPos, MonsterPos))
				continue;

			if (now - player.lastHitTime < MONSTER_INVINCIBLE_TIME)
				continue;

			pFish->mon.hp -= player.HeldWeapon->attack;	// 몬스터 체력 감소
			pFish->mon.isDamaged = true;	// 피격 상태로 변경
			player.lastHitTime = now;

			if (pFish->mon.hp <= 0) {
				pFish->mon.alive = false;	// 체력이 0 이하가 되면 사망 처리
			}
		}
	}
}

// 작은 물고기 > 플레이어 공격하는 함수
void SmallFishHitPlayer()
{
	SmallFish* smallFishList = g_SmallFishList[GetMapStatus()];
	int fishCount = g_SmallFishListIdx[GetMapStatus()];
	DWORD now = GetTickCount();

	Rect PlayerPos = { player.Pos.x + 8 + GetPlusX(), player.Pos.y, 5, 3 };

	for (int idx = 0; idx < fishCount; idx++)
	{
		SmallFish* pFish = &smallFishList[idx];
		int posX = pFish->pos.x;
		int posY = pFish->pos.y;
		Rect MonsterPos = { posX, posY, 6, 1 };

		if (!pFish->mon.alive) continue;

		if (!IsOverlap(PlayerPos, MonsterPos))
			continue;

		if (now - pFish->mon.lastHitTime < INVINCIBLE_TIME)
			continue;

		SetInvincibleTime(true);	// 플레이어 무적 시간 설정
		player.Health -= pFish->attack;
		pFish->mon.lastHitTime = now;
	}
}

void ResetSmallFish() {
	for (int i = 0; i < STAGE_CNT; i++)
	{
		SmallFish* tempSmallFish = g_SmallFishList[i];
		for (int idx = 0; idx < g_SmallFishListIdx[i]; idx++)
		{
			tempSmallFish[idx].mon.alive = false;
		}
	}
}


void InitSmallFish()
{
	// 작은 물고기 공통된 속성 설정
	g_SmallFishMon = (Monster)
	{
		.alive = true,		// 생존 여부
		.hp = 1,			// 체력
		.isDamaged = false,	// 피격 상태 (무적 여부)
		.lastHitTime = 0,	// 마지막 피격 시간
		.speed = 1.6,		// 이동 속도
	};

	// 감옥
	g_SmallFishList[E_Jail][g_SmallFishListIdx[E_Jail]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 470,	// X 좌표
		.pos.y = 18,	// Y 좌표
		.attack = SMALLFISH_ATTACK,	// 공격력
		.isRush = false
	};

	// 용궁
	g_SmallFishList[E_DragonPalace][g_SmallFishListIdx[E_DragonPalace]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 375,
		.pos.y = 23,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};

	g_SmallFishList[E_DragonPalace][g_SmallFishListIdx[E_DragonPalace]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 445,
		.pos.y = 7,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};

	// 바다1
	g_SmallFishList[E_Sea1][g_SmallFishListIdx[E_Sea1]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 310,
		.pos.y = 14,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};

	g_SmallFishList[E_Sea1][g_SmallFishListIdx[E_Sea1]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 620,
		.pos.y = 23,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};

	// 바다2
	g_SmallFishList[E_Sea2][g_SmallFishListIdx[E_Sea2]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 236,
		.pos.y = 12,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};

	g_SmallFishList[E_Sea2][g_SmallFishListIdx[E_Sea2]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 412,
		.pos.y = 13,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};

	g_SmallFishList[E_Sea2][g_SmallFishListIdx[E_Sea2]++] = (SmallFish)
	{
		.mon = g_SmallFishMon,
		.pos.x = 570,
		.pos.y = 15,
		.attack = SMALLFISH_ATTACK,
		.isRush = false
	};
}