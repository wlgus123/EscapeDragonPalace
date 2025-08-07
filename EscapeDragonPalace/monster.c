#include "init.h"
#include "monster.h"

Monster monsterList[MAX_Monster_COUNT];	// 몬스터 배열 선언
int numMonster = 0;

// 몬스터 업데이트
void UpdateMonster()
{
	unsigned int now = _GetTickCount();
	// 몬스터 배열 for문
	for (int i = 0; i < numMonster; i++) {
		// 조개일 때
		if (monsterList[i].type == E_MONSTER_CLAM) {
			UpdateClam(monsterList[i].pos, monsterList[i].alive);
		}
		// 자라일 때
		else if (monsterList[i].type == E_MONSTER_TURTLE) {
			UpdateTurtle(now);
			continue;
			// 자라 업데이트 함수 적기
		}
		// 그 외(큰물고기, 작은물고기, 꽃게)
		else {
			// 화면에 보이지 않는 몬스터(죽었거나 다른 스테이지)인 경우 넘어가기
			if (!monsterList[i].alive) continue;
			unsigned int now = _GetTickCount();

			// 무적시간 지나면 피격 상태 해제
			if (monsterList[i].isDamaged && now - monsterList[i].lastHitTime >= INVINCIBLE_TIME) {
				monsterList[i].isDamaged = false;
			}

			// 몬스터 이동
			monsterList[i].pos.x += (monsterList[i].dir == Right) ? 1 : -1;

			// 왼쪽 벽 충돌 시 방향 전환
			/*if (monsterList[i].pos.x <= 0) {
				monsterList[i].pos.x = 0;
				monsterList[i].dir = Right;
			}*/

			// 몬스터 타입에 따라 오른쪽 벽 충돌시 방향전환
			if (monsterList[i].type == E_MONSTER_FISH) {
				// 물고기
				if (monsterList[i].pos.x + FISH_WIDTH >= monsterList[i].pos.x + monsterList[i].moveNum + GetPlusX()) {
					monsterList[i].pos.x = monsterList[i].moveNum - FISH_WIDTH;
					monsterList[i].dir = Left;
				}
			}
			else if (monsterList[i].type == E_MONSTER_CRAB) {
				// 꽃게
				if (monsterList[i].pos.x + CRAB_WIDTH >= monsterList[i].pos.x + monsterList[i].moveNum - GetPlusX()) {
					monsterList[i].pos.x = monsterList[i].moveNum - CRAB_WIDTH;
					monsterList[i].dir = Left;
				}
			}
			else if (monsterList[i].type == E_MONSTER_SMALLFISH) {
				// 작은 물고기
				if (monsterList[i].pos.x + SMALLFISH_WIDTH >= monsterList[i].pos.x + monsterList[i].moveNum + GetPlusX()) {
					monsterList[i].pos.x = monsterList[i].moveNum - SMALLFISH_WIDTH;
					monsterList[i].dir = Left;
				}
			}

		}

	}

}

// 몬스터 그리는 함수
void DrawMonster() {

	for (int i = 0; i < numMonster; i++)
	{
		int tempX = monsterList[i].pos.x - GetPlusX();  // 카메라 기준 위치 조정
		if (monsterList[i].alive) {

			switch (monsterList[i].type)
			{
			case E_MONSTER_FISH:
				_SetColor(monsterList[i].isDamaged ? 12 : 9);
				DrawFish(monsterList[i].dir, tempX, monsterList[i].pos.y);
				FishHitP(tempX, monsterList[i].pos.y);
				break;
			case E_MONSTER_CRAB:
				_SetColor(monsterList[i].isDamaged ? 6 : 12);  // 피격 시 노란색, 평시 빨간색
				DrawCrab(tempX, monsterList[i].pos.y);
				CrabHitP(tempX, monsterList[i].pos.y);
				break;
			case E_MONSTER_CLAM:
				_SetColor(E_White);
				DrawClam(tempX, monsterList[i].pos.y);
				break;
			case E_MONSTER_TURTLE:
				break;
			case E_MONSTER_SMALLFISH:
				//_SetColor(monsterList[i].isDamaged ? 12 : 15);
				DrawSmallFish(monsterList[i].dir, tempX, monsterList[i].pos.y);
				break;
			}
		}
	}

}

// 몬스터 피격 처리 함수
void HitMonster(Monster* monster, Weapon* weapon, unsigned int now) {
	if (monster->isDamaged) return;

	monster->hp -= weapon->attack;
	monster->isDamaged = true;	// 무적 상태 진입
	monster->lastHitTime = now;	// 피격 시간 기록

	if (monster->hp <= 0) {
		monster->alive = false;         // 체력이 0 이하가 되면 사망 처리
	}
}


// 몬스터 초기화 함수
void InitMonster() {
	//감옥
	//꽃게
	monsterList[numMonster++] = (Monster){
		.pos.x = 100,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 0
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 170,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 0
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 285,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 0
	};



	//물고기
	monsterList[numMonster++] = (Monster){
		.pos.x = 100,
		.pos.y = 11,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_FISH,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 0
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 170,
	.pos.y = 11,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 0
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 300,
	.pos.y = 11,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 0
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 500,
	.pos.y = 12,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 0
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 500,
	.pos.y = 21,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 0
	};

	//조개
	monsterList[numMonster++] = (Monster){
		.pos.x = 119,
		.pos.y = 13,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Jail
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 251,
	.pos.y = 18,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_CLAM,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = E_Jail
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 400,
	.pos.y = 23,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_CLAM,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = E_Jail
	};

	//용궁맵
	//꽃게
	monsterList[numMonster++] = (Monster){
		.pos.x = 70,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 1
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 170,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 1
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 280,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 1
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 440,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 1
	};


	//물고기
	monsterList[numMonster++] = (Monster){
	.pos.x = 70,
	.pos.y = 14,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 1
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 170,
	.pos.y = 14,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 1
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 280,
	.pos.y = 11,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 1
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 520,
	.pos.y = 11,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 1
	};

	//조개
	monsterList[numMonster++] = (Monster){
		.pos.x = 145,
		.pos.y = 19,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_DragonPalace
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 308,
		.pos.y = 13,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_DragonPalace
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 413,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_DragonPalace
	};

	//바다 1맵
	//꽃게
	monsterList[numMonster++] = (Monster){
		.pos.x = 120,
		.pos.y = 13,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 190,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 400,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 575,
		.pos.y = 17,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 580,
		.pos.y = 7,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 2
	};


	//물고기
	monsterList[numMonster++] = (Monster){
	.pos.x = 120,
	.pos.y = 21,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 190,
	.pos.y = 12,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 300,
	.pos.y = 21,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 400,
	.pos.y = 13,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 2
	};

	monsterList[numMonster++] = (Monster){
	.pos.x = 480,
	.pos.y = 21,
	.moveNum = 30,
	.dir = Right,
	.hp = 5,
	.alive = true,
	.type = E_MONSTER_FISH,
	.isDamaged = false,
	.lastHitTime = 0,
	.mapStatus = 2
	};

	//조개
	monsterList[numMonster++] = (Monster){
		.pos.x = 140,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea1
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 226,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea1
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 404,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea1
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 532,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea1
	};

	//바다맵2
	//꽃게
	monsterList[numMonster++] = (Monster){
		.pos.x = 95,
		.pos.y = 10,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};
	monsterList[numMonster++] = (Monster){
		.pos.x = 150,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};
	monsterList[numMonster++] = (Monster){
		.pos.x = 410,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};
	monsterList[numMonster++] = (Monster){
		.pos.x = 500,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};
	monsterList[numMonster++] = (Monster){
		.pos.x = 500,
		.pos.y = 16,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};
	monsterList[numMonster++] = (Monster){
		.pos.x = 610,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};

	//물고기
	monsterList[numMonster++] = (Monster){
		.pos.x = 260,
		.pos.y = 21,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_FISH,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 320,
		.pos.y = 16,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_FISH,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 410,
		.pos.y = 16,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_FISH,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = 3
	};

	//조개
	monsterList[numMonster++] = (Monster){
		.pos.x = 62,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 167,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 247,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 335,
		.pos.y = 23,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea2
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 374,
		.pos.y = 18,
		.moveNum = 30,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = E_MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.mapStatus = E_Sea2
	};
}