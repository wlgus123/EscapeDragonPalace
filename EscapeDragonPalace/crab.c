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




// 꽃게 그리기 함수
// 하영 수정 =================================
void DrawCrab(int posX, int posY)
{
	
	// 화면 범위 밖이면 출력 안 함
	if (posX + CRAB_WIDTH < 0 || posX >= SCREEN_WIDTH) return;
	Rect PlayerPos = {player.Pos.x, player.Pos.y, 8, 3 };
	Rect MosterPos = { posX, posY, 9, 3 };
	
	_SetColor(monsterList[E_MONSTER_CRAB].isDamaged ? 6 : 12);// 피격 시 노란색, 평시 빨간색

	for (int y = 0; y < CRAB_HEIGHT; y++)
	{
		if(IsOverlap(PlayerPos, MosterPos))
		{
			const char* line = crabGraphic[IsOverlap ? 1 : 0][y]; //충돌하면 1, 비충돌 시 0
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
	}
	_SetColor(15);
}
//받아오는 부분이 다르다고 챗지피티는 그랬는데 일단 모르겠음

void CrabHitP() {
	static DWORD lastHitTime = 0;  // 마지막 공격 시간
	static int hitCount = 0;       // 총 공격 횟수

	// 몬스터와 플레이어 위치 정보
	Rect monsterpos = GetMonsterRect(monsterList[E_MONSTER_CRAB]);
	Rect playerpos = GetPlayerRect();

	if (IsOverlap(playerpos, monsterpos)) {
		DWORD now = GetTickCount();

		// 공격 3번까지만 허용
		if (hitCount < 3) {
			// 1초(1000ms)마다 공격
			if (now - lastHitTime >= 1000) {
				player.Health -= E_CRAB_ATTACK;
				lastHitTime = now;
				hitCount++;
			}
		}
	}
	else {
		// 떨어지면 리셋 (다시 가까워지면 다시 3번 공격 가능)
		lastHitTime = 0;
		hitCount = 0;
	}
}

