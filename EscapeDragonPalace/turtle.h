#pragma once

#define TURTLE_HEIGHT      6
#define TURTLE_WIDTH       12
#define TURTLE_HP          80

// 돌진 횟수
#define TURTLE_RUSH_COUNT  3
// Idle Y좌표
#define TURTLE_IDLE_Y      18

#define WATER_DROP_INTERVAL_MIN_MS 500
#define WATER_DROP_INTERVAL_MAX_MS 700
#define MAX_WATER_DROPS 64
#define WATER_DROP_MOVE_MS 150

typedef struct { int x, y; } Point;

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

// 거북이 구조체
typedef struct {
    Point pos;
    float speed;
    int dir;
    int hp;
    int attack;
    bool alive;
    bool isDamaged;
    unsigned int lastHitTime;
} Turtle;

// 상태 정의
typedef enum {
    TURTLE_STATE_IDLE,
    TURTLE_STATE_PREPARE_RUSH,
    TURTLE_STATE_RUSHING
} TurtleState;

extern Turtle g_Turtle;

// 돌진 Y좌표 배열
extern int g_TargetYs[TURTLE_RUSH_COUNT];
// 돌진 시작 X좌표
extern int g_StartX[2];

void InitTurtle(unsigned int now);
void UpdateTurtle(unsigned int now);
void DrawTurtle(void);
void TurtleHitP(int posX, int posY);