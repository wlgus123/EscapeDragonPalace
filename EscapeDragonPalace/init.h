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
#define SCREEN_WIDTH 80 // 화면 너비
#define SCREEN_HEIGHT 25 // 화면 높이
#define STR_MAX 256 // 문자열 최대 크기

// 구조체
typedef struct MyPoint
{
    float x;
    float y;
} MyPoint;

// 열거형
// 방향
typedef enum {
    E_RIGHT,
    E_LEFT,
} Direction;

// 전역 변수
int g_Key;