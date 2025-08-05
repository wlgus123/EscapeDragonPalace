#include "init.h"
#include "monster.h"

Monster monsterList[MAX_Monster_COUNT];	// 몬스터 배열 선언
int numMonster = 0;

// 몬스터 업데이트
void UpdateMonster()
{
	// 하영 수정 ================================

	for (int i = 0; i < numMonster; i++) {

		if (monsterList[i].type == MONSTER_CLAM) {
			UpdateClam(monsterList[i].pos, monsterList[i].alive);
		}
		else if (monsterList[i].type == MONSTER_TURTLE) {

		}
		else {
			// 화면에 보이지 않는 몬스터(죽었거나 다른 스테이지)인 경우 넘어가기
			if (!monsterList[i].alive) continue;
			unsigned int now = _GetTickCount();

			// 무적시간 지나면 피격 상태 해제
			if (monsterList[i].isDamaged && now - monsterList[i].lastHitTime >= INVINCIBLE_TIME) {
				monsterList[i].isDamaged = 0;
			}

			// 몬스터 이동
			if (monsterList[i].dir == Right)
				monsterList[i].pos.x += 1;
			else
				monsterList[i].pos.x -= 1;

			// 왼쪽 벽 충돌 시 방향 전환
			if (monsterList[i].pos.x <= 0) {
				monsterList[i].pos.x = 0;
				monsterList[i].dir = Right;
			}

			// 몬스터 타입에 따라 오른쪽 벽 충돌시 방향전환
			if (monsterList[i].type == MONSTER_FISH) {
				// 물고기
				if (monsterList[i].pos.x + FISH_WIDTH >= 82) {
					monsterList[i].pos.x = 80 - FISH_WIDTH + 2;
					monsterList[i].dir = Left;
				}
			}
			else {
				// 꽃게
				if (monsterList[i].pos.x + CRAB_WIDTH >= 82) {
					monsterList[i].pos.x = 80 - CRAB_WIDTH + 2;
					monsterList[i].dir = Left;
				}
			}

		}

	}

	//UpdateCrab(now);
	//UpdateClam();
	// =========================================

	// int damage = 1; // 무기 공격력 받아와야 함
	/*
	if (GetFishAlive()) {
		UpdateFish(now);
	}

	if (GetFishIsDamaged())
	{
		HitFish(now, damage);
	}
	*/
}

// 몬스터 그리는 함수
void DrawMonster(Monster monster) {
	switch (monster.type)
	{
	case MONSTER_FISH:
		_SetColor(monster.isDamaged ? 12 : 9);
		DrawFish(monster.dir, monster.pos);
		break;
	case MONSTER_CRAB:
		_SetColor(monster.isDamaged ? 6 : 12);  // 피격 시 노란색, 평시 빨간색
		DrawCrab(monster.pos);
		break;
	case MONSTER_CLAM:
		DrawClam(monster.pos);
		break;
	case MONSTER_TURTLE:
		break;
	}
}


// 몬스터 피격 처리 함수
void HitMonster(Monster monster, Weapon* weapon, unsigned int now) {
	if (monster.isDamaged) return;

	monster.hp -= weapon->attack;
	monster.isDamaged = true;	// 무적 상태 진입
	monster.lastHitTime = now;	// 피격 시간 기록

	if (monster.hp <= 0) {
		monster.alive = false;         // 체력이 0 이하가 되면 사망 처리
	}
}


// 몬스터 초기화 함수
void InitMonster() {
	monsterList[numMonster++] = (Monster){
		.pos.x = 40,
		.pos.y = 21,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = MONSTER_CRAB,
		.isDamaged = false,
		.lastHitTime = 0,
		.monsterStatus = 0
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 20,
		.pos.y = 21,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = MONSTER_FISH,
		.isDamaged = false,
		.lastHitTime = 0,
		.monsterStatus = 0
	};

	monsterList[numMonster++] = (Monster){
		.pos.x = 30,
		.pos.y = 23,
		.dir = Right,
		.hp = 5,
		.alive = true,
		.type = MONSTER_CLAM,
		.isDamaged = false,
		.lastHitTime = 0,
		.monsterStatus = 0
	};
}