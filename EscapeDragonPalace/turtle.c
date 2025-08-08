#include "init.h"
#include "turtle.h"
#include "map.h"


Turtle g_Turtle;

static TurtleState g_State;

static unsigned int g_NextRushTime, g_PrepStartTime, g_RushEndTime;
static bool g_ShowPrep;
static int g_RushCount;
static int g_RushDir;
static int g_InitialDir;
static int g_LastRandIdx;

static bool g_ShowTarget = false;
static unsigned int g_TargetPreviewEnd = 0;
static int g_PendingTargetY = 0;

int g_TargetFirstY = TURTLE_IDLE_Y + 1;
int g_TargetOptions[] = { 4, 9, 14 };
int g_StartX[2] = { -TURTLE_WIDTH, SCREEN_WIDTH };

static bool g_WaterActive = false;        // 물대포(물방울) 활성 여부
static unsigned int g_NextDropTime = 0;   // 다음 물방울 생성 시각
static char g_WaterChar = '@';            // 물방울 문자

typedef struct {
    bool active;
    int x, y;
    unsigned int lastMoveTime;
} WaterDrop;
static WaterDrop g_WaterDrops[MAX_WATER_DROPS];

static void ClearWaterDrops(void) {
    for (int i = 0; i < MAX_WATER_DROPS; ++i) g_WaterDrops[i].active = false;
}
static void SpawnWaterDrop(unsigned int now) {
    for (int i = 0; i < MAX_WATER_DROPS; ++i) {
        if (!g_WaterDrops[i].active) {
            g_WaterDrops[i].active = true;
            // 화면 위에서 랜덤 X에 생성
            g_WaterDrops[i].x = randRange(0, SCREEN_WIDTH - 1);
            g_WaterDrops[i].y = 0;
            g_WaterDrops[i].lastMoveTime = now;
            break;
        }
    }
}
static void UpdateWaterDrops(unsigned int now) {
    for (int i = 0; i < MAX_WATER_DROPS; ++i) {
        if (!g_WaterDrops[i].active) continue;
        if (now - g_WaterDrops[i].lastMoveTime >= WATER_DROP_MOVE_MS) {
            g_WaterDrops[i].y += 1;
            g_WaterDrops[i].lastMoveTime = now;
            // 화면 아래로 벗어나면 제거
            if (g_WaterDrops[i].y > SCREEN_HEIGHT + 5) g_WaterDrops[i].active = false;
        }
    }
}
static void DrawWaterDrops(void) {
    char s[2] = { g_WaterChar, '\0' }; _SetColor(E_Blue);
    for (int i = 0; i < MAX_WATER_DROPS; ++i) {
        if (!g_WaterDrops[i].active) continue;
        int dropX = g_WaterDrops[i].x;
        int dropY = g_WaterDrops[i].y;

        if (g_Turtle.alive) {
            int tLeft = g_Turtle.pos.x;
            int tTop = g_Turtle.pos.y;
            int tLines = (g_State == TURTLE_STATE_RUSHING ? TURTLE_HEIGHT - 1 : TURTLE_HEIGHT);
            int tRight = tLeft + TURTLE_WIDTH - 1;
            int tBottom = tTop + tLines - 1;
            if (dropX >= tLeft && dropX <= tRight && dropY >= tTop && dropY <= tBottom) {
                continue;
            }
        }

        int dx = dropX - GetPlusX();
        int dy = dropY;
        _DrawText(dx, dy, s);
    }
    _SetColor(E_White);
}

const char* turtleGraphic[2][TURTLE_HEIGHT] = {
    {
        "       ______",
        "|\\   _/ \\__/ \\_ ___",
        "| \\_/ \\ /  \\ / \\ o_)",
        " \\__----------- __/",
        "    \\_|_|_|_|_/",
        "    |_|_\\  |_|_\\ "
    },
    {
        "       ______ ",
        " ___ _/ \\__/ \\_   /|",
        "(_o / \\ /  \\ / \\_/ |",
        " \\__ -----------__/",
        "     \\_|_|_|_|_/ ",
        "    /_|_|  /_|_|"
    }
};
static unsigned int randRange(unsigned min, unsigned max) { return min + rand() % (max - min + 1); }

void InitTurtle(unsigned int now) {
    srand((unsigned)time(NULL));
    g_Turtle.pos.x = 58;
    g_Turtle.pos.y = TURTLE_IDLE_Y;
    g_Turtle.speed = 1.0f;
    g_Turtle.dir = 1;
    g_Turtle.hp = TURTLE_HP;
    g_Turtle.attack = 2;
    g_Turtle.alive = true;
    g_Turtle.isDamaged = false;
    g_Turtle.lastHitTime = now;
    g_State = TURTLE_STATE_IDLE;
    g_NextRushTime = now + randRange(10000, 15000);
    g_PrepStartTime = g_NextRushTime - 2000;
    g_ShowPrep = false;
    g_RushCount = 0;
    g_InitialDir = -1;
    g_RushDir = g_InitialDir;
    g_LastRandIdx = -1;

    g_ShowTarget = false;
    g_TargetPreviewEnd = 0;
    g_PendingTargetY = 0;

    g_WaterActive = true;
    g_NextDropTime = now + randRange(WATER_DROP_INTERVAL_MIN_MS, WATER_DROP_INTERVAL_MAX_MS);
    ClearWaterDrops();
}

void UpdateTurtle(unsigned int now) {
    if (!g_Turtle.alive) return;
    if (g_Turtle.isDamaged && now - g_Turtle.lastHitTime >= 1000)
        g_Turtle.isDamaged = false;

    if (g_WaterActive) {
        if (now >= g_NextDropTime) {
            SpawnWaterDrop(now);
            g_NextDropTime = now + randRange(WATER_DROP_INTERVAL_MIN_MS, WATER_DROP_INTERVAL_MAX_MS);
        }
        UpdateWaterDrops(now);
    }

    switch (g_State) {
    case TURTLE_STATE_IDLE:
        if (now >= g_PrepStartTime) {
            g_State = TURTLE_STATE_PREPARE_RUSH;
            g_PrepStartTime = now;
            g_ShowPrep = true;
        }
        break;
    case TURTLE_STATE_PREPARE_RUSH:
        if (now - g_PrepStartTime >= 2000) {
            g_State = TURTLE_STATE_RUSHING;
            g_ShowPrep = false;
            g_RushCount = 0;
            g_RushDir = g_InitialDir;
            g_Turtle.dir = (g_RushDir < 0 ? 1 : 0);
            g_Turtle.pos.y = g_TargetFirstY;
            g_Turtle.pos.x = (g_RushDir < 0 ? g_StartX[1] - 20 : g_StartX[0] + 20);
            g_RushEndTime = now + 1000;
        }
        break;
    case TURTLE_STATE_RUSHING: {
        if (g_ShowTarget) {
            if (now < g_TargetPreviewEnd) {
                break;
            }
            else {
                g_ShowTarget = false;
                g_Turtle.pos.y = g_PendingTargetY;
                g_Turtle.pos.x = (g_RushDir < 0 ? g_StartX[1] - 20 : g_StartX[0] + 20);
                g_RushEndTime = now + 900;
            }
        }

        int step = (int)(g_Turtle.speed * 3);
        g_Turtle.pos.x += (g_RushDir < 0 ? -step : step);
        bool passed = (g_RushDir < 0)
            ? (g_Turtle.pos.x + TURTLE_WIDTH < 0)
            : (g_Turtle.pos.x > SCREEN_WIDTH);
        if (passed && now >= g_RushEndTime) {
            g_RushCount++;
            if (g_RushCount < TURTLE_RUSH_COUNT) {
                g_RushDir = -g_RushDir;
                g_Turtle.dir = (g_RushDir < 0 ? 1 : 0);
                int opts = sizeof(g_TargetOptions) / sizeof(int);
                int idx;
                do { idx = randRange(0, opts - 1); } while (idx == g_LastRandIdx && opts > 1);
                g_LastRandIdx = idx;

                // 미리보기 설정: 1초 동안 목표 Y를 화면에 보여주고 그 뒤에 출현하여 돌진 시작
                g_PendingTargetY = g_TargetOptions[idx];
                g_ShowTarget = true;
                g_TargetPreviewEnd = now + 1000; // 1초 미리보기

            }
            else {
                g_State = TURTLE_STATE_IDLE;
                g_Turtle.pos.y = TURTLE_IDLE_Y;

                g_InitialDir = -g_InitialDir;

                if (g_InitialDir > 0) {
                    g_Turtle.pos.x = g_StartX[0] + 14;
                    g_Turtle.dir = 0;
                }
                else {
                    g_Turtle.pos.x = g_StartX[1] - 22;
                    g_Turtle.dir = 1;
                }

                g_NextRushTime = now + randRange(20000, 25000);
                g_PrepStartTime = g_NextRushTime - 2000;
            }
        }
        break;
    }
    }
}

void DrawTurtle(void) {
    if (!g_Turtle.alive) return;

    // 물방울 그리기
    if (g_WaterActive) DrawWaterDrops();

    if (g_ShowTarget) {
        int mx = SCREEN_WIDTH / 2 - 3 + GetPlusX();
        _DrawText(mx + 3, g_PendingTargetY + 3, "!");
    }

    // 돌진 중에 자라가 화면 밖으로 완전히 나가면 그리지 않음
    if (g_State == TURTLE_STATE_RUSHING &&
        (g_Turtle.pos.x < 0 || g_Turtle.pos.x + TURTLE_WIDTH > SCREEN_WIDTH)) return;


    _SetColor(E_White); //자라 흰색유지

    int x = g_Turtle.pos.x - GetPlusX();
    if (g_State == TURTLE_STATE_PREPARE_RUSH) {
        const char* m = "자라가 돌진을 준비중입니다!";
        int l = strlen(m);
        _DrawText(SCREEN_WIDTH / 2 - l / 2 + GetPlusX(), 1, m);
    }
    int idx = (g_Turtle.dir == 0 ? 0 : 1);
    int lines = (g_State == TURTLE_STATE_RUSHING ? TURTLE_HEIGHT - 1 : TURTLE_HEIGHT);
    for (int r = 0; r < lines; ++r) _DrawText(x, g_Turtle.pos.y + r, turtleGraphic[idx][r]);
}
