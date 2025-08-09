#pragma once
#include "monster.h"

#define TURTLE_HEIGHT 6     // 자라 높이
#define TURTLE_WIDTH 24     // 자라 너비
#define TURTLE_HP 80        // 자라 체력

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

static const char turtleGraphic[2][TURTLE_HEIGHT][TURTLE_WIDTH] =
{
	{
		"        ________",
		"  ___ _/ \\__/ \\_   /|",
		" (_o / \\ /  \\ / \\_/ |",
		"  \\__ -----------__/",
		"      \\_|_|_|_|_/ ",
		"     /_|_|  /_|_|"
	},
	{
		"        ________",
		"|\\   _/ \\__/ \\_ ___",
		"| \\_/ \\ /  \\ / \\ o_)",
		" \\__----------- __/",
		"    \\_|_|_|_|_/",
		"    |_|_\\  |_|_\\ ",
	}
};

// 자라 스킬
typedef enum TurtleSkill
{
	E_Basic,		// 평타
	E_Rush,			// 돌진
	E_WaterCannon,	// 물대포
	E_SkillCnt,		// 스킬 갯수
} TurtleSkill;

// 자라 구조체
typedef struct Turtle
{
	Monster mon;	// 몬스터 구조체 중첩
	MyPoint pos;	// 위치
	float speed;	// 이동 속도
	int attack;		// 공격력
	bool isDamaged;	// 피격 상태 (무적 여부)
	Direction dir;	// 바라보는 방향
} Turtle;

// 물대포 구조체
typedef struct WaterDrop {
	bool active;
	int x, y;
	unsigned int lastMoveTime;
} WaterDrop;

typedef enum TurtleState {
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

void SetTurtleJumpY(int y); // 점프 높이 설정
void SetTurtleAscentSteps(int steps); // 점프 올라갈 때 단계 수 설정
void SetTurtleDescentSteps(int steps); // 점프 내려갈 때 단계 수 설정
void SetTurtleAscentFrameMs(unsigned int ms); // 점프 올라갈 때 프레임(ms) 설정
void SetTurtleDescentFrameMs(unsigned int ms); // 점프 내려갈 때 프레임(ms) 설정
void SetTurtlePeakHoldMs(unsigned int ms); // 평타 시 공중에서 멈추는 시간 설정

void InitTurtle(unsigned int now); // 자라 초기화
void UpdateTurtle(unsigned int now); // 자라 업데이트
void DrawTurtle(void); // 자라 그리기
void TurtleHitP(int posX, int posY); // 자라가 플레이어를 공격했는지 확인
int GetTurtleHp(); // 자라 체력 가져오기
int SetTurtleHp(); // 자라 체력 초기화