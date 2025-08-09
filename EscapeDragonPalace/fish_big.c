#include "init.h"
#include "fish_big.h"
#include "Rabbit.h"

Monster g_BigFishMon;	// 큰 물고기 몬스터 구조체 공통 설정
BigFish g_BigFishList[STAGE_CNT][BIGFISH_CNT];	// 큰 물고기 포인트 배열
int g_BigFishListIdx[STAGE_CNT] = { 0, };

// 큰 물고기 업데이트
void UpdateBigFish(unsigned long now)
{
	// 현재 맵의 몬스터 데이터 불러오기
	BigFish* tempBigFish = g_BigFishList[GetMapStatus()];
	// 현재 맵에 있는 몬스터 수만큼 반복하기
	for (int idx = 0; idx < g_BigFishListIdx[GetMapStatus()]; idx++)
	{
		// 몬스터가 죽었을 경우 넘어가기
		if (!tempBigFish[idx].mon.alive) continue;

		// 무적시간 지나면 피격 상태 해제
		if (tempBigFish[idx].mon.isDamaged && now - tempBigFish[idx].mon.lastHitTime >= INVINCIBLE_TIME)
		{
			tempBigFish[idx].mon.isDamaged = false;
		}

		// 몬스터 이동
		tempBigFish[idx].pos.x += (tempBigFish[idx].dir == E_Right) ? g_BigFishMon.speed : -g_BigFishMon.speed;

		// 정해진 범위 안에서 이동
		if (tempBigFish[idx].pos.x <= tempBigFish[idx].startPosX)
		{
			tempBigFish[idx].dir = E_Right;
		}
		else if (tempBigFish[idx].pos.x + BIGFISH_WIDTH >= tempBigFish[idx].startPosX + tempBigFish[idx].moveNum)
		{
			tempBigFish[idx].dir = E_Left;
		}
	}
}

// 큰 물고기 그리기
void DrawBigFish()
{
	// 현재 맵 데이터 임시로 불러오기
	BigFish* tempBigFish = g_BigFishList[GetMapStatus()];
	for (int idx = 0; idx < g_BigFishListIdx[GetMapStatus()]; idx++)
	{

		// 몬스터가 죽었을 경우 넘어가기
		if (!tempBigFish[idx].mon.alive) continue;

		// 피격 시 빨간색, 평시 파란색
		_SetColor(tempBigFish[idx].mon.isDamaged ? E_BrightRed : E_BrightBlue);
		int posX = tempBigFish[idx].pos.x - GetPlusX();
		for (int y = 0; y < BIGFISH_HEIGHT; y++)
		{
			for (int x = 0; x < BIGFISH_WIDTH; x++)
			{
				if (g_BigFishGraphic[tempBigFish[idx].dir][y][x] != ' ')
				{
					// 화면 범위 내에 있을 경우 그리기
					if (0 <= posX + x && SCREEN_WIDTH > posX + x)
					{
						_DrawText(posX + x, tempBigFish[idx].pos.y + y,
							(char[]) {
							g_BigFishGraphic[tempBigFish[idx].dir][y][x], 0
						});
					}
				}
			}
		}
	}
}

//물고기 > 플레이어 공격하는 함수
void BigFishHitPlayer()
{
	BigFish* bigFishList = &g_BigFishList[GetMapStatus()];
	DWORD now = GetTickCount();
	Rect PlayerPos = GetPlayerRect();

	for (int idx = 0; idx < g_BigFishListIdx[GetMapStatus()]; idx++)
	{
		BigFish* tempFish = &bigFishList[idx];
		int posX = tempFish->pos.x - GetPlusX();
		int posY = tempFish->pos.y;
		Rect MosterPos = { posX, posY, 13, 3 };

		if (!tempFish->mon.alive) continue;

		if (!(IsOverlap(PlayerPos, MosterPos)))
			continue;

		// 무적 시간 체크
		if (now - tempFish->mon.lastHitTime < INVINCIBLE_TIME) {
			continue; // 아직 무적 상태면 데미지 무시
		}

		SetInvincibleTime(true);	// 플레이어 무적 시간 설정
		player.Health -= tempFish->attack; // 플레이어 체력 2 감소
		tempFish->mon.lastHitTime = now; // 마지막 피격 시간 갱신


	}
}

// 플레이어 > 큰 물고기 공격하는 함수
void PlayerHitBigFish()
{
	BigFish* bigFishList = &g_BigFishList[GetMapStatus()];
	DWORD now = GetTickCount();
	Rect PlayerWeaponPos = GetWeaponRect();

	for (int idx = 0; idx < g_BigFishListIdx[GetMapStatus()]; idx++)
	{
		BigFish* tempFish = &bigFishList[idx];
		int posX = tempFish->pos.x - GetPlusX();
		int posY = tempFish->pos.y;
		Rect MosterPos = { posX, posY, 13, 3 };

		if (!player.IsAttacking) continue;

		if (!tempFish->mon.alive) continue; // 몬스터가 죽었을 경우 넘어가기

		if (tempFish->mon.isDamaged) continue; // 몬스터가 무적 상태일 경우 넘어가기

		if (!(IsOverlap(PlayerWeaponPos, MosterPos))) continue;

		if (now - player.lastHitTime < INVINCIBLE_TIME) continue;
		tempFish->mon.hp -= player.HeldWeapon->attack; // 물고기 체력 감소
		tempFish->mon.isDamaged = true; // 무적 상태로 변경
		player.lastHitTime = now; // 마지막 피격 시간 갱신


		if (tempFish->mon.hp <= 0) {
			tempFish->mon.alive = false;
		}
	}
}


void ResetBigFish() {
	for (int i = 0; i < STAGE_CNT; i++)
	{
		BigFish* tempBigFish = g_BigFishList[i];
		for (int idx = 0; idx < g_BigFishListIdx[i]; idx++)
		{
			tempBigFish[idx].mon.alive = false;
		}
	}
}


// 큰 물고기 초기화
void InitBigFish()
{
	// 큰 물고기 공통된 속성 설정
	g_BigFishMon = (Monster)
	{
		.alive = true,		// 생존 여부
		.hp = 4,			// 체력
		.isDamaged = false,	// 피격 상태 (무적 여부)
		.lastHitTime = 0,	// 마지막 피격 시간
		.speed = 0.6,		// 이동 속도
	};

	// 감옥
	g_BigFishList[E_Jail][g_BigFishListIdx[E_Jail]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 95,		// X 좌표
		.pos.y = 11,		// Y 좌표
		.startPosX = 95,	// 초기 X 좌표
		.attack = BIGFISH_ATTACK,	// 공격력
		.moveNum = 62,		// 이동 범위
		.dir = E_Right,		// 바라보는 방향
	};

	g_BigFishList[E_Jail][g_BigFishListIdx[E_Jail]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 170,
		.pos.y = 11,
		.startPosX = 170,
		.attack = BIGFISH_ATTACK,
		.moveNum = 62,
		.dir = E_Right,
	};

	g_BigFishList[E_Jail][g_BigFishListIdx[E_Jail]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 277,
		.pos.y = 11,
		.startPosX = 277,
		.attack = BIGFISH_ATTACK,
		.moveNum = 74,
		.dir = E_Right,
	};

	g_BigFishList[E_Jail][g_BigFishListIdx[E_Jail]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 490,
		.pos.y = 12,
		.startPosX = 490,
		.attack = BIGFISH_ATTACK,
		.moveNum = 57,
		.dir = E_Right,
	};

	g_BigFishList[E_Jail][g_BigFishListIdx[E_Jail]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 547,
		.pos.y = 21,
		.startPosX = 490,
		.attack = BIGFISH_ATTACK,
		.moveNum = 57,
		.dir = E_Right,
	};

	// 용궁
	g_BigFishList[E_DragonPalace][g_BigFishListIdx[E_DragonPalace]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 71,
		.pos.y = 14,
		.startPosX = 71,
		.attack = BIGFISH_ATTACK,
		.moveNum = 59,
		.dir = E_Right,
	};

	g_BigFishList[E_DragonPalace][g_BigFishListIdx[E_DragonPalace]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 169,
		.pos.y = 14,
		.startPosX = 169,
		.attack = BIGFISH_ATTACK,
		.moveNum = 36,
		.dir = E_Right,
	};

	g_BigFishList[E_DragonPalace][g_BigFishListIdx[E_DragonPalace]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 245,
		.pos.y = 11,
		.startPosX = 245,
		.attack = BIGFISH_ATTACK,
		.moveNum = 84,
		.dir = E_Right,
	};

	g_BigFishList[E_DragonPalace][g_BigFishListIdx[E_DragonPalace]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 514,
		.pos.y = 11,
		.startPosX = 514,
		.attack = BIGFISH_ATTACK,
		.moveNum = 52,
		.dir = E_Right,
	};

	// 바다1
	g_BigFishList[E_Sea1][g_BigFishListIdx[E_Sea1]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 80,
		.pos.y = 21,
		.startPosX = 80,
		.attack = BIGFISH_ATTACK,
		.moveNum = 94,
		.dir = E_Right,
	};

	g_BigFishList[E_Sea1][g_BigFishListIdx[E_Sea1]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 187,
		.pos.y = 12,
		.startPosX = 187,
		.attack = BIGFISH_ATTACK,
		.moveNum = 72,
		.dir = E_Right,
	};

	g_BigFishList[E_Sea1][g_BigFishListIdx[E_Sea1]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 290,
		.pos.y = 21,
		.startPosX = 290,
		.attack = BIGFISH_ATTACK,
		.moveNum = 56,
		.dir = E_Right,
	};

	g_BigFishList[E_Sea1][g_BigFishListIdx[E_Sea1]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 373,
		.pos.y = 13,
		.startPosX = 373,
		.attack = BIGFISH_ATTACK,
		.moveNum = 72,
		.dir = E_Right,
	};

	g_BigFishList[E_Sea1][g_BigFishListIdx[E_Sea1]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 458,
		.pos.y = 21,
		.startPosX = 458,
		.attack = BIGFISH_ATTACK,
		.moveNum = 119,
		.dir = E_Right,
	};

	// 바다2
	g_BigFishList[E_Sea2][g_BigFishListIdx[E_Sea2]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 231,
		.pos.y = 21,
		.startPosX = 231,
		.attack = BIGFISH_ATTACK,
		.moveNum = 86,
		.dir = E_Right,
	};

	g_BigFishList[E_Sea2][g_BigFishListIdx[E_Sea2]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 313,
		.pos.y = 16,
		.startPosX = 313,
		.attack = BIGFISH_ATTACK,
		.moveNum = 79,
		.dir = E_Right,
	};

	g_BigFishList[E_Sea2][g_BigFishListIdx[E_Sea2]++] = (BigFish)
	{
		.mon = g_BigFishMon,
		.pos.x = 401,
		.pos.y = 16,
		.startPosX = 401,
		.attack = BIGFISH_ATTACK,
		.moveNum = 51,
		.dir = E_Right,
	};
}