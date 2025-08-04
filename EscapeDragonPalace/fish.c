#include "Rabbit.h"
Monster g_Fish = { { 5, MONSTER_Y }, Right, FISH_HP, true, MONSTER_FISH, 0, 0 }; // 물고기

bool GetFishAlive()
{
	return g_Fish.alive;
}

bool GetFishIsDamaged()
{
	return g_Fish.isDamaged;
}

MyPoint GetFishPos()
{
	return g_Fish.pos;
}

int GetFishDir()
{
	return g_Fish.dir;
}


void UpdateFish(unsigned int now) {

	// monster.c UpdateMonster()로 이동

}

/*	// 수정전
void DrawFish() {
	for (int y = 0; y < FISH_HEIGHT; y++)
	{
		_SetColor(g_Fish.isDamaged ? 12 : 9);

		if (g_Fish.dir == Right) // 물고기 방향이 오른쪽일 때
		{
			char* line = g_FishGraphic[Right][y];
			int len = strlen(line);

			for (int x = 0; x < len; x++)
			{
				if (line[x] != ' ')
				{
					_DrawText(g_Fish.pos.x + x, g_Fish.pos.y + y, (char[]) { line[x], '\0' });
				}
			}
		}
		else // 물고기 방향이 왼쪽일 때
		{
			// Left 가 0 이었는데 오류나서 -1로 바꾸니까 배열에서 오류남 그냥 [0][y] 으로 바꿔둠
			char* line = g_FishGraphic[0][y];
			int len = strlen(line);

			for (int x = 0; x < len; x++)
			{
				if (line[x] != ' ')
				{
					_DrawText(g_Fish.pos.x + x, g_Fish.pos.y + y, (char[]) { line[x], '\0' });
				}
			}
		}

	}

	_SetColor(15);
}
*/

// 하영 수정 후
void DrawFish(int dir, MyPoint pos) {
	for (int y = 0; y < FISH_HEIGHT; y++)
	{
		if (dir == Right) // 물고기 방향이 오른쪽일 때
		{
			char* line = g_FishGraphic[Right][y];
			int len = strlen(line);

			for (int x = 0; x < len; x++)
			{
				if (line[x] != ' ')
				{
					_DrawText(pos.x + x, pos.y + y, (char[]) { line[x], '\0' });
				}
			}
		}
		else // 물고기 방향이 왼쪽일 때
		{
			char* line = g_FishGraphic[Left][y];
			int len = strlen(line);

			for (int x = 0; x < len; x++)
			{
				if (line[x] != ' ')
				{
					_DrawText(pos.x + x, pos.y + y, (char[]) { line[x], '\0' });
				}
			}
		}

	}

	_SetColor(15);
}


/* monster.c에 몬스터를 공격했을 때 함수 구현해둠 HitMonster()
// 몬스터 피격 함수 (체력 감소, 피격 상태 시작)
void HitFish(unsigned int now, int damage) {
	if (g_Fish.isDamaged) return; // 무적중이면 데미지 무시

	g_Fish.hp--;
	g_Fish.isDamaged = 1;
	g_Fish.lastHitTime = now;

	if (g_Fish.hp <= 0) {
		g_Fish.alive = 0; // 죽음 처리
	}
}
*/
