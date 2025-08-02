#include "monster.h"

// 전역 변수
const char* crabGraphic[CRAB_HEIGHT] = {
    "(\\/) (\\/)",    
    "  \\o_o/ ",      
    " =(___)= "       
};

Monster g_Crab = { 30, MONSTER_Y, Right, CRAB_HP, 1, MONSTER_CRAB, 0, 0 };

void UpdateCrab(unsigned int now) {
    if (!g_Crab.alive) return; // 죽었으면 처리하지 않음

    // 무적 시간 경과했는지 확인
    if (g_Crab.isDamaged && now - g_Crab.lastHitTime >= INVINCIBLE_TIME) {
        g_Crab.isDamaged = 0;
    }

    g_Crab.x += g_Crab.dir;

    if (g_Crab.x <= 0) {
        g_Crab.x = 0;
        g_Crab.dir = 1;
    }       
    if (g_Crab.x + CRAB_WIDTH >= 82) {
        g_Crab.x = 80 - CRAB_WIDTH + 2;
        g_Crab.dir = -1;
    }
}

// 꽃게 그리기 함수
void DrawCrab() {
    for (int y = 0; y < CRAB_HEIGHT; y++) {
        _SetColor(g_Crab.isDamaged ? 6 : 12);  // 피격 시 노란색, 평시 빨간색

        const char* line = crabGraphic[y]; 
        for (int x = 0; line[x] != '\0'; x++) {
            if (line[x] != ' ') {
                char ch[2] = { line[x], '\0' };
                _DrawText(g_Crab.x + x, g_Crab.y + y, ch);
            }
        }
    }

    _SetColor(15);
}

// 꽃게 피격 처리 함수
void HitCrab(unsigned int now, int damage) {
    if (g_Crab.isDamaged) return; 

    g_Crab.hp -= damage;          
    g_Crab.isDamaged = 1;         // 무적 상태 진입
    g_Crab.lastHitTime = now;     // 피격 시간 기록

    if (g_Crab.hp <= 0) {
        g_Crab.alive = 0;         // 체력이 0 이하가 되면 사망 처리
    }
}

