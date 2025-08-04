#include "monster.h"

// 전역 변수
char crabGraphic[2][CRAB_HEIGHT][CRAB_WIDTH] = {
    {"(\\/) (\\/) ",
     "  \\o_o/ ",
     " =(___)= "}, //비폭력0

    {"(l)  (l) ",
     " \\o_o/ ",
     "=(___)= " //Be폭력1
    }       
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

        const char* line = crabGraphic[0][y];
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

/*void ChasunsignedeCrab(unsigned int slap, int Paware, int Maware) {
    Paware = g_Crab.x + 5; //양수 인식범위
    Maware = g_Crab.x - 5; //음수 인식범위

    if (player.Pos.x > Paware || player.Pos.x < Maware)
        return;
        
    //플레이어 위치에 따라 움직임
    if (player.Pos.x > g_Crab.x){
        g_Crab.x += g_Crab.x;
    }
    if (player.Pos.x < g_Crab.x) {ㅐ
        g_Crab.x -= g_Crab.x;
    }
    
    //플레이어와 같아졌다면
    if (player.Pos.x == g_Crab.x && player.Pos.y == g_Crab.y) {
        player.Health -= player.Health;

    }

}*/