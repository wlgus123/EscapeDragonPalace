#include "init.h"
#include "crab.h"
#include "Rabbit.h"

// 전역 변수
Monster g_CrabMon;	// 꽃게 몬스터 구조체 공통 설정
Skill g_CrabSkill;	// 꽃게 스킬 구조체 공통 설정
Crab g_CrabList[STAGE_CNT][CRAB_CNT];	// 꽃게 포인트 배열
int g_CrabListIdx[STAGE_CNT] = { 0, };
DWORD lastBleedTick = 0; // 출혈 데미지 체크용 타이머
int bleedCount = 0;      // 출혈 데미지 횟수



// 꽃게 업데이트
void UpdateCrab(unsigned long now)
{
	// 현재 맵의 몬스터 데이터 불러오기
	Crab* tempCrab = g_CrabList[GetMapStatus()];

	// 현재 맵에 있는 몬스터 수만큼 반복하기
	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		// 몬스터가 죽었을 경우 넘어가기
		if (!tempCrab[idx].mon.alive) continue;

		// 무적시간 지나면 피격 상태 해제
		if (tempCrab[idx].mon.isDamaged && now - tempCrab[idx].mon.lastHitTime >= INVINCIBLE_TIME)
		{
			tempCrab[idx].mon.isDamaged = false;
		}

		if (player.Pos.x - (tempCrab[idx].pos.x - GetPlusX()) > (player.Pos.x - GetPlusX()) - 20) {	// 플레이어가 꽃게와 가까워지면 추격 상태로 변경
			tempCrab[idx].state = true;
		}

		// 플레이어 인식 후
		if (tempCrab[idx].state == true) {
			// 플레이어가 꽃게보다 오른쪽에 있을 때
			if(tempCrab[idx].pos.x > player.Pos.x + GetPlusX() + RABBIT_WIDTH) {
				tempCrab[idx].dir = E_Left;
			}
			// 플레이어가 꽃게보다 왼쪽에 있을 때
			else if (tempCrab[idx].pos.x < player.Pos.x + GetPlusX())
				tempCrab[idx].dir = E_Right;
		}

		// 플레이어 인식 전
		if(tempCrab[idx].state == false){
			if (tempCrab[idx].pos.x <= tempCrab[idx].startPosX)
			{
				tempCrab[idx].dir = E_Right;
			}
			if (tempCrab[idx].pos.x + CRAB_WIDTH >= tempCrab[idx].startPosX + tempCrab[idx].moveNum)
			{
				tempCrab[idx].dir = E_Left;
			}
		}
		

		// 몬스터 이동
		tempCrab[idx].pos.x += (tempCrab[idx].dir == E_Right) ? g_CrabMon.speed : -g_CrabMon.speed;

	}
}

// 꽃게 공격 체크, 공격 애니메이션 처리
void CheckAttacking()
{
	for (int i = 0; i < g_CrabListIdx[GetMapStatus()]; i++)
	{
		// 죽은 몬스터일 경우 넘어가기
		if (!g_CrabList[GetMapStatus()][i].mon.alive) continue;
	}
}

// 꽃게 피격 처리
//void HitCrab(unsigned int now) {
//	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
//	{
//		Crab tempCrab = g_CrabList[GetMapStatus()][idx];
//		if (tempCrab.mon.alive) return;
//
//		tempCrab.mon.hp -= player.HeldWeapon->attack;
//		tempCrab.mon.isDamaged = true;	// 무적 상태 진입
//		tempCrab.mon.lastHitTime = now;	// 피격 시간 기록
//
//		if (tempCrab.mon.hp <= 0) {
//			tempCrab.mon.alive = false;	// 체력이 0 이하가 되면 사망 처리
//		}
//	}
//}

// 꽃게 > 플레이어 공격하는 함수


void CrabHitPlayer() {
	if (player.isBleeding == true) return; // 이미 출혈 상태면 충돌 체크 안 함

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		Crab* tempCrab = &g_CrabList[GetMapStatus()][idx];
		int posX = tempCrab->pos.x - GetPlusX();
		int posY = tempCrab->pos.y;
		Rect PlayerPos = GetPlayerRect();
		Rect MonsterPos = { posX, posY, 1, 3 };

		if (IsOverlap(PlayerPos, MonsterPos)) {
			// 출혈 시작
			player.isBleeding = true;
			bleedCount = 0; // 출혈 데미지 카운트 초기화
			lastBleedTick = GetTickCount(); // 출혈 타이머 초기화
		}
	}
}

void BleedPlayer() {
	if (!player.isBleeding) return;

	DWORD now = GetTickCount();

	
	if (now - lastBleedTick >= 1000) {
		player.Health -= 1;       // HP 감소
		bleedCount++;             // 데미지 횟수 증가
		lastBleedTick = now;      // 타이머 갱신

		if (bleedCount >= 3) {    // 3번 맞으면 출혈 종료
			player.isBleeding = false;
			bleedCount = 0;
		}
	}
}

// 꽃게 그리기
void DrawCrab()
{
	// 현재 맵 데이터 임시로 불러오기
	Crab* tempCrab = g_CrabList[GetMapStatus()];
	int imageDir;
	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		// 피격 시 노란색, 평시 빨간색
		_SetColor(tempCrab[idx].mon.isDamaged ? E_Yellow : E_BrightRed);

		int posX = tempCrab[idx].pos.x - GetPlusX();

		if(player.isBleeding ==true){
			imageDir = E_Left;
		}
		else {
			imageDir = E_Right;
		}
		

		for (int y = 0; y < CRAB_HEIGHT; y++)
		{
			for (int x = 0; x < CRAB_WIDTH; x++)
			{
				if (g_CrabGraphic[imageDir][y][x] != ' ')
				{
					// 화면 범위 내에 있을 경우 그리기
					if (0 <= posX + x && SCREEN_WIDTH > posX + x)
					{
						_DrawText(posX + x, tempCrab[idx].pos.y + y,
							(char[]) {g_CrabGraphic[imageDir][y][x], 0});
					}
				}
			}
		}
	}
}

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
		.attackCnt = 3,		// 공격 횟수
		.coolTime = 4000,	// 쿨타임 (2초)
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
		.dir = E_Right,
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
	};
}