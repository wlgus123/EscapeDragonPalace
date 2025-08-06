#include "Rabbit.h"

void TriggerClam() {

}

// 이거 rabbit.c에 GetMonsterRect() 사용해서 다시 코드 짜는게 좋을 듯
void UpdateClam(MyPoint pos, bool alive) {

	// 플레이어가 조개 위에 올라감 → 발동
	if (player.Pos.x == pos.x && player.Pos.y == pos.y) {
		alive = false;
	}
}

// 조개 그리기
void DrawClam(int posX, int posY) {
	for (int y = 0; y < CLAM_HEIGHT; y++) {
		for (int x = 0; x < CLAM_WIDTH; x++)
		{
			// 몬스터 위치가 화면 내에 있을 때만 출력
			if (posX + x >= 0 && posX + x < SCREEN_WIDTH)
			{
				_DrawText(posX + x, posY + y, (char[]) { g_ClamGraphic[x], 0 });
			}
		}
	}
}


