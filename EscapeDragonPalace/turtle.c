#include "init.h"
#include "turtle.h"
#include "map.h"
#include "Rabbit.h"

Turtle g_Turtle;

static TurtleState g_State;


static unsigned int g_NextRushTime, g_PrepStartTime, g_RushEndTime; // 다음 돌진,준비,종료
static bool g_ShowPrep;    // 돌진 준비 메시지 화면출력 여부
static int g_RushCount;    // 돌진 3번 카운트
static int g_RushDir;      // 돌진 방향
static int g_InitialDir;   // 맨 처음 돌진 방향 오->왼 첫 돌진 종료 후 자라가 왼쪽에 있을땐 왼->오
static int g_LastRandIdx;  // 돌진 중복발판 금지용

static bool g_ShowTarget = false;            //미리보기 표시 여부
static unsigned int g_TargetPreviewEnd = 0;  //미리보기 종료 
static int g_PendingTargetY = 0; // 돌진할 발판 Y 좌표 (미리보기용)

int g_TargetFirstY = TURTLE_IDLE_Y + 1;             // 다리 없애기때문에 Y좌표 +1
int g_TargetOptions[] = { 2, 10, 18 };               // 자라가 돌진할때 발판 좌표
int g_StartX[2] = { -TURTLE_WIDTH, SCREEN_WIDTH };  // 시작 X 좌표 ( 오른쪽 , 왼쪽 ) 

static bool g_WaterActive = false;           // 물방울 활성 비활성
static unsigned int g_NextWaveStartTime = 0; // 다음 물방울 시작
static bool g_WaveActive = false;            // 물방울 진행중인지 확인
static unsigned int g_WaveEndTime = 0;       // 현재 물방울 종료
static unsigned int g_NextDropSpawnTime = 0; // 물방울 동안 다음 물방울 생성 시각
static char g_WaterChar = '@';

static bool g_WaveWarnActive = false;         // 물방울 시작 2초 전 경고 활성여부
static unsigned int g_WaveWarnStart = 0;      // 경고가 켜진 시각
static unsigned int g_WaveWarnDelayUntil = 0; // 경고가 꺼지는 시각

// 물방울이 모두 바닥에 닿을 때까지 스킬 사용 막기
static bool g_WaveDropsBlocking = false;

// === 평타 범위 표시 설정 (사용자가 쉽게 조정할 수 있도록 전역 변수로 분리) ===
static const int ATTACK_HALF = 7;           // 범위 반폭
static const int ATTACK_CENTER_RIGHT = 58;  // 오른쪽 기준 중심 X (월드 좌표)
static const int ATTACK_CENTER_LEFT = 22;   // 왼쪽 기준 중심 X (월드 좌표)
// 범위 표시 Y 위치
static const int ATTACK_RANGE_Y_OFFSET = 5;

static WaterDrop g_WaterDrops[MAX_WATER_DROPS];

static void ClearWaterDrops(void) {
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        g_WaterDrops[i].active = false;
        g_WaterDrops[i].lastMoveTime = 0;
        g_WaterDrops[i].x = 0;
        g_WaterDrops[i].y = 0;
    }
}

static void SpawnWaterDropAt(unsigned int now) {
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
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
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        if (!g_WaterDrops[i].active) continue;

        // 이동 주기에 따라 한 칸씩 아래로 이동
        if (now - g_WaterDrops[i].lastMoveTime >= WATER_DROP_MOVE_MS) {
            g_WaterDrops[i].y += 1;
            g_WaterDrops[i].lastMoveTime = now;
            // 화면 아래로 벗어나면 제거 (바닥에 떨어지면 제거)
            if (g_WaterDrops[i].y > SCREEN_HEIGHT + 5) g_WaterDrops[i].active = false;
        }
    }
}

// 활성화된 물방울이 하나라도 있는지 체크
static bool AnyWaterDropsActive(void) {
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        if (g_WaterDrops[i].active) return true;
    }
    return false;
}

static void DrawWaterDrops(void) {
    char s[2] = { g_WaterChar, '\0' };

    // 물방울은 파란색으로 그리기
    _SetColor(E_Blue);
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        if (!g_WaterDrops[i].active) continue;
        int dropX = g_WaterDrops[i].x;
        int dropY = g_WaterDrops[i].y;

        if (g_Turtle.mon.alive) {
            int tLeft = g_Turtle.pos.x;
            int tTop = g_Turtle.pos.y;
            int tLines = (g_State == TURTLE_STATE_RUSHING ? TURTLE_HEIGHT - 1 : TURTLE_HEIGHT);
            int tRight = tLeft + TURTLE_WIDTH - 1;
            int tBottom = tTop + tLines - 1;
            if (dropX >= tLeft && dropX <= tRight && dropY >= tTop && dropY <= tBottom) {
                // 자라 영역에 있는 물방울은 표시하지 않음 (시각적 충돌 회피)
                continue;
            }
        }

        int dx = dropX - GetPlusX();
        int dy = dropY;
        _DrawText(dx, dy, s);
    }
    //자라가 물방울 맞으면 색이 변하니 흰색으로 초기화
    _SetColor(E_White);
}


static unsigned int randRange(unsigned min, unsigned max) { return min + rand() % (max - min + 1); }

int g_NormalJumpY = 4;            // 점프 높이 (y 단위)

// 단계 수: 올라갈 때 / 내려갈 때 (각각 >=1)
int g_TurtleAscentSteps = 2;      // base -> mid -> peak
int g_TurtleDescentSteps = 2;     // peak -> mid -> base

// 올라갈때 / 내려갈때 프레임 (ms)
unsigned int g_TurtleAscentFrameMs = 100;
unsigned int g_TurtleDescentFrameMs = 10;

// 점프 위에서 멈추는 시간(ms).
unsigned int g_TurtlePeakHoldMs = 1000;

static bool g_ExclaimActive = false;        // ! 활성화 여부
static unsigned int g_ExclaimStart = 0;     // !가 켜진 시간
static unsigned int g_LastExclaimBlink = 0; // 마지막 깜빡임 시간
static bool g_ExclaimVisible = false;       // 현재 ! 깜빡임 상태

// 평타 실행 상태 변수들
static bool g_JumpActive = false;
static unsigned int g_JumpStartTime = 0;
static unsigned int g_LastJumpFrameTime = 0;
static int g_JumpPhase = 0;
static int g_JumpBaseY = TURTLE_IDLE_Y;

static unsigned int g_LastAttackTime = 0; // 마지막 평타 종료 시간 (쿨다운 체크)

static unsigned int g_PeakHoldUntil = 0;

static unsigned int g_SkillLockUntil = 0;       // 이 시각 이전까지 돌진 사용 금지
static const unsigned int g_SkillLockMs = 1000; // 1초 락

extern Rect GetPlayerRect(void);

void SetTurtleJumpY(int y) {
    if (y < 0) y = 0;
    g_NormalJumpY = y;
}
void SetTurtleAscentSteps(int steps) {
    if (steps < 1) steps = 1;
    g_TurtleAscentSteps = steps;
}
void SetTurtleDescentSteps(int steps) {
    if (steps < 1) steps = 1;
    g_TurtleDescentSteps = steps;
}
void SetTurtleAscentFrameMs(unsigned int ms) {
    if (ms < 10) ms = 10;
    g_TurtleAscentFrameMs = ms;
}
void SetTurtleDescentFrameMs(unsigned int ms) {
    if (ms < 10) ms = 10;
    g_TurtleDescentFrameMs = ms;
}
void SetTurtlePeakHoldMs(unsigned int ms) {
    g_TurtlePeakHoldMs = ms;
}

void InitTurtle(unsigned int now) {
    srand((unsigned)time(NULL));
    g_Turtle.pos.x = 58;
    g_Turtle.pos.y = TURTLE_IDLE_Y;
    g_Turtle.speed = 1.0f;
    g_Turtle.dir = E_Right;
    g_Turtle.mon.hp = TURTLE_HP;
    g_Turtle.attack = 2;
    g_Turtle.mon.alive = true;
    g_Turtle.isDamaged = false;
    g_Turtle.mon.lastHitTime = now;
    g_State = TURTLE_STATE_IDLE;
    g_NextRushTime = now + randRange(10000, 15000);
    g_PrepStartTime = g_NextRushTime - 2000;
    g_ShowPrep = false;
    g_RushCount = 0;
    g_InitialDir = E_Right;
    g_RushDir = g_InitialDir;
    g_LastRandIdx = -1;

    g_ShowTarget = false;
    g_TargetPreviewEnd = 0;
    g_PendingTargetY = 0;

    // 물방울 시스템 초기화
    g_WaterActive = true;
    ClearWaterDrops();
    // 물방울 시작 시각: 8~12초 랜덤 쿨타임 후 첫 물방울 시작
    g_WaveActive = false;
    g_NextWaveStartTime = now + randRange(WATER_WAVE_COOLDOWN_MIN_MS, WATER_WAVE_COOLDOWN_MAX_MS);
    g_WaveEndTime = 0;
    g_NextDropSpawnTime = 0;

    // 물방울 경고 초기화
    g_WaveWarnActive = false;
    g_WaveWarnStart = 0;
    g_WaveWarnDelayUntil = 0;
    g_WaveDropsBlocking = false;

    // 평타 초기 상태
    g_ExclaimActive = false;
    g_ExclaimVisible = false;
    g_JumpActive = false;
    g_JumpPhase = 0;
    g_JumpBaseY = TURTLE_IDLE_Y;
    g_LastAttackTime = now; // 초기에는 쿨타임 검사 기준 시각
    g_PeakHoldUntil = 0;

    // 스킬 락 초기화
    g_SkillLockUntil = now;
}

//플레이어 근처 감지
static bool IsPlayerNear(void) {
    Rect pr = GetPlayerRect();
    int playerLeft = pr.x;
    int playerRight = pr.x + pr.w - 1;

    // 자라의 머리 화면 X 좌표 계산
    int turtleScreenX = g_Turtle.pos.x - GetPlusX();
    int headOffset = (g_Turtle.dir != E_Right) ? E_Left : 0; // dir==0: 오른쪽 바라봄 dir!=0 : 왼쪽 바라봄
    int headScreenX = turtleScreenX + headOffset;

    if (playerRight < headScreenX - TURTLE_ATTACK_RANGE) return false;
    if (playerLeft > headScreenX + TURTLE_ATTACK_RANGE) return false;
    return true;
}

// 평타 올라가는거 및 내려가는거
static int ComputeJumpYForPhase(int phase, int baseY) {
	int ascent = g_TurtleAscentSteps;   // 올라가는 단계 수
	int descent = g_TurtleDescentSteps; // 내려가는 단계 수
	if (ascent < 1) ascent = 1;     // 올라가는 단계 수가 1보다 작으면 1로 설정
	if (descent < 1) descent = 1;   // 내려가는 단계 수가 1보다 작으면 1로 설정
	int total = ascent + descent;   // 전체 단계 수
	if (phase < 1) phase = 1;       // phase가 1보다 작으면 1로 설정
	if (phase > total) phase = total;   // phase가 전체 단계 수보다 크면 전체 단계 수로 설정
	int jumpY = g_NormalJumpY;  // 점프 높이 (y 단위)
	int peakY = baseY - jumpY;  // 최고점 Y 좌표

    if (phase <= ascent) {
        int k = phase;
        int yOffset = (jumpY * k + (ascent / 2)) / ascent;
        return baseY - yOffset;
    }
    else {
        int d = phase - ascent;
        int yOffset = (jumpY * d + (descent / 2)) / descent;
        return peakY + yOffset;
    }
}

void UpdateTurtle(unsigned int now) {
	if (!g_Turtle.mon.alive) return;    // 자라가 죽으면 업데이트 중지
	if (g_Turtle.isDamaged && now - g_Turtle.mon.lastHitTime >= 1000) // 1초 후 무적 해제
        g_Turtle.isDamaged = false;

    // 돌진 시작하는 경우 평타/웨이브 충돌 방지용
    const unsigned int PREP_MARGIN_MS = 50;
    bool prepImminent = (g_PrepStartTime <= now + PREP_MARGIN_MS);

    // 스킬 락 확인: 물방울이 다 사라질 때까지 스킬 금지
    bool skillLocked = (now < g_SkillLockUntil) || g_WaveDropsBlocking;

    // 평상시 자라일 때
    if (g_State == TURTLE_STATE_IDLE) {
        // 평타
		if (!g_ExclaimActive && !g_JumpActive) {
            // 평타 시작 조건:
            // - 돌진 임박/돌진 중 금지
            // - 스킬 락 해제 (단, 경고가 뜨면 평타를 지연시킴)
            // (물방울 웨이브가 있어도 평타 시작 가능하게 변경)
            if (!prepImminent && !skillLocked && IsPlayerNear() && (now - g_LastAttackTime >= TURTLE_ATTACK_COOLDOWN_MS)) {
                if (g_WaveWarnActive && now < g_WaveWarnDelayUntil) {
                    g_ExclaimActive = true;
                    g_ExclaimStart = g_WaveWarnDelayUntil; // 경고 끝난 후 1초 이후 준비 시작
                    g_LastExclaimBlink = g_ExclaimStart;
                    g_ExclaimVisible = true;
                }
                else {
                    g_ExclaimActive = true;
                    g_ExclaimStart = now;
                    g_LastExclaimBlink = now;
                    g_ExclaimVisible = true;
                }

                if (g_PrepStartTime <= g_ExclaimStart + 500) {
                    g_PrepStartTime = g_ExclaimStart + 500;
                }
            }
        }

		if (g_ExclaimActive) {  // 느낌표 활성화 상태
            if (now >= g_ExclaimStart) {
                // 느낌표 활성화 시간 이후에만 깜빡이게 함
                if (now - g_LastExclaimBlink >= 300) {
                    g_ExclaimVisible = !g_ExclaimVisible;
                    g_LastExclaimBlink = now;
                }
                if (now - g_ExclaimStart >= 2000) {
                    g_ExclaimActive = false;
                    g_ExclaimVisible = false;

                    // 점프 시작
                    g_JumpActive = true;
                    g_JumpStartTime = now;
                    g_LastJumpFrameTime = now;
                    g_JumpPhase = 0;
                    g_JumpBaseY = g_Turtle.pos.y;
                    g_PeakHoldUntil = 0;
                }
            }
            else {
                // 경고가 끝나면 실행
                g_ExclaimVisible = false;
            }
        }
    }

    // 물방울
    if (g_WaterActive) {
        if (!g_WaveActive) {
            // 물방울 시작 예정인지 체크
            if (now >= g_NextWaveStartTime) {
                // 돌진 준비/돌진 상태에서는 시작 금지
                if (g_State != TURTLE_STATE_PREPARE_RUSH && g_State != TURTLE_STATE_RUSHING && !skillLocked) {
                    // 시작
                    g_WaveActive = true;
                    g_WaveEndTime = now + randRange(WATER_WAVE_DURATION_MIN_MS, WATER_WAVE_DURATION_MAX_MS);
                    g_NextDropSpawnTime = now;
                    // 경고 종료
                    g_WaveWarnActive = false;
                    g_WaveWarnDelayUntil = 0;
                    // 물방울 끝나면 활성화
                    g_WaveDropsBlocking = false;
                }
                else {
                    g_NextWaveStartTime = now + 500;
                }
            }
            else {
                // 2초 전 경고 체크
                if (!g_WaveWarnActive) {
                    // 경고를 켤 때는 돌진 관련 상태가 아니고 스킬락도 아니어야 함
                    if (g_State != TURTLE_STATE_PREPARE_RUSH && g_State != TURTLE_STATE_RUSHING && !skillLocked
                        && g_NextWaveStartTime > now && (g_NextWaveStartTime - now) <= 2000) {
                        g_WaveWarnActive = true;
                        g_WaveWarnStart = now;
                        g_WaveWarnDelayUntil = now + 1000;
                    }
                }
                else {
                    // 만약 자라가 돌진 관련 상태가 되거나 스킬락이면 경고 취소
                    if (g_State == TURTLE_STATE_PREPARE_RUSH || g_State == TURTLE_STATE_RUSHING || skillLocked) {
                        g_WaveWarnActive = false;
                        g_WaveWarnDelayUntil = 0;
                    }
                }
            }
        }
        else {
            // 스폰 간격에 따라 물방울 생성
            if (now >= g_NextDropSpawnTime) {
                SpawnWaterDropAt(now);
                // 다음 스폰 시간은 500~700ms 랜덤
                g_NextDropSpawnTime = now + randRange(WATER_DROP_INTERVAL_MIN_MS, WATER_DROP_INTERVAL_MAX_MS);
            }
            // 웨이브 종료 체크
            if (now >= g_WaveEndTime) {
                // 물방울은 비활성화하지만 화면에 남은 물방울이 모두 사라질 때까지
                // 다른 스킬을 못 쓰도록 하기
                g_WaveActive = false;
                g_WaveWarnActive = false;
                g_WaveWarnDelayUntil = 0;
                if (AnyWaterDropsActive()) {
                    g_WaveDropsBlocking = true;
                }
                else {
                    if (g_SkillLockUntil < now + g_SkillLockMs) g_SkillLockUntil = now + g_SkillLockMs;
                    g_WaveDropsBlocking = false;
                }
                // 다음 물방울 시작 시간: 8~12초 랜덤 쿨타임
                g_NextWaveStartTime = now + randRange(WATER_WAVE_COOLDOWN_MIN_MS, WATER_WAVE_COOLDOWN_MAX_MS);
            }
        }
    }

    // 물방울 이동 처리 (항상 수행)
    UpdateWaterDrops(now);

    // 만약 모든 물방울이 바닥에 떨어지면 해제 & 소규모 락 적용
    if (g_WaveDropsBlocking) {
        if (!AnyWaterDropsActive()) {
            g_WaveDropsBlocking = false;
            if (g_SkillLockUntil < now + g_SkillLockMs) g_SkillLockUntil = now + g_SkillLockMs;
        }
    }

	// 평타 점프 처리
    if (g_JumpActive) {
        int ascent = (g_TurtleAscentSteps >= 1 ? g_TurtleAscentSteps : 1);
        int descent = (g_TurtleDescentSteps >= 1 ? g_TurtleDescentSteps : 1);
        int total = ascent + descent;

        if (g_JumpPhase == ascent && g_PeakHoldUntil != 0 && now < g_PeakHoldUntil) {
        }
        else {
            int nextPhase = g_JumpPhase + 1;
            unsigned int curFrameMs;
            if (nextPhase <= ascent) curFrameMs = g_TurtleAscentFrameMs;
            else curFrameMs = g_TurtleDescentFrameMs;

            if (now - g_LastJumpFrameTime >= curFrameMs) {
                g_LastJumpFrameTime = now;
                g_JumpPhase++;

                if (g_JumpPhase > total) {
                    g_JumpActive = false;
                    g_JumpPhase = 0;
                    g_Turtle.pos.y = g_JumpBaseY;
                    g_LastAttackTime = now; // 평타 끝났을 때 쿨다운 시작
                    g_PeakHoldUntil = 0;
                    if (g_SkillLockUntil < now + g_SkillLockMs) g_SkillLockUntil = now + g_SkillLockMs;
                }
                else {
                    int newY = ComputeJumpYForPhase(g_JumpPhase, g_JumpBaseY);
                    g_Turtle.pos.y = newY;

                    if (g_JumpPhase == ascent && g_TurtlePeakHoldMs > 0) {
                        g_PeakHoldUntil = now + g_TurtlePeakHoldMs;
                    }
                }
            }
        }
    }

    // 돌진
    switch (g_State) {
    case TURTLE_STATE_IDLE: // 평상시일 경우
        // 돌진 준비 시작 조건:
        // 물방울 비활성
        // 경고 비활성
        // 평타 비활성
        // 스킬락 해제
        if (now >= g_PrepStartTime && !g_WaveActive && !g_WaveWarnActive && !g_ExclaimActive && !g_JumpActive && !skillLocked) {
            // 돌진 준비로 넘어갈 때: 평타/경고/물방울 상태 초기화 (겹침 방지)
            g_ExclaimActive = false;
            g_ExclaimVisible = false;
            g_JumpActive = false;
            g_PeakHoldUntil = 0;
            g_WaveWarnActive = false;
            g_WaveWarnDelayUntil = 0;

            g_State = TURTLE_STATE_PREPARE_RUSH;
            g_PrepStartTime = now;
            g_ShowPrep = true;
        }
        break;
    case TURTLE_STATE_PREPARE_RUSH: // 돌진 준비 중일 경우
        if (now - g_PrepStartTime >= 2000) {
            // 돌진 준비 -> 돌진중 전환 시에도 평타/느낌표/경고 초기화
            g_ExclaimActive = false;
            g_ExclaimVisible = false;
            g_JumpActive = false;
            g_PeakHoldUntil = 0;
            g_WaveWarnActive = false;
            g_WaveWarnDelayUntil = 0;

            g_State = TURTLE_STATE_RUSHING;
            g_ShowPrep = false;
            g_RushCount = 0;
            g_RushDir = g_InitialDir;
            g_Turtle.dir = (g_RushDir < E_Right ? E_Left : E_Right);
            g_Turtle.pos.y = g_TargetFirstY;
            g_Turtle.pos.x = (g_RushDir < E_Right ? g_StartX[E_Left] - 20 : g_StartX[E_Right] + 20);
            g_RushEndTime = now + 1000;
        }
        break;
    case TURTLE_STATE_RUSHING: {    // 돌진 중일 경우
        if (g_ShowTarget) {
            if (now < g_TargetPreviewEnd) {
                break;
            }
            else {
                g_ShowTarget = false;
                g_Turtle.pos.y = g_PendingTargetY;
                g_Turtle.pos.x = (g_RushDir < E_Right ? g_StartX[E_Left] - 20 : g_StartX[E_Right] + 20);
                g_RushEndTime = now + 900;
            }
        }

        int step = (int)(g_Turtle.speed * 3);
        g_Turtle.pos.x += (g_RushDir < E_Right ? -step : step);
        bool passed = (g_RushDir < E_Right)
            ? (g_Turtle.pos.x + TURTLE_WIDTH < 0)
            : (g_Turtle.pos.x > SCREEN_WIDTH);
        if (passed && now >= g_RushEndTime) {
            g_RushCount++;
            if (g_RushCount < TURTLE_RUSH_COUNT) {
                g_RushDir = -g_RushDir;
                g_Turtle.dir = (g_RushDir < E_Right ? E_Left : E_Right);
                int opts = sizeof(g_TargetOptions) / sizeof(int);
                int idx;
                do { idx = randRange(0, opts - 1); } while (idx == g_LastRandIdx && opts > 1);
                g_LastRandIdx = idx;

                // 돌진 할 발판에 ! 표시
                g_PendingTargetY = g_TargetOptions[idx];
                g_ShowTarget = true;
                g_TargetPreviewEnd = now + 1000; // 1초 미리보기

            }
            else {
                if (g_SkillLockUntil < now + g_SkillLockMs) g_SkillLockUntil = now + g_SkillLockMs;

                // 돌진 끝날 때도 평타/느낌표/경고/웨이브가 동작하지 않도록 확실히 초기화
                g_ExclaimActive = false;
                g_ExclaimVisible = false;
                g_JumpActive = false;
                g_PeakHoldUntil = 0;
                g_WaveWarnActive = false;
                g_WaveWarnDelayUntil = 0;
                g_WaveActive = false;

                g_State = TURTLE_STATE_IDLE;
                g_Turtle.pos.y = TURTLE_IDLE_Y;

                g_InitialDir = -g_InitialDir;

                if (g_InitialDir > E_Right) {
                    g_Turtle.pos.x = g_StartX[E_Right] + 14;
                    g_Turtle.dir = E_Right;
                }
                else {
                    g_Turtle.pos.x = g_StartX[E_Left] - 22;
                    g_Turtle.dir = E_Left;
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
    if (!g_Turtle.mon.alive) return;

    // 물방울 그리기
    if (g_WaterActive) DrawWaterDrops();

    // 물방울 경고 표시
    if (g_WaveWarnActive) {
        const char* warn = "물방울이 떨어집니다!";
        int len = strlen(warn);
        _DrawText(SCREEN_WIDTH / 2 - len / 2 + GetPlusX(), 2, warn);
    }

    if (g_ShowTarget) {
        int mx = SCREEN_WIDTH / 2 - 3 + GetPlusX();
        _DrawText(mx + 3, g_PendingTargetY + 3, "!");
    }

    // 느낌표 깜빡일때만 범위 표시
    if (g_ExclaimActive && g_ExclaimVisible && !g_JumpActive) {
        int centerWorld = (g_Turtle.dir == E_Right) ? ATTACK_CENTER_RIGHT : ATTACK_CENTER_LEFT;
        int leftWorld = centerWorld - ATTACK_HALF;
        int rightWorld = centerWorld + ATTACK_HALF;

        int leftScreen = leftWorld - GetPlusX();
        int rightScreen = rightWorld - GetPlusX();

        if (leftScreen < 0) leftScreen = 0;
        if (rightScreen >= SCREEN_WIDTH) rightScreen = SCREEN_WIDTH - 1;
        if (rightScreen < leftScreen) rightScreen = leftScreen;

        int rangeY = g_Turtle.pos.y + ATTACK_RANGE_Y_OFFSET;
        if (rangeY < 0) rangeY = 0;
        if (rangeY >= SCREEN_HEIGHT) rangeY = g_Turtle.pos.y;

        char sRange[2] = { '-', '\0' };
        for (int sx = leftScreen; sx <= rightScreen; ++sx) {
            _DrawText(sx, rangeY, sRange);
        }
        _DrawText(leftScreen, rangeY, "[");
        _DrawText(rightScreen, rangeY, "]");
    }

    // 평타 준비 ( ! )
    if (g_State == TURTLE_STATE_IDLE && g_ExclaimActive && g_ExclaimVisible) {
        int exX = (g_Turtle.pos.x - GetPlusX()) + (TURTLE_WIDTH / 2);
        int exY = g_Turtle.pos.y - 1;
        _DrawText(exX, exY, "!");
    }

    // 돌진 중에 자라가 화면 밖으로 완전히 나가면 그리지 않음
    if (g_State == TURTLE_STATE_RUSHING &&
        (g_Turtle.pos.x < 5 || g_Turtle.pos.x + TURTLE_WIDTH > 75)) return;


    int x = g_Turtle.pos.x - GetPlusX();
    if (g_State == TURTLE_STATE_PREPARE_RUSH) {
        const char* m = "자라가 돌진을 준비중입니다!";
        int l = strlen(m);
        _DrawText(SCREEN_WIDTH / 2 - l / 2 + GetPlusX(), 1, m);
    }
    int idx = (g_Turtle.dir == E_Right ? E_Right : E_Left);
    int lines = (g_State == TURTLE_STATE_RUSHING ? TURTLE_HEIGHT - 1 : TURTLE_HEIGHT);
    for (int r = 0; r < lines; ++r) _DrawText(x, g_Turtle.pos.y + r, turtleGraphic[idx][r]);

    TurtleHitP(g_Turtle.pos.x, g_Turtle.pos.y); //여기서 거북이 좌표를 받아옴
}

void TurtleHitP(int posX, int posY) { //닿으면 2씩 닳음
    Rect PlayerPos = GetPlayerRect();
    Rect MosterPos = { posX, posY, 58, TURTLE_IDLE_Y };
    DWORD now = GetTickCount();

    if ((IsOverlap(PlayerPos, MosterPos)) == false)
        return;

    // 무적 시간 체크
    if (now - g_Turtle.mon.lastHitTime < INVINCIBLE_TIME) {
        return; // 아직 무적 상태면 데미지 무시
    }

    // 자라가 돌진 중일 때
	if (g_State == TURTLE_STATE_RUSHING) {
        // 4칸 닳음
		player.Health -= 4; // 플레이어 체력 감소
	}
    else
    {
        player.Health -= 1;
    }


    g_Turtle.mon.lastHitTime = now; // 마지막 피격 시간 갱신
}
//자라 그려둔 부분은 헤더로 옮김
//아마 물방울 닿으면 피가 감소되는 것도 이 코드 부분 보고 해주면 될 것 같음