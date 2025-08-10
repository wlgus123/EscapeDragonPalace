#pragma once
#include "init.h"

#define ITEM_SPRITE_ROWS 10     // 아이템 스프라이트 가로 크기
#define ITEM_SPRITE_COLS 20     // 아이템 스프라이트 세로 크기

#define ITEMNUM 2               // 아이템 종류 수
#define MAX_ITEM_COUNT 100      // 맵에 등장할 수 있는 아이템 인스턴스 수

#define SPEEDUP 0.5     // 속도 증가값
#define SPEEDDOWN 0.5   // 속도 감소값
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
    ItemType type;          // 아이템 종류
	float x, y;             // 아이템 위치 좌표
    bool isHeld;            // 플레이어 소지 여부
	float width, height;    // 아이템 너비, 높이
	MapStatus mapStatus;    // 아이템이 등장하는 맵
} Item;

extern Item g_ItemList[MAX_ITEM_COUNT]; // 아이템 리스트
extern int g_ItemIdx;   // 아이템 개수

// 함수
void InitItem();        // 아이템 초기화
void DrawItem();        // 아이템 그리기
void ResetItem();       // 아이템 isHeld false로 초기화
void ItemFrameDelay();  // 아이템 모션 효과
