#include "monster.h"

//1. 이동 2. 평타 3. 돌진 4. 물대포 구현해야함.

// ---------------- 물대포 관련 전역 ----------------
static bool isWaterCannonActive = false;
static unsigned int waterCannonStartTime = 0;
static unsigned int lastWaterDropTime = 0;
static unsigned int nextWaterCannonCooldown = 0;
static unsigned int waterCannonDuration = 0;
static unsigned int waterDropInterval = 0;
static Turtle g_Turtle;
//----------------------------------------------------


void DrawTurtle(int dir, int posX, int posY) {
    if (posX + TURTLE_WIDTH < 0 || posX >= SCREEN_WIDTH) return;

    for (int y = 0; y < TURTLE_HEIGHT; y++) {
        const char* line = turtleGraphic[dir][y];
        int len = strlen(line);

        for (int x = 0; x < len; x++) {
            char ch = line[x];
            if (ch != ' ') {
                int drawX = posX + x;
                int drawY = posY + y;
                
                if (0 <= drawX && drawX < SCREEN_WIDTH && 0 <= drawY && drawY < SCREEN_HEIGHT) {
                    char temp[2] = { ch, '\0' };
                    _DrawText(drawX, drawY, temp);
                }
            }
        }
    }
}

#define MAX_WATER_DROPS 10

typedef struct {
    int x, y;
    bool active;
} WaterDrop;

WaterDrop g_WaterDrops[MAX_WATER_DROPS];

// --------------------------------------------------

// 초기화 시 쿨타임 지정
void InitTurtle() {
        srand(time(NULL)); // 랜덤 초기화
    nextWaterCannonCooldown = rand() % 4001 + 8000; // 8000 ~ 12000
}

// 물대포 업데이트 함수
void UpdateTurtleWaterCannon(unsigned int now) {
    if (!isWaterCannonActive) {
        if (now - g_Turtle.lastAttackTime >= nextWaterCannonCooldown) {
            isWaterCannonActive = true;
            waterCannonStartTime = now;
            g_Turtle.lastAttackTime = now;

            waterCannonDuration = rand() % 2001 + 3000;   // 3~5초
            waterDropInterval = rand() % 201 + 500;       // 500~700ms
            lastWaterDropTime = now;

            for (int i = 0; i < MAX_WATER_DROPS; i++)
                g_WaterDrops[i].active = false;
        }
        return;
    }

    // 공격 중인 경우
    if (now - waterCannonStartTime >= waterCannonDuration) {
        isWaterCannonActive = false;
        nextWaterCannonCooldown = rand() % 4001 + 8000; // 다음 쿨타임 설정
        return;
    }

    // 물방울 생성 타이밍
    if (now - lastWaterDropTime >= waterDropInterval) {
        lastWaterDropTime = now;
        // 비어있는 물방울 슬롯에 하나 생성
        for (int i = 0; i < MAX_WATER_DROPS; i++) {
            if (!g_WaterDrops[i].active) {
                g_WaterDrops[i].x = rand() % SCREEN_WIDTH;
                g_WaterDrops[i].y = 0;
                g_WaterDrops[i].active = true;
                break;
            }
        }
    }

    // 물방울 낙하
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        if (g_WaterDrops[i].active) {
            g_WaterDrops[i].y += 1;
            if (g_WaterDrops[i].y >= SCREEN_HEIGHT) {
                g_WaterDrops[i].active = false;
            }
        }
    }
}

// 물방울 그리기
void DrawWaterDrops() {
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        if (g_WaterDrops[i].active) {
            _DrawText(g_WaterDrops[i].x, g_WaterDrops[i].y, "o");  // 또는 "o"나 "*"
        }
    }
}