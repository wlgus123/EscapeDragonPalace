#include "init.h"
#include "item.h"
#include "map.h"

typedef char SpriteType[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS];

// 해초 sprite
static const char g_SeaweedSprite[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS] = {
	{
		" )) (( ((",
		"((   )) ))",
		" )) (( (("
	},
	{
		"((  )) ))",
		" ))(( ((",
		"((  )) ))"
	}
};

// 공기방울 sprite
static const char g_BubblesSprite[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS] = {
	{
		"o .o",
		".o o"
	},
	{
		".o o",
		"o .o"
	}
};

// 조개 sprite
static const char g_ClamSprite[CLAM_WIDTH] = "(\\ /)";

Item g_ItemList[MAX_ITEM_COUNT];	// 아이템 배열 선언
int g_ItemIdx = 0;		// 아이템 개수 초기화
int g_Frame = 0;		// 아이템 모션효과용 frame 초기화

clock_t g_LastFrameTime = 0;	// 프레임 전환용 변수
const int g_FrameDelay = 400;	// 400ms마다 프레임 전환

// 아이템 모션효과 frame 계산 함수
void ItemFrameDelay() {
	clock_t now = clock();
	int elapsed = (now - g_LastFrameTime) * 1000 / CLOCKS_PER_SEC;

	if (elapsed >= g_FrameDelay) {
		g_Frame = (g_Frame + 1) % 2;
		g_LastFrameTime = now;
	}

	_Invalidate();  // 매 프레임 그리기는 계속
}

// 아이템 그리는 함수
void DrawItem() {

	// 아이템 배열인덱스 만큼	반복
	for (int i = 0; i < g_ItemIdx; i++)
	{
		// 아이템이 바닥에 있을 때만 그리기 ( 현재 스테이지에 플레이어가 먹지 않은 아이템만 그리기 )
		if (!g_ItemList[i].isHeld) {

			SpriteType* sprite = 0;

			// 아이템 타입에 따라 해초 or 공기방울 or 조개 sprite 선택
			switch (g_ItemList[i].type) {
			case E_ITEM_LIFE:	// 체력증가(해초)일 때
				sprite = &g_SeaweedSprite;
				_SetColor(E_Green);
				break;
			case E_ITEM_SPEED:	// 속도증가(공기방울)일 때
				sprite = &g_BubblesSprite;
				_SetColor(E_BrightTeal);
				break;
			case E_ITEM_DEBUFF:	// 속도감소(조개)일 때
				_SetColor(E_White);
				break;
			}


			int tempX = g_ItemList[i].x - GetPlusX();

			switch (g_ItemList[i].type)
			{
			case E_ITEM_LIFE: case E_ITEM_SPEED:
				for (int row = 0; row < g_ItemList[i].height; row++) {
					if ((*sprite)[g_Frame][row] == '\0') break;  // 공백 줄이면 중단

					for (int col = 0; col < g_ItemList[i].width; col++)
					{
						// 아이템 위치가 화면 내에 있을 때만 출력
						if (0 <= tempX + col && SCREEN_WIDTH >= tempX + col)
						{
							_DrawText(tempX + col, g_ItemList[i].y + row, (char[]) { (*sprite)[g_Frame][row][col], 0 });
						}
					}
				}
				break;
			case E_ITEM_DEBUFF:
				// 조개일 경우 프레임 없이 출력
				for(int col = 0; col < g_ItemList[i].width; col++)
					if (0 <= tempX + col && SCREEN_WIDTH >= tempX + col)
					{
						_DrawText(tempX + col, g_ItemList[i].y, (char[]) { g_ClamSprite[col], 0 });
					}
				break;
			}
		}
	}
}



void InitItem() {

	// 감옥맵 ===============================================

	// 해초
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,		// 아이템 종류
		.x = 353, .y = 14,			// 위치
		.isHeld = false,			// 플레이어 소지 여부
		.width = SEAWEED_WIDTH,		// 너비
		.height = SEAWEED_HEIGHT,	// 높이
		.mapStatus = E_Jail			// 맵
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 530, .y = 12,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Jail
	};

	// 공기방울
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 160, .y = 22,
		.isHeld = false,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Jail
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 570, .y = 22,
		.isHeld = false,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Jail
	};

	// 조개
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 119, .y = 13,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Jail
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 251, .y = 18,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Jail
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 400, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Jail
	};

	// ======================================================

	// 용궁맵 ================================================

	// 해초
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 206, .y = 9,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 288, .y = 6,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 467, .y = 1,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	// 공기방울
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 190, .y = 22,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 340, .y = 22,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 484, .y = 22,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	// 조개
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 145, .y = 19,
		.isHeld = true,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 308, .y = 13,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 413, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_DragonPalace
	};

	// ======================================================


	// 바다맵 1 ==============================================

	// 해초
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 99, .y = 21,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 429, .y = 21,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Sea1
	};

	// 공기방울
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 131, .y = 9,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 282, .y = 8,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 386, .y = 14,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 563, .y = 13,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Sea1
	};

	// 조개
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 140, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 226, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 404, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea1
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 532, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea1
	};

	// ======================================================


	// 바다맵 2 ==============================================

	// 해초
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 100, .y = 21,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 210, .y = 5,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 471, .y = 21,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 542, .y = 11,
		.isHeld = false,
		.width = SEAWEED_WIDTH,
		.height = SEAWEED_HEIGHT,
		.mapStatus = E_Sea2
	};

	// 공기방울
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 138, .y = 11,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 440, .y = 17,
		.isHeld = true,
		.width = BUBBLES_WIDTH,
		.height = BUBBLES_HEIGHT,
		.mapStatus = E_Sea2
	};

	// 조개
	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 62, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 167, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 247, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 335, .y = 23,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea2
	};

	g_ItemList[g_ItemIdx++] = (Item){
		.type = E_ITEM_DEBUFF,
		.x = 374, .y = 18,
		.isHeld = false,
		.width = CLAM_WIDTH,
		.height = CLAM_HEIGHT,
		.mapStatus = E_Sea2
	};

	// ======================================================
}
