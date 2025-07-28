#pragma once
#include "init.h"

// 매크로
#define STAGE_CNT 4 // 일반 스테이지 갯수
#define BG_CNT 5 // 맵 배경 요소 갯수
#define BG_LINE 20 // 맵 배경 줄 수
#define PLATFORM_CNT 10 // 발판 갯수
#define MAP_HEIGHT 49 // 맵 높이
#define MAP_WIDTH SCREEN_WIDTH // 맵 너비
#define GOAL_HEIGHT 5 // 도착지점 높이

void Draw();
void DrawNormalMap(); // 일반 맵 그리기
void DrawNormalBG(); // 일반 맵 배경 그리기
void DrawBossMap(); // 보스 맵 그리기
void UpdateMapPos();
int GetMapStatus(); // 맵 정보 받아오기

// CMD 색깔
typedef enum CMDColor
{
	E_Black,
	E_Blue,
	E_Green,
	E_Teal, // 청록색
	E_Red,
	E_Purple,
	E_Yellow,
	E_White,
	E_Gray,
	E_BrightBlue,
	E_BrightGreen,
	E_BrightTeal,
	E_BrightRed,
	E_BrightPurple,
	E_BrightYellow,
	E_BrightWhite,
} CMDColor;

typedef enum MapBgColor
{
	E_CJail = E_Gray,
	E_CDragonPalace = E_BrightBlue,
	E_CSea1 = E_Blue,
	E_CSea2 = E_Blue,
	E_CGround = E_Green,
} MapBgColor;

typedef enum MapStatus
{
	E_Jail,
	E_DragonPalace,
	E_Sea1,
	E_Sea2,
	E_Boss,
} MapStatus;

typedef struct StageBG
{
	MyPoint pos;
	CMDColor color;
	char bg[20][256];
} StageBG;