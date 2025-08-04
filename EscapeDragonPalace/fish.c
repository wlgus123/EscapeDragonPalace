#include "monster.h"

Monster g_Fish = { { 5, MONSTER_Y }, Right, FISH_HP, true, MONSTER_FISH, 0, 0 }; // 물고기

bool GetFishAlive()
{
	return g_Fish.alive;
}

bool GetFishIsDamaged()
{
	return g_Fish.isDamaged;
}

void UpdateFish(unsigned int now) {
	if (!g_Fish.alive) return;

	// 무적시간 지나면 피격 상태 해제
	if (g_Fish.isDamaged && now - g_Fish.lastHitTime >= INVINCIBLE_TIME) {
		g_Fish.isDamaged = 0;
	}

	// 이동
	g_Fish.pos.x += g_Fish.dir;

	// 벽 충돌 시 방향 전환
	if (g_Fish.pos.x <= 0) {
		g_Fish.pos.x = 0;
		g_Fish.dir = 1;
	}
	if (g_Fish.pos.x + FISH_WIDTH >= 82) {
		g_Fish.pos.x = 80 - FISH_WIDTH + 2;
		g_Fish.dir = -1;
	}
}

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
			char* line = g_FishGraphic[Left][y];
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
