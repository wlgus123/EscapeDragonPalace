#include "crab.h"
#include "Rabbit.h"

// 전역 변수
Monster g_CrabMon;	// 꽃게 몬스터 구조체 공통 설정
Skill g_CrabSkill;	// 꽃게 스킬 구조체 공통 설정
Crab g_CrabList[STAGE_CNT][CRAB_CNT];	// 꽃게 포인트 배열
int g_CrabListIdx[STAGE_CNT] = { 0, };	// 맵 별 꽃게 수

static int bleedCnt = 0;	// 출혈 횟수
static unsigned long lastBleedTick = 0;	// 마지막 출혈 시간

// 꽃게 그리기
void DrawCrab()
{
	// 현재 맵 데이터 임시로 불러오기
	Crab* tempCrab = g_CrabList[GetMapStatus()];

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		if (!tempCrab[idx].mon.alive) continue;


		// 무적시간 지나면 피격 상태 해제
		if (tempCrab[idx].mon.isDamaged && _GetTickCount() - tempCrab[idx].mon.lastHitTime >= MONSTER_INVINCIBLE_TIME)
			tempCrab[idx].mon.isDamaged = false;

		// 피격 시 노란색, 평시 빨간색
		_SetColor(tempCrab[idx].mon.isDamaged ? E_Yellow : E_BrightRed);

		int posX = tempCrab[idx].pos.x - GetPlusX();

		for (int y = 0; y < CRAB_HEIGHT; y++)
		{
			for (int x = 0; x < CRAB_WIDTH; x++)
			{
				// 화면 범위를 넘어갈 경우 그리지 않기
				if (0 > posX + x || SCREEN_WIDTH <= posX + x) continue;

				_DrawText(posX + x, tempCrab[idx].pos.y + y, (char[]) { g_CrabGraphic[0][y][x], 0 });
			}
		}
	}
}

// 꽃게 업데이트
void UpdateCrab()
{
	// 현재 맵 데이터 임시로 불러오기
	Crab* tempCrab = g_CrabList[GetMapStatus()];

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		// 만약 살아있지 않으면 넘어가기
		if (!tempCrab[idx].mon.alive) continue;

		// 중력 적용
		Crab_ApplyGravity(&tempCrab[idx]);

		// 플레이어가 범위에 있는지 확인
		// 꽃게 앞 뒤 범위 10일 때 인식 (절대값 사용)
		// 인식 후 20 넘어가면 어그로 풀림
		int playerMiddlePosX = player.Pos.x + RABBIT_WIDTH - 9;	// 플레이어 X좌표 중간값
		int crabMiddlePosX = tempCrab[idx].pos.x - CRAB_WIDTH / 2;	// 꽃게 X좌표 중간값

		// 플레이어, 꽃게 거리 체크 (절대값)
		int distanceAbsX = abs(crabMiddlePosX - GetPlusX() - playerMiddlePosX);
		int distanceAbsY = abs(tempCrab[idx].pos.y - player.Pos.y);

		// TODO: 나중에 주석 풀기
		// 어그로 범위 내에 들어오면
		//if (distanceAbsX <= AGGRO_X && distanceAbsY <= AGGRO_Y)
		//{
		//	tempCrab[idx].isChase = true;	// 플레이어 추격
		//	tempCrab[idx].mon.speed = AGGRO_SPEED;	// 속도 올리기
		//}

		// 어그로 범위를 벗어나면
		if (distanceAbsX > AGGRO_OFF_X || (distanceAbsY > AGGRO_Y && !player.IsJumping))
		{
			// 추격 도중 어그로가 풀렸을 경우
			if (tempCrab[idx].isChase)
			{
				tempCrab[idx].dir = !tempCrab[idx].dir;	// 현재 방향 반대로 이동
				tempCrab[idx].mon.speed = CRAB_SPEED;	// 원래 속도대로 변경
			}
			tempCrab[idx].isChase = false;	// 플레이어 추격 해제
		}

		// 추격상태일 때 이동
		if (tempCrab[idx].isChase)
		{
			// 꽃게가 플레이어보다 오른쪽에 있을 때
			if (playerMiddlePosX + 5 < crabMiddlePosX - GetPlusX())
			{
				tempCrab[idx].dir = E_Left;
			}
			// 플레이어보다 왼쪽에 있을 때
			else if (playerMiddlePosX - 5 > crabMiddlePosX - GetPlusX())
			{
				tempCrab[idx].dir = E_Right;
			}
			// -> ±5: 방향을 인식하는 범위
		}
		else
		{
			// 일반 이동: 정해진 범위 안에서 이동
			// TODO: 어그로 풀렸을 때 풀린 그 자리에서 움직이는 거 구현해보기
			if (tempCrab[idx].pos.x <= tempCrab[idx].startPosX)
			{
				tempCrab[idx].dir = E_Right;
			}
			else if (tempCrab[idx].pos.x + CRAB_WIDTH >= tempCrab[idx].startPosX + tempCrab[idx].moveNum)
			{
				tempCrab[idx].dir = E_Left;
			}
		}

		// 꽃게 이동
		tempCrab[idx].pos.x += (tempCrab[idx].dir == E_Right)
			? tempCrab[idx].mon.speed
			: -tempCrab[idx].mon.speed;
	}
}

static bool Crab_IsOnGround(Crab* crab)// 꽃게가 발판 위에 있는지 확인
{
	// 중앙 X좌표
	int tileX = (crab->pos.x + (CRAB_WIDTH / 2)) / TILE_SIZE;
	// 발밑 Y타일 좌표
	int tileY = (crab->pos.y + CRAB_HEIGHT) / TILE_SIZE;

	// 맵 범위 체크
	if (tileY < 0 || tileY >= MAP_HEIGHT || tileX < 0 || tileX >= MAP_WIDTH)
		return false;

	// 현재 맵 받아오기
	int mapStatus = GetMapStatus();

	// 스테이지 발판 확인
	if (mapStatus >= 0 && mapStatus < STAGE_CNT)
	{
		if (g_StagePlatform[mapStatus][tileY][tileX] == '=')
			return true;
	}

	// 기본 맵 발판 확인
	if (g_Map[tileY][tileX] == '=')
		return true;

	return false;
}

static void Crab_ApplyGravity(Crab* crab) //중력 적용
{
	if (!Crab_IsOnGround(crab))// 꽃게가 발판 위에 있지 않으면 중력 적용
	{
		// 발판 없으면 낙하
		crab->pos.y += 1.0f;

		// 화면 하단 범위 제한
		if (crab->pos.y > SCREEN_HEIGHT - CRAB_HEIGHT - 1) //-1 안하면 안에 들어감
			crab->pos.y = SCREEN_HEIGHT - CRAB_HEIGHT - 1;
	}
	else
	{
		// 발판 위에 고정
		int tileY = (crab->pos.y + CRAB_HEIGHT) / TILE_SIZE;
		crab->pos.y = (tileY * TILE_SIZE) - CRAB_HEIGHT;
	}
}

// 플레이어 -> 꽃게 공격 처리
void PlayerHitCrab()
{
	Crab* tempCrab = g_CrabList[GetMapStatus()];	// 꽃게 임시 정보 가져오기
	unsigned long now = _GetTickCount();	// 현재 시간 가져오기
	Rect weaponPos = GetWeaponRect();	// 무기 위치 정보 가져오기

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		// 몬스터 위치 정보
		int posX = tempCrab[idx].pos.x - GetPlusX();
		int posY = tempCrab[idx].pos.y;
		Rect monsterPos = { posX, posY, CRAB_WIDTH, CRAB_HEIGHT };

		if (!player.IsAttacking // 플레이어가 공격 중이 아닐 경우
			|| !tempCrab[idx].mon.alive	// 몬스터가 죽었을 경우
			|| tempCrab[idx].mon.isDamaged) continue;	// 몬스터가 무적 상태일 경우 넘어가기

		// 충돌 시 꽃게 데미지 처리
		if (IsOverlap(weaponPos, monsterPos))
		{
			// 꽃게 무적 시간이 안 지났을 경우 넘어가기
			if (now - tempCrab[idx].mon.lastHitTime < MONSTER_INVINCIBLE_TIME) continue;

			tempCrab[idx].mon.hp -= player.HeldWeapon->attack;	// 꽃게 체력 감소
			tempCrab[idx].mon.isDamaged = true;		// 무적 상태 변경
			tempCrab[idx].mon.lastHitTime = now;	// 마지막 피격당한 시간 갱신

		}

		// 꽃게 체력이 0 이하일 때
		if (tempCrab[idx].mon.hp <= 0)
		{
			tempCrab[idx].mon.alive = false;	// 사망 처리
		}
	}
}

// 꽃게 -> 플레이어 공격 처리
void CrabHitPlayer()
{
	//if (player.isBleeding) return;	//출혈 상태면 데미지 X

	//unsigned int now = _GetTickCount();

	//for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	//{
	//	Crab* tempCrab = g_CrabList[GetMapStatus()];
	//	// 꽃게 위치값
	//	int posX = tempCrab[idx].pos.x - GetPlusX();
	//	int posY = tempCrab[idx].pos.y;
	//	Rect PlayerPos = GetPlayerRect();
	//	Rect CrabPos = { posX + 3, posY, CRAB_WIDTH, CRAB_HEIGHT };

	//	// 꽃게가 살아있지 않으면 넘어가기
	//	if (!tempCrab[idx].mon.alive) continue;

	//	// 꽃게와 충돌되었을 때, 출혈상태가 아닐 때
	//	// 각 꽃게 스킬 쿨타임이 지났을 때 스킬 사용
	//	if (IsOverlap(CrabPos, PlayerPos)
	//		&& now - tempCrab[idx].skill.coolTime >= tempCrab[idx].skill.coolTime)
	//	{
	//		tempCrab[idx].skill.isAttack = true;
	//	}

	//	// 꽃게가 공격 상태면 플레이어 출혈 처리
	//	if (tempCrab[idx].skill.isAttack)
	//	{
	//		SetInvincibleTime(true);	// 플레이어 무적 설정
	//		player.lastHitTime = _GetTickCount();	// 플레이어 마지막 피격 시간 갱신

	//		// 플레이어가 무적이 아닐 경우 피격당했을 떄
	//		if (now - lastBleedTick >= INVINCIBLE_TIME)
	//		{
	//			--player.Health;		// 플레이어 HP 감소 (반 칸)
	//			++bleedCnt;				// 출혈 횟수 증가
	//			lastBleedTick = now;	// 마지막 출혈 시간 갱신

	//			// 출혈 횟수가 3번을 넘어갔을 경우
	//			if (bleedCnt >= tempCrab[idx].skill.attackCnt)
	//			{
	//				SetInvincibleTime(false);	// 플레이어 무적 해제
	//				tempCrab[idx].skill.isAttack = false;	// 꽃게 공격 상태 해제
	//				tempCrab[idx].skill.coolTime = CRAB_SKILL_COOLTIME;	// 꽃게 쿨타임 초기화
	//				bleedCnt = 0;	// 출혈 횟수 초기화
	//			}
	//		}
	//	}
	//}
}

// 꽃게 정보 초기화 
void ResetCrab()
{
	Crab* tempCrab = g_CrabList[GetMapStatus()];
	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		tempCrab[idx].mon.alive = false;
	}
}

// 꽃게 초기화
void InitCrab()
{
	// 꽃게 공통된 속성 설정
	g_CrabMon = (Monster)
	{
		.alive = true,			// 생존 여부
		.hp = 5,				// 체력
		.isDamaged = false,		// 피격 상태 (무적 여부)
		.lastHitTime = 0,		// 마지막 피격 시간
		.speed = CRAB_SPEED,	// 이동 속도
	};
	g_CrabSkill = (Skill)
	{
		.isAttack = false,	// 스킬 사용 여부
		.attack = 1,		// 공격력 (반 칸)
		.attackCnt = 3,		// 스킬 횟수
		.coolTime = CRAB_SKILL_COOLTIME,	// 스킬 쿨타임
	};

	// 감옥
	g_CrabList[E_Jail][g_CrabListIdx[E_Jail]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 95,		// X 좌표
		.pos.y = 21,		// Y 좌표
		.startPosX = 95,	// 초기 X 좌표
		.moveNum = 64,		// 이동 범위
		.dir = E_Right,		// 이동 방향
		.attackStatus = E_NONE,	// 꽃게 공격 상태
	};

	g_CrabList[E_Jail][g_CrabListIdx[E_Jail]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 170,
		.pos.y = 21,
		.startPosX = 170,
		.moveNum = 64,
		.dir = E_Right,
		.attackStatus = E_NONE,	// 꽃게 공격 상태
	};

	g_CrabList[E_Jail][g_CrabListIdx[E_Jail]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 277,
		.pos.y = 21,
		.startPosX = 277,
		.moveNum = 92,
		.dir = E_Right,
		.attackStatus = E_NONE,	// 꽃게 공격 상태
	};

	// 용궁
	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 72,
		.pos.y = 21,
		.startPosX = 72,
		.moveNum = 60,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 170,
		.pos.y = 21,
		.startPosX = 170,
		.moveNum = 61,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 246,
		.pos.y = 21,
		.startPosX = 246,
		.moveNum = 86,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 440,
		.pos.y = 21,
		.startPosX = 440,
		.moveNum = 125,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	// 바다1
	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 80,
		.pos.y = 13,
		.startPosX = 83,
		.moveNum = 90,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 187,
		.pos.y = 21,
		.startPosX = 187,
		.moveNum = 75,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 373,
		.pos.y = 21,
		.startPosX = 373,
		.moveNum = 73,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 574,
		.pos.y = 17,
		.startPosX = 574,
		.moveNum = 56,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 580,
		.pos.y = 7,
		.startPosX = 580,
		.moveNum = 44,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	// 바다2
	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 76,
		.pos.y = 10,
		.startPosX = 76,
		.moveNum = 76,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 150,
		.pos.y = 21,
		.startPosX = 150,
		.moveNum = 41,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 404,
		.pos.y = 21,
		.startPosX = 404,
		.moveNum = 50,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 546,
		.pos.y = 21,
		.startPosX = 491,
		.moveNum = 55,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 491,
		.pos.y = 16,
		.startPosX = 491,
		.moveNum = 55,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 610,
		.pos.y = 21,
		.startPosX = 610,
		.moveNum = 62,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};
}