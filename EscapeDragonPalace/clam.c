#include "Rabbit.h"

Monster g_Clam = { { 20, 23 }, Right, CLAM_HP, true, MONSTER_CLAM, 0, 0 };

MyPoint GetClamPos()
{
    return g_Clam.pos;
}

int GetClamDir()
{
    return g_Clam.dir;
}

void TriggerClam() {
    
}

// 이거 rabbit.c에 GetMonsterRect() 사용해서 다시 코드 짜는게 좋을 듯
void UpdateClam(MyPoint pos, bool alive) {

    // 플레이어가 조개 위에 올라감 → 발동
    if (player.Pos.x == pos.x && player.Pos.y == pos.y) {
        alive = false;
    }
}

void DrawClam(MyPoint pos) {
    int tempX = pos.x + GetPlusX();

    //if (g_Clam.alive) {
    //    for (int y = 0; y < CLAM_HEIGHT; y++) {
    //        for (int x = 0; x < CLAM_WIDTH; x++)
    //        {
    //            // 몬스터 위치가 화면 내에 있을 때만 출력
    //            if (tempX - x >= 0 && tempX + x < SCREEN_WIDTH)
    //            {
    //                _DrawText(tempX + x, pos.y + y, (char[]) { clamGraphic[x], 0 });
    //            }
    //        }
    //    }
    //}
    //else
    //{
    //    _DrawText(pos.x + GetPlusX(), pos.y, " ");
    //}

    if (g_Clam.alive) {
        _DrawText(pos.x, pos.y, clamGraphic);
    }
    else {
        _DrawText(pos.x, pos.y, " ");
    }
}


