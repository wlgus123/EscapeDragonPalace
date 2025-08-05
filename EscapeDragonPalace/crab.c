#include "Rabbit.h"

Monster g_Crab = { 30, MONSTER_Y, Right, CRAB_HP, 1, MONSTER_CRAB, 0, 0 };

MyPoint GetCrabPos()
{
	return g_Crab.pos;
}

int GetCrabDir()
{
	return g_Crab.dir;
}

void UpdateCrab(unsigned int now) {

	// monster.c UpdateMonster()로 이동

}


// 꽃게 그리기 함수
// 하영 수정 =================================
void DrawCrab(MyPoint pos)
{
	float dx = player.Pos.x - g_Crab.pos.x; //여기서부터
	float dy = player.Pos.y - g_Crab.pos.y;

	_SetColor(g_Crab.isDamaged ? 6 : 12);// 피격 시 노란색, 평시 빨간색

	for (int y = 0; y < CRAB_HEIGHT; y++)
	{
		if (sqrt(dx * dx + dy * dy) <= 2)
		{
			const char* line = crabGraphic[1][y];
			for (int x = 0; line[x] != '\0'; x++) 
			{
				if (line[x] != ' ') 
				{
					char ch[2] = { line[x], '\0' };
					_DrawText(g_Crab.pos.x + x, g_Crab.pos.y + y, ch);
				}
			}
		}
		else
		{
			const char* line = crabGraphic[0][y];
			for (int x = 0; line[x] != '\0'; x++)
			{
				if (line[x] != ' ')
				{
					char ch[2] = { line[x], '\0' };
					_DrawText(pos.x + x, pos.y + y, ch);
				}
			}
		}
	}
	_SetColor(15);
}

/* monster.c에 몬스터를 공격했을 때 함수 구현해둠 HitMonster()
// 꽃게 피격 처리 함수
void HitCrab(unsigned int now, int damage) {
	if (g_Crab.isDamaged) return;

	g_Crab.hp -= damage;
	g_Crab.isDamaged = 1;         // 무적 상태 진입
	g_Crab.lastHitTime = now;     // 피격 시간 기록

	if (g_Crab.hp <= 0) {
		g_Crab.alive = 0;         // 체력이 0 이하가 되면 사망 처리
	}
}
*/

/*void ChasunsignedeCrab(unsigned int slap, int Paware, int Maware) {
	Paware = g_Crab.x + 5; //양수 인식범위
	Maware = g_Crab.x - 5; //음수 인식범위

	if (player.Pos.x > Paware || player.Pos.x < Maware)
		return;

	//플레이어 위치에 따라 움직임
	if (player.Pos.x > g_Crab.x){
		g_Crab.x += g_Crab.x;
	}
	if (player.Pos.x < g_Crab.x) {ㅐ
		g_Crab.x -= g_Crab.x;
	}

	//플레이어와 같아졌다면
	if (player.Pos.x == g_Crab.x && player.Pos.y == g_Crab.y) {
		player.Health -= player.Health;

	}

}*/