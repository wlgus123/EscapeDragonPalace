#include "Rabbit.h"

void UpdateFish(unsigned int now) {

	// monster.c UpdateMonster()로 이동

}

// 쓸모없는 주석 제거
void DrawFish(int dir, int posX, int posY) {

	// 화면 범위 밖이면 출력 안 함
	if (posX + FISH_WIDTH < 0 || posX >= SCREEN_WIDTH) return;

	for (int y = 0; y < FISH_HEIGHT; y++) {
		char* line = g_FishGraphic[dir][y];
		int len = strlen(line);

		for (int x = 0; x < len; x++) {
			if (line[x] != ' ') {
				if (0 <= posX + x && posX + x < SCREEN_WIDTH) {
					_DrawText(posX + x, posY + y, (char[]) { line[x], '\0' });
				}
			}
		}
	}

	_SetColor(15);

}

void FishHitP(int posX, int posY) {//물고기 > 플레이어 공격하는 함수
	Rect PlayerPos = { player.Pos.x, player.Pos.y, 8, 3 };
	Rect MosterPos = { posX, posY, 3, 3 };
	DWORD now = GetTickCount();

	if ((IsOverlap(PlayerPos, MosterPos)) == false)
		return;

	// 무적 시간 체크
	if (now - monsterList[E_MONSTER_CRAB].lastHitTime < INVINCIBLE_TIME) {
		return; // 아직 무적 상태면 데미지 무시
	}

	player.Health -= E_FISH_ATTACK; // 플레이어 체력 2 감소

	monsterList[E_MONSTER_CRAB].lastHitTime = now; // 마지막 피격 시간 갱신
}
