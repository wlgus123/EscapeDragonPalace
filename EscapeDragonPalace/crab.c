#include "init.h"
#include "crab.h"
#include "Rabbit.h"

// 전역 변수
Monster g_CrabMon;	// 꽃게 몬스터 구조체 공통 설정
Skill g_CrabSkill;	// 꽃게 스킬 구조체 공통 설정
Crab g_CrabList[STAGE_CNT][CRAB_CNT];	// 꽃게 포인트 배열
int g_CrabListIdx[STAGE_CNT] = { 0, };
DWORD lastBleedTick = 0; // 출혈 데미지 체크용 타이머
int bleedCount = 0;      // 출혈 데미지 횟수(3)



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

static void Crab_ApplyGravity(Crab* crab) //중력을 적용
{
	if (!Crab_IsOnGround(crab))// 꽃게가 발판 위에 있지 않으면 중력 적용
	{
		// 발판 없으면 낙하
		crab->pos.y += 1.0f;

		// 화면 하단 범위 제한
		if (crab->pos.y > SCREEN_HEIGHT - CRAB_HEIGHT -1) //-1 안하면 안에 들어감
			crab->pos.y = SCREEN_HEIGHT - CRAB_HEIGHT -1;
	}
	else
	{
		// 발판 위에 고정
		int tileY = (crab->pos.y + CRAB_HEIGHT) / TILE_SIZE;
		crab->pos.y = (tileY * TILE_SIZE) - CRAB_HEIGHT;
	}
}

void UpdateCrab(unsigned long now)
{
	Crab* tempCrab = g_CrabList[GetMapStatus()];

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{

		//만약 사망한 꽃게라면 넘어감
		if (tempCrab[idx].mon.alive == false) continue;



		// 중력 적용
		Crab_ApplyGravity(&tempCrab[idx]);


		// 플레이어가 범위에 들어왔는지 확인
		if (player.Pos.x - (tempCrab[idx].pos.x - GetPlusX()) > (player.Pos.x - GetPlusX()) - CRAB_EYE)// 플레	이어가 범위에 들어왔을 경우
			tempCrab[idx].state = true;
		else if (player.Pos.x - (tempCrab[idx].pos.x - GetPlusX()) < (player.Pos.x - GetPlusX()) + CRAB_EYE)// 플레이어가 범위에 들어오지 않았을 경우
			tempCrab[idx].state = false;

		
		if (tempCrab[idx].state) {// 꽃게가 플레이어를 추격하는 상태
			if (tempCrab[idx].pos.x > player.Pos.x + GetPlusX() + RABBIT_WIDTH)// 플레이어가 꽃게보다 오른쪽에 있을 경우
				tempCrab[idx].dir = E_Left;
			else if (tempCrab[idx].pos.x < player.Pos.x + GetPlusX())// 플레이어가 꽃게보다 왼쪽에 있을 경우
				tempCrab[idx].dir = E_Right;
		}
		else {// 꽃게가 플레이어를 추격하지 않는 상태(범위 내에서 움직임)
			if (tempCrab[idx].pos.x <= tempCrab[idx].startPosX)
				tempCrab[idx].dir = E_Right;
			if (tempCrab[idx].pos.x + CRAB_WIDTH >= tempCrab[idx].startPosX + tempCrab[idx].moveNum)
				tempCrab[idx].dir = E_Left;
		}

		// 이동
		tempCrab[idx].pos.x += (tempCrab[idx].dir == E_Right) ? g_CrabMon.speed : -g_CrabMon.speed;
	}
}

	


void DrawCrab()
{
	// 현재 맵 데이터 임시로 불러오기
	Crab* tempCrab = g_CrabList[GetMapStatus()];
	int imageDir;
	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		if (!tempCrab[idx].mon.alive) continue;


		// 무적시간 지나면 피격 상태 해제
		if (tempCrab[idx].mon.isDamaged && GetTickCount() - tempCrab[idx].mon.lastHitTime >= MONSTER_INVINCIBLE_TIME)
			tempCrab[idx].mon.isDamaged = false;

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
}//꽃게 그리기

// 꽃게 공격 체크, 공격 애니메이션 처리
void CheckAttacking()
{
	for (int i = 0; i < g_CrabListIdx[GetMapStatus()]; i++)
	{
		// 죽은 몬스터일 경우 넘어가기
		if (!g_CrabList[GetMapStatus()][i].mon.alive) continue;
	}
}



//꽃게 > 플레이어 충돌 체크(체크만 하는 용도)
void CrabHitPlayer() {
	if (player.isBleeding == true) return; // 이미 출혈 상태면 충돌 체크 안 함

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		Crab* tempCrab = &g_CrabList[GetMapStatus()][idx];
		int posX = tempCrab->pos.x - GetPlusX();
		int posY = tempCrab->pos.y;
		Rect PlayerPos = GetPlayerRect();
		Rect MonsterPos = { posX, posY, 1, 3 };

		if (!tempCrab->mon.alive) continue;

		if (IsOverlap(PlayerPos, MonsterPos)) {
			// 출혈 시작
			player.isBleeding = true;
			bleedCount = 0; // 출혈 데미지 카운트 초기화
			lastBleedTick = GetTickCount() - INVINCIBLE_TIME; // 출혈 타이머 초기화
		}

	}
}

void BleedPlayer() {//출혈
	if (!player.isBleeding) return; // 출혈 상태가 아니면 아무것도 안 함

	DWORD now = GetTickCount();

	SetInvincibleTime(true);	// 플레이어 무적 시간 설정
	player.lastHitTime = GetTickCount(); // 플레이어 마지막 피격 시간 갱신
	
	if (now - lastBleedTick >= INVINCIBLE_TIME) {
		player.Health -= 1;       // HP 감소
		bleedCount++;             // 데미지 횟수 증가
		lastBleedTick = now;      // 타이머 갱신

		if (bleedCount >= 3) {    // 3번 맞으면 출혈 종료
			SetInvincibleTime(false);	// 플레이어 무적 시간 설정
			player.isBleeding = false;
			bleedCount = 0;
		}
	}
}

// 플레이어 -> 꽃게 공격
void PlayerHitCrab()
{
	Crab* crabList = &g_CrabList[GetMapStatus()];
	DWORD now = GetTickCount();
	Rect PlayerWeaponPos = GetWeaponRect();

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		Crab* tempCrab = &g_CrabList[GetMapStatus()][idx];// 현재 꽃게 정보
		int posX = tempCrab->pos.x - GetPlusX();
		int posY = tempCrab->pos.y;
		Rect MosterPos = { posX, posY, 10, 3 };

		if (!player.IsAttacking) continue;//플레이어가 공격 중이 아닐 경우 넘어가기

		if (!tempCrab->mon.alive) continue; // 몬스터가 죽었을 경우 넘어가기

		if (tempCrab->mon.isDamaged) continue; // 몬스터가 무적 상태일 경우 넘어가기

		if (!(IsOverlap(PlayerWeaponPos, MosterPos))) continue;

		if (now - tempCrab->mon.lastHitTime < MONSTER_INVINCIBLE_TIME) continue;
		tempCrab->mon.hp -= player.HeldWeapon->attack; // 꽃게 체력 감소
		tempCrab->mon.isDamaged = true; // 무적 상태로 변경
		tempCrab->mon.lastHitTime = now; // 마지막 피격 시간 갱신


		if (tempCrab->mon.hp <= 0) {
			tempCrab->mon.alive = false;// 체력이 0 이하가 되면 사망 처리
		}
	}
}


void ResetCrab() {
	for (int i = 0; i < STAGE_CNT; i++)
	{
		Crab* tempCrab = g_CrabList[i];
		for (int idx = 0; idx < g_CrabListIdx[i]; idx++)
		{
			tempCrab[idx].mon.alive = false;
		}
	}
}

// 꽃게 초기화
void InitCrab()
{
	for (int i = 0; i < STAGE_CNT; i++) {
		g_CrabListIdx[i] = 0;
	}
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