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
    if (g_Clam.alive) {
        _DrawText(pos.x, pos.y, clamGraphic);
    }
    else {
        _DrawText(pos.x, pos.y, " ");
    }
}


