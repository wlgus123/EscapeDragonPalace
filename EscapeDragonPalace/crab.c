#include "Rabbit.h"

//Monster g_Crab = { 30, MONSTER_Y, Right, CRAB_HP, 1, E_MONSTER_CRAB, 0, 0 };

MyPoint GetCrabPos()
{
	//return g_Crab.pos;
}

int GetCrabDir()
{
	//return g_Crab.dir;
}

void UpdateCrab(unsigned int now) {

	// monster.c UpdateMonster()로 이동

}



bool isBleeding = false; // 출혈 상태 변수

// 꽃게 그리기 함수
// 하영 수정 =================================
void DrawCrab(int posX, int posY)
{

	// 화면 범위 밖이면 출력 안 함
	if (posX + CRAB_WIDTH < 0 || posX >= SCREEN_WIDTH) return;
	Rect PlayerPos = GetPlayerRect();
	Rect MosterPos = { posX, posY, 1, 1 };

	for (int y = 0; y < CRAB_HEIGHT; y++)
	{
		const char* line = crabGraphic[IsOverlap(PlayerPos, MosterPos) ? 1 : 0][y];
		//충돌하면 1, 비충돌 시 0
		for (int x = 0; line[x] != '\0'; x++)
		{
			if (line[x] != ' ')
			{
				if (0 <= posX + x && posX + x < SCREEN_WIDTH) {
					char ch[2] = { line[x], '\0' };
					_DrawText(posX + x, posY + y, ch);
				}
			}
		}
	}
	_SetColor(15);
}


void CrabHitP(int posX, int posY) {//꽃게 > 플레이어 공격하는 함수
	Rect PlayerPos = GetPlayerRect();
	Rect MosterPos = { posX, posY, 1, 3 };
	DWORD now = GetTickCount();
	static int count = 0;

	if (IsOverlap(PlayerPos, MosterPos))
		isBleeding = true; // 플레이어가 꽃게와 겹치면 출혈 상태로 변경

	if (isBleeding == false)
		return;

	if (count == 3) {
		count = 0; // 카운트 초기화
		isBleeding = false; // 3번 공격 후 출혈 상태 해제
	}

	if (now - monsterList[E_MONSTER_CRAB].lastHitTime < INVINCIBLE_TIME) {
		return; // 아직 때린지 1초가 안지났으면 안때림
	}
	//출혈 데미지

	player.Health -= E_CRAB_ATTACK; // 플레이어 체력 1 감소

	count++;
	monsterList[E_MONSTER_CRAB].lastHitTime = now; // 마지막 피격 시간 갱신
}
