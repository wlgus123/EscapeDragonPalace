#pragma once
#include "init.h"

#define ITEM_SPRITE_ROWS 10     // 아이템 스프라이트 가로 크기
#define ITEM_SPRITE_COLS 20     // 아이템 스프라이트 세로 크기
#define ITEMNUM 2            // 아이템 종류 수
#define MAX_ITEM_COUNT 100   // 맵에 등장할 수 있는 아이템 인스턴스 수
#define SPEEDUP 0.5     // 속도 증가값
#define SPEEDDOWN 0.5 // 속도 감소값
#define LIFEUP 2        // 생명 증가값
#define LIFEDOWN 1      // 생명 감소값

#define SEAWEED_WIDTH 11    // 해초 너비
#define SEAWEED_HEIGHT 3    // 해초 높이
#define BUBBLES_WIDTH 5     // 공기방울 너비
#define BUBBLES_HEIGHT 2    // 공기방울 높이
#define CLAM_WIDTH 6        // 조개 너비
#define CLAM_HEIGHT 1       // 조개 높이

// 아이템 종류 열거형
typedef enum {
    E_ITEM_SPEED,   // 속도 증가
    E_ITEM_LIFE,    // 체력 증가
    E_ITEM_DEBUFF,  // 속도 및 체력 감소 (디버프)
} ItemType;

// 아이템 구조체
typedef struct Item {
    ItemType type;
    float x, y;
    bool isHeld;    // 0: 바닥에 있음, 1: 플레이어가 소지
    float width, height;
    MapStatus mapStatus;
} Item;

extern Item g_ItemList[MAX_ITEM_COUNT]; // 아이템 리스트
extern int g_ItemIdx;   // 아이템 개수

// 함수
void InitItem();
void DrawItem();
void ItemFrameDelay();
