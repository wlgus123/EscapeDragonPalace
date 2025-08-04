#include "init.h"
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

void UpdateClam() {
    if (!g_Clam.alive) return;

    // 플레이어가 조개 위에 올라감 → 발동
    if (RabbitX == g_Clam.pos.x && RabbitY == g_Clam.pos.y) {
        g_Clam.alive = 0;
    }
}

void DrawClam() {
    if (g_Clam.alive) {
        _DrawText(g_Clam.pos.x, g_Clam.pos.y, clamGraphic);
    }
    else {
        _DrawText(g_Clam.pos.x, g_Clam.pos.y, " ");
    }
}


