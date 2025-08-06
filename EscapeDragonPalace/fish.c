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

// 쓸모없는 주석 제거
void DrawFish(int dir, MyPoint pos) {

	int tempX = pos.x - GetPlusX();  // 카메라 기준 위치 조정

	// 화면 범위 밖이면 출력 안 함
	if (tempX + FISH_WIDTH < 0 || tempX >= SCREEN_WIDTH) return;

	for (int y = 0; y < FISH_HEIGHT; y++) {
		char* line = g_FishGraphic[dir][y];
		int len = strlen(line);

		for (int x = 0; x < len; x++) {
			if (line[x] != ' ') {
				if (0 <= tempX + x && tempX + x < SCREEN_WIDTH) {
					_DrawText(tempX + x, pos.y + y, (char[]) { line[x], '\0' });
				}
			}
		}
	}

	_SetColor(15);

}
