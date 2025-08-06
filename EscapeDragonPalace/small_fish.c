#include "Rabbit.h"

Monster g_SmallFish = { { 10, MONSTER_Y }, Right, SMALLFISH_HP, true, MONSTER_SMALLFISH, 0, 0 }; // 물고기

bool GetSmallFishAlive()
{
	return g_SmallFish.alive;
}

bool GetSmallFishIsDamaged()
{
	return g_SmallFish.isDamaged;
}

MyPoint GetSmallFishPos()
{
	return g_SmallFish.pos;
}

int GetSmallFishDir()
{
	return g_SmallFish.dir;
}

void DrawSmallFish(int dir, int posX, int posY) {

	int tempX = posX - GetPlusX();  // 카메라 기준 위치 조정

	// 화면 범위 밖이면 출력 안 함
	if (tempX + SMALLFISH_WIDTH < 0 || tempX >= SCREEN_WIDTH) return;

	for (int y = 0; y < SMALLFISH_HEIGHT; y++) {
		char* line = g_SmallFishGraphic[dir][y];
		int len = strlen(line);

		for (int x = 0; x < len; x++) {
			if (line[x] != ' ') {
				if (0 <= tempX + x && tempX + x < SCREEN_WIDTH) {
					_DrawText(tempX + x, posY + y, (char[]) { line[x], '\0' });
				}
			}
		}
	}

	_SetColor(15);

}