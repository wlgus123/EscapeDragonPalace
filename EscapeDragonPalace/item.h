#pragma once

#include "map.h"

#define ITEM_SPRITE_ROWS 10
#define ITEM_SPRITE_COLS 20
#define ITEMNUM 2            // 아이템 종류 수
#define MAX_ITEM_COUNT 100   // 맵에 등장할 수 있는 아이템 인스턴스 수
#define SPEEDUP 1     // 속도 증가값
#define LIFEUP 2        // 생명 증가값

typedef enum {
    E_ITEM_SPEED,
    E_ITEM_LIFE
} ItemType;

typedef struct Item {
    ItemType type;
    float x, y;
    bool isHeld;    // 0: 바닥에 있음, 1: 플레이어가 소지
    float width, height;
    MapStatus mapStatus;
} Item;

typedef char SpriteType[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS];

extern SpriteType seaweedSprite;
extern SpriteType bubblesSprite;

extern Item itemList[MAX_ITEM_COUNT];
extern int numItem;


void InitItem();
void DrawItem(Item* item, int frame);
void ItemFrameDelay();
