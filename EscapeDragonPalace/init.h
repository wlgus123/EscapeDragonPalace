#pragma once

// inlcude
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include "mtg.h" // mtg 파일 불러오기

// 메크로
#define STAGE_CNT 5 // 스테이지 갯수
#define SCREEN_WIDTH 80 // 화면 너비
#define SCREEN_HEIGHT 25 // 화면 높이
#define STR_MAX 256 // 문자열 최대 크기

// 구조체
typedef struct MyPoint
{
    float x;
    float y;
} MyPoint;

typedef struct Rect {
	float x, y, w, h;
} Rect;

// 열거형
// 방향
typedef enum {
    E_Right,
    E_Left,
} Direction;

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

// 맵
typedef enum MapStatus
{
	E_Jail, // 감옥
	E_DragonPalace, // 용궁
	E_Sea1, // 바다1
	E_Sea2, // 바다2
	E_Ground, // 육지(보스)
} MapStatus;

// 전역 변수
int g_Key;	// 입력 키
static bool isInit = false; // 초기화 여부