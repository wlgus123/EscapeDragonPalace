#include "init.h"
#include "turtle.h"
#include "map.h"

Turtle g_Turtle;


static TurtleState g_State;          // 현재 상태
static unsigned int g_NextRushTime;  // 다음 돌진 준비 시작 시간
static unsigned int g_PrepStartTime; // 돌진 준비 시작
static unsigned int g_RushEndTime;   // 현재 돌진의 종료(맵 통과) 기대 시간
static bool g_ShowPrep;              // 돌진 준비 문구용
static int g_RushCount;              // 3번의 돌진중 몇번째인지 여부
static int g_RushDir;
static int g_InitialDir;             // 사이클 시작 시 첫 돌진 방향 토글
static int g_LastRandIdx;            // 랜덤 선택 중복 방지

// 미리보기용 변수 (2,3번째 돌진에서 돌진 지점 보여주기)
static bool g_ShowTarget = false;
static unsigned int g_TargetPreviewEnd = 0; //미리보기 끝
static int g_PendingTargetY = 0;

int g_TargetFirstY = TURTLE_IDLE_Y + 1; //첫 돌진 Y (다리 사라지니까 Y+1)
int g_TargetOptions[TURTLE_RUSH_OPTION_COUNT] = { 4, 9, 14 };
int g_StartX[2] = { -TURTLE_WIDTH, SCREEN_WIDTH };

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

static unsigned int randRange(unsigned min, unsigned max) {
    return min + (unsigned)(rand() % (max - min + 1));
}

void InitTurtle(unsigned int now) {
    srand((unsigned)time(NULL));

    // 기본 위치/상태
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
    g_NextRushTime = now + randRange(10000, 15000); // 10~15초 후 다음 돌진
    g_PrepStartTime = g_NextRushTime - 2000;       // 돌진 전 2초 대기
    g_ShowPrep = false;
    g_RushCount = 0;

    g_InitialDir = -1;
    g_RushDir = g_InitialDir;
    g_LastRandIdx = -1;

    // 미리보기 초기화
    g_ShowTarget = false;
    g_TargetPreviewEnd = 0;
    g_PendingTargetY = 0;
}

void UpdateTurtle(unsigned int now) {
    if (!g_Turtle.alive) return;

    // 피격 초기화(1초)
    if (g_Turtle.isDamaged && now - g_Turtle.lastHitTime >= 1000)
        g_Turtle.isDamaged = false;

    switch (g_State) {
    case TURTLE_STATE_IDLE:
        // 일정 시간이 되면 준비 상태로 전환
        if (now >= g_PrepStartTime) {
            g_State = TURTLE_STATE_PREPARE_RUSH;
            g_PrepStartTime = now;
            g_ShowPrep = true; // 경고 문구 보여줌
        }
        break;

    case TURTLE_STATE_PREPARE_RUSH:
        // 준비 시간(2초)이 지나면 실제 돌진 시작 세팅
        if (now - g_PrepStartTime >= 2000) {
            g_State = TURTLE_STATE_RUSHING;
            g_ShowPrep = false;
            g_RushCount = 0;
            g_RushDir = g_InitialDir; // 사이클의 첫 방향 사용
            g_Turtle.dir = (g_RushDir < 0 ? 1 : 0);
            g_Turtle.pos.y = g_TargetFirstY; // 첫 돌진 Y 고정
            g_Turtle.pos.x = (g_RushDir < 0 ? g_StartX[1] - 20 : g_StartX[0] + 20);
            g_RushEndTime = now + 1000; // 첫 돌진의 기준 시간
        }
        break;

    case TURTLE_STATE_RUSHING: {
        // 미리보기 종료 전까지 이동하지 않음
        if (g_ShowTarget) {
            if (now < g_TargetPreviewEnd) {
                // 플레이어에게 돌진 위치 보여줌
                break;
            }
            else {
                // 자라를 출현 위치로 배치하고 돌진 시작
                g_ShowTarget = false;
                g_Turtle.pos.y = g_PendingTargetY;
                g_Turtle.pos.x = (g_RushDir < 0 ? g_StartX[1] - 20 : g_StartX[0] + 20);
                g_RushEndTime = now + 900; // 이후 돌진의 기준 시간
            }
        }

        // 실제 이동: 속도 * 상수
        int step = (int)(g_Turtle.speed * 3);
        g_Turtle.pos.x += (g_RushDir < 0 ? -step : step);

        // 화면을 완전히 지나갔는지 체크
        bool passed = (g_RushDir < 0)
            ? (g_Turtle.pos.x + TURTLE_WIDTH < 0)
            : (g_Turtle.pos.x > SCREEN_WIDTH);

        if (passed && now >= g_RushEndTime) {
            g_RushCount++;
            if (g_RushCount < TURTLE_RUSH_COUNT) {
                // 다음 돌진 준비
                g_RushDir = -g_RushDir;
                g_Turtle.dir = (g_RushDir < 0 ? 1 : 0);
                int opts = sizeof(g_TargetOptions) / sizeof(int);
                int idx;
                do { idx = randRange(0, opts - 1); } while (idx == g_LastRandIdx && opts > 1);
                g_LastRandIdx = idx;

                // 미리보기
                g_PendingTargetY = g_TargetOptions[idx];
                g_ShowTarget = true;
                g_TargetPreviewEnd = now + 1000; // 1초 미리보기
            }
            else {
                // IDLE로 돌아가고 다음 사이클 준비
                g_State = TURTLE_STATE_IDLE;
                g_Turtle.pos.y = TURTLE_IDLE_Y;

                // 다음 사이클의 첫 돌진 방향을 반전시키기
                g_InitialDir = -g_InitialDir;
                if (g_InitialDir > 0) {
                    g_Turtle.pos.x = g_StartX[0] + 14;
                    g_Turtle.dir = 0;
                }
                else {
                    g_Turtle.pos.x = g_StartX[1] - 22;
                    g_Turtle.dir = 1;
                }

                g_NextRushTime = now + randRange(10000, 15000);
                g_PrepStartTime = g_NextRushTime - 2000;
            }
        }
        break;
    }
    }
}

void DrawTurtle(void) {
    if (!g_Turtle.alive) return;

    if (g_ShowTarget) {
        const char* marker = "!";
        int len = strlen(marker);
        int mx = SCREEN_WIDTH / 2 - len / 2 + GetPlusX();
        int py = g_PendingTargetY - 3; if (py < 0) py = 0;
        _DrawText(mx, py, marker);
    }

    // 돌진 중에 거북이가 화면 밖으로 완전히 나가면 그리지 않음
    if (g_State == TURTLE_STATE_RUSHING) {
        if (g_RushDir > 0 && g_Turtle.pos.x < 0) return;
        if (g_RushDir < 0 && g_Turtle.pos.x + TURTLE_WIDTH > SCREEN_WIDTH) return;
    }

    // 실제 거북이 그리기
    int x = g_Turtle.pos.x - GetPlusX();
    if (g_State == TURTLE_STATE_PREPARE_RUSH) {
        const char* m = "자라가 돌진을 준비중입니다!";
        int l = strlen(m);
        _DrawText(SCREEN_WIDTH / 2 - l / 2 + GetPlusX(), 1, m);
    }

    int idx = (g_Turtle.dir == 0 ? 0 : 1);
    int lines = (g_State == TURTLE_STATE_RUSHING ? TURTLE_HEIGHT - 1 : TURTLE_HEIGHT);
    for (int r = 0; r < lines; ++r) {
        _DrawText(x, g_Turtle.pos.y + r, turtleGraphic[idx][r]);
    }
}