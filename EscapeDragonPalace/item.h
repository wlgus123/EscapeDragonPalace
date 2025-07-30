#pragma once

#define ITEM_SPRITE_ROWS 10
#define ITEM_SPRITE_CLOS 20
#define ITEMNUM 2            // 아이템 종류 수
#define MAX_ITEM_COUNT 100   // 맵에 등장할 수 있는 아이템 인스턴스 수

typedef enum {
    ITEM_SPEED,
    ITEM_LIFE
} ItemType;

typedef struct Item {
    ItemType type;
    float x, y;
    bool isHeld;    // 0: 바닥에 있음, 1: 플레이어가 소지
    int value;     // 효과 수치
    float width, height;
} Item;

typedef char SpriteType[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_CLOS];

extern SpriteType seaweedSprite;
extern SpriteType bubblesSprite;

extern Item itemList[MAX_ITEM_COUNT];
extern int numItem;

void InitItem();
void DrawItem(Item* item, int frame);
void ItemFrameDelay();
