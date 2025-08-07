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
//충돌 하면 닫히게 만듬
// 조개 그리기
void DrawClam(int posX, int posY) {
	// 화면 범위 밖이면 출력 안 함
	if (posX + CLAM_WIDTH < 0 || posX >= SCREEN_WIDTH) return;
	Rect PlayerPos = { player.Pos.x, player.Pos.y, 8, 3 };
	Rect MosterPos = { posX, posY, 1, 1 };

	for (int y = 0; y < CLAM_HEIGHT; y++)
	{
		const char* line = g_ClamGraphic[IsOverlap(PlayerPos, MosterPos) ? 1 : 0][y];
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
}


