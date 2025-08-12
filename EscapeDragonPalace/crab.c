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
		if (tempCrab[idx].mon.isDamaged && GetTickCount() - tempCrab[idx].mon.lastHitTime >= MONSTER_INVINCIBLE_TIME)
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

		// TODO: 중력 작용 코드 작성

		// 플레이어가 범위에 있는지 확인
		// 꽃게 앞 뒤 범위 10일 때 인식 (절대값 사용)
		// 인식 후 20 넘어가면 어그로 풀림
		int playerMiddlePosX = player.Pos.x + RABBIT_WIDTH - 9;	// 플레이어 X좌표 중간값
		int crabMiddlePosX = tempCrab[idx].pos.x - CRAB_WIDTH / 2;	// 꽃게 X좌표 중간값

		// 플레이어, 꽃게 거리 체크 (절대값)
		int distanceAbsX = abs(crabMiddlePosX - GetPlusX() - playerMiddlePosX);
		int distanceAbsY = abs(tempCrab[idx].pos.y - player.Pos.y);

		// 어그로 범위 내에 들어오면
		if (distanceAbsX <= AGGRO_X && distanceAbsY <= AGGRO_Y)
		{
			tempCrab[idx].isChase = true;	// 플레이어 추격
		}
		
		// 어그로 범위를 벗어나면
		if (distanceAbsX > AGGRO_OFF_X || (distanceAbsY > AGGRO_Y && !player.IsJumping))
		{
			// 추격 도중 어그로가 풀렸을 경우
			if(tempCrab[idx].isChase)
			{
				tempCrab[idx].dir = !tempCrab[idx].dir;	// 현재 방향 반대로 이동
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
			else if(playerMiddlePosX - 5 > crabMiddlePosX - GetPlusX())
			{
				tempCrab[idx].dir = E_Right;
			}
			// -> ±5: 방향을 인식하는 범위 늘리기
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

// 플레이어 -> 꽃게 공격 처리
void PlayerHitCrab()
{

}

// 꽃게 -> 플레이어 공격 처리
void CrabHitPlayer()
{
	if (player.isBleeding) return;	//출혈 상태면 데미지 X

	unsigned int now = _GetTickCount();

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		Crab* tempCrab = &g_CrabList[GetMapStatus()][idx];
		int posX = tempCrab[idx].pos.x - GetPlusX();
		int posY = tempCrab[idx].pos.y;
		Rect PlayerPos = GetPlayerRect();
		Rect CrabPos = { posX + 3, posY, 1, CRAB_WIDTH };

		// 꽃게가 살아있지 않으면 넘어가기
		if (!tempCrab[idx].mon.alive) continue;

		// 꽃게와 충돌되었을 때, 출혈상태가 아닐 때 스킬 사용 및 출혈 상태로 전환
		if (IsOverlap(CrabPos, PlayerPos))
		{
			tempCrab[idx].skill.isAttack = true;
		}

		// 꽃게가 공격 상태면 플레이어 출혈 처리
		if (tempCrab[idx].skill.isAttack)
		{
			SetInvincibleTime(true);	// 플레이어 무적 설정
			player.lastHitTime = GetTickCount();	// 플레이어 마지막 피격 시간 갱신

			// 플레이어가 무적이 아닐 경우 피격당했을 떄
			if (now - lastBleedTick >= INVINCIBLE_TIME)
			{
				--player.Health;		// 플레이어 HP 감소 (반 칸)
				++bleedCnt;				// 출혈 횟수 증가
				lastBleedTick = now;	// 마지막 출혈 시간 갱신

				// 출혈 횟수가 3번을 넘어갔을 경우
				if (bleedCnt >= tempCrab[idx].skill.attackCnt)
				{
					SetInvincibleTime(false);	// 플레이어 무적 해제
					tempCrab[idx].skill.isAttack = false;	// 꽃게 공격 상태 해제
					bleedCnt = 0;	// 출혈 횟수 초기화
				}
			}
		}
	}
}

// 플레이어 출혈 처리
void BleedingPlayer() 
{
}

void ResetCrab() {}   // 꽃게 정보 초기화

// 꽃게 초기화
void InitCrab()
{
	// 꽃게 공통된 속성 설정
	g_CrabMon = (Monster)
	{
		.alive = true,		// 생존 여부
		.hp = 5,			// 체력
		.isDamaged = false,	// 피격 상태 (무적 여부)
		.lastHitTime = 0,	// 마지막 피격 시간
		.speed = 0.8,		// 이동 속도
	};
	g_CrabSkill = (Skill)
	{
		.isAttack = false,	// 스킬 사용 여부
		.attack = 1,		// 공격력 (반 칸)
		.attackCnt = 3,	// 스킬 횟수
		.coolTime = 4000,	// 쿨타임 (4초)
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
		.startPosX = 80,
		.moveNum = 96,
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
		.moveNum = 60,
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
		.moveNum = 47,
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