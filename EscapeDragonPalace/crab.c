#include "monster.h"

const char* crabGraphic[CRAB_HEIGHT] = {
    "(\\/) (\\/)",    
    "   \\o_o/ ",      
    "  =(___)= "       
};

// 꽃게 초기화 함수
void InitCrab(Monster crab, int x, int y) {
    crab.x = x;                 
    crab.y = y;                 
    crab.dir = 1;               
    crab.hp = CRAB_HP;          
    crab.alive = 1;             
    crab.type = MONSTER_CRAB;   
    crab.isDamaged = 0;         
    crab.lastHitTime = 0;       
}

void UpdateCrab(Monster crab, unsigned int now) {
    if (!crab.alive) return; // 죽었으면 처리하지 않음

    // 무적 시간 경과했는지 확인
    if (crab.isDamaged && now - crab.lastHitTime >= INVINCIBLE_TIME) {
        crab.isDamaged = 0;
    }

    crab.x += crab.dir;

    if (crab.x <= 0) {
        crab.x = 0;
        crab.dir = 1;
    }       
    if (crab.x + CRAB_WIDTH >= 80) {
        crab.x = 80 - CRAB_WIDTH;
        crab.dir = -1;
    }
}

// 꽃게 그리기 함수
void DrawCrab(Monster crab) {
    for (int y = 0; y < CRAB_HEIGHT; y++) {
        _SetColor(crab.isDamaged ? 6 : 12);  // 피격 시 노란색, 평시 빨간색

        const char* line = crabGraphic[y]; 
        for (int x = 0; line[x] != '\0'; x++) {
            if (line[x] != ' ') {
                char ch[2] = { line[x], '\0' };
                _DrawText(crab.x + x, crab.y + y, ch);
            }
        }
    }

    _SetColor(15);
}

// 꽃게 피격 처리 함수
void HitCrab(Monster crab, unsigned int now, int damage) {
    if (crab.isDamaged) return; 

    crab.hp -= damage;          
    crab.isDamaged = 1;         // 무적 상태 진입
    crab.lastHitTime = now;     // 피격 시간 기록

    if (crab.hp <= 0) {
        crab.alive = 0;         // 체력이 0 이하가 되면 사망 처리
    }
}