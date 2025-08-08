#pragma once

#define TURTLE_HEIGHT      6
#define TURTLE_WIDTH       12
#define TURTLE_HP          80

// 돌진 횟수
#define TURTLE_RUSH_COUNT  3
// 평상시 Y좌표
#define TURTLE_IDLE_Y      18

// 물방울 생성 간격 (500~700ms)
#define WATER_DROP_INTERVAL_MIN_MS 500
#define WATER_DROP_INTERVAL_MAX_MS 700
#define MAX_WATER_DROPS 64
#define WATER_DROP_MOVE_MS 150

// 물방울 지속시간 (3~5초)
#define WATER_WAVE_DURATION_MIN_MS 3000
#define WATER_WAVE_DURATION_MAX_MS 5000

// 물방울 쿨타임 (8~12초)
#define WATER_WAVE_COOLDOWN_MIN_MS 8000
#define WATER_WAVE_COOLDOWN_MAX_MS 12000

// 자라 머리 기준 좌우 ±범위
#define TURTLE_ATTACK_RANGE 7

// 평타 쿨타임 (5초)
#define TURTLE_ATTACK_COOLDOWN_MS 5000

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

typedef enum {
    TURTLE_STATE_IDLE,          // 평상시 자라
    TURTLE_STATE_PREPARE_RUSH,  // 돌진 준비
    TURTLE_STATE_RUSHING,       // 돌진 중
    TURTLE_STATE_ATTACK
} TurtleState;

extern Turtle g_Turtle;

// 점프 높이(평타)
extern int g_NormalJumpY;

// 점프 올라갈 때 / 내려올 때 단계 수, 프레임(ms)
extern int g_TurtleAscentSteps;
extern int g_TurtleDescentSteps;
extern unsigned int g_TurtleAscentFrameMs;
extern unsigned int g_TurtleDescentFrameMs;

// 평타 시 공중에서 멈추는 시간
extern unsigned int g_TurtlePeakHoldMs;

void SetTurtleJumpY(int y);
void SetTurtleAscentSteps(int steps);
void SetTurtleDescentSteps(int steps);
void SetTurtleAscentFrameMs(unsigned int ms);
void SetTurtleDescentFrameMs(unsigned int ms);
void SetTurtlePeakHoldMs(unsigned int ms);

void InitTurtle(unsigned int now);
void UpdateTurtle(unsigned int now);
void DrawTurtle(void);
