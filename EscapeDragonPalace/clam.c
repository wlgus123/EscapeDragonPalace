#include "init.h"
#include "monster.h"
#include "Rabbit.h"

const char* clamGraphic = "。";

Monster g_Clam = { 20, 23, Right, CLAM_HP, 1, MONSTER_CLAM, 0, 0 };

void TriggerClam() {
    
}

void UpdateClam() {
    if (!g_Clam.alive) return;

    // 플레이어가 조개 위에 올라감 → 발동
    if (RabbitX == g_Clam.x && RabbitY == g_Clam.y) {
        g_Clam.alive = 0;
    }
}

void DrawClam() {
    if (g_Clam.alive) {
        _DrawText(g_Clam.x, g_Clam.y, clamGraphic);
    }
    else {
        _DrawText(g_Clam.x, g_Clam.y, " ");
    }
}


