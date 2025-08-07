#pragma once

#define TURTLE_HEIGHT      6
#define TURTLE_WIDTH       13
#define TURTLE_HP          80

// 돌진 횟수 (한 사이클에 몇 번 돌진할지)
#define TURTLE_RUSH_COUNT  3
// Idle 상태일 때 거북이 Y 좌표
#define TURTLE_IDLE_Y      18

// 돌진 옵션 개수 (첫 돌진 제외)
#define TURTLE_RUSH_OPTION_COUNT 3

// 첫 돌진 Y좌표 (Idle보다 1칸 위)
extern int g_TargetFirstY;
// 이후 돌진에서 사용할 Y 옵션들 (랜덤 선택)
extern int g_TargetOptions[TURTLE_RUSH_OPTION_COUNT];
// 출현 X좌표: [0]=왼쪽 시작, [1]=오른쪽 시작
extern int g_StartX[2];

typedef struct { int x, y; } Point;

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

//돌진 상태
typedef enum {
    TURTLE_STATE_IDLE, // 평시 상태
    TURTLE_STATE_PREPARE_RUSH, // 돌진 준비
    TURTLE_STATE_RUSHING // 돌진중
} TurtleState;

extern Turtle g_Turtle;

void InitTurtle(unsigned int now);
void UpdateTurtle(unsigned int now);
void DrawTurtle(void);
