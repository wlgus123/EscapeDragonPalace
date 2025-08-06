#include "init.h"
#include "item.h"

// 해초 sprite
char seaweedSprite[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS] = {
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
char bubblesSprite[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS] = {
	{
		"o .o",
		".o o"
	},
	{
		".o o",
		"o .o"
	}
};


Item itemList[MAX_ITEM_COUNT];	// 아이템 배열 선언
int numItem = 0;	// 아이템 개수 초기화
int frame = 0;		// 아이템 모션효과용 frame 초기화

clock_t lastFrameTime = 0;
const int frameDelay = 400;  // 400ms마다 프레임 전환

// 아이템 모션효과 frame 계산 함수
void ItemFrameDelay() {
	clock_t now = clock();
	int elapsed = (now - lastFrameTime) * 1000 / CLOCKS_PER_SEC;

	if (elapsed >= frameDelay) {
		frame = (frame + 1) % 2;
		lastFrameTime = now;
	}

	_Invalidate();  // 매 프레임 그리기는 계속!
}

// 아이템 그리는 함수
void DrawItem() {

	for (int i = 0; i < numItem; i++)
	{
		if (!itemList[i].isHeld) {

			SpriteType* sprite = &seaweedSprite;

			// 아이템 타입에 따라 해초 or 공기방울 sprite 선택
			switch (itemList[i].type) {
			case E_ITEM_LIFE:
				sprite = &seaweedSprite;
				_SetColor(E_Green);

				break;
			case E_ITEM_SPEED:
				sprite = &bubblesSprite;
				_SetColor(E_Teal);
				break;
			}


			int tempX = itemList[i].x - GetPlusX();


			for (int row = 0; row < ITEM_SPRITE_ROWS; row++) {
				if ((*sprite)[frame][row] == '\0') break;  // 공백 줄이면 중단

				for (int col = 0; col < ITEM_SPRITE_COLS; col++)
				{
					// 아이템 위치가 화면 내에 있을 때만 출력
					if (0 <= tempX + col && SCREEN_WIDTH >= tempX + col)
					{
						if (tempX + 7 > 0 && tempX < SCREEN_WIDTH) {
							_DrawText(tempX + col, itemList[i].y + row, (char[]) { (*sprite)[frame][row][col], 0 });
						}
					}
				}
			}
		}
	}

}



void InitItem() {

	// 감옥맵 ===============================================

	// 해초
	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 353, .y = 14,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 0
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 530, .y = 12,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 0
	};

	// 공기방울
	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 160, .y = 22,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 0
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 570, .y = 22,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 0
	};

	// ======================================================

	// 용궁맵 ================================================

	// 해초
	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 206, .y = 9,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 1
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 288, .y = 6,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 1
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 467, .y = 1,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 1
	};

	// 공기방울
	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 190, .y = 22,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 1
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 340, .y = 22,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 1
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 484, .y = 22,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 1
	};

	// ======================================================


	// 바다맵 1 ==============================================

	// 해초
	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 99, .y = 21,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 2
	};

	itemList[numItem++] = (Item){
	.type = E_ITEM_LIFE,
	.x = 429, .y = 21,
	.isHeld = false,
	.width = 11,
	.height = 3,
	.mapStatus = 2
	};

	// 공기방울
	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 131, .y = 9,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 2
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 282, .y = 8,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 2
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 386, .y = 14,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 2
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 563, .y = 13,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 2
	};

	// ======================================================


	// 바다맵 2 ==============================================

	// 해초
	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 100, .y = 21,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 3
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 210, .y = 5,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 3
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 471, .y = 21,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 3
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_LIFE,
		.x = 542, .y = 11,
		.isHeld = false,
		.width = 11,
		.height = 3,
		.mapStatus = 3
	};

	// 공기방울
	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 138, .y = 11,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 3
	};

	itemList[numItem++] = (Item){
		.type = E_ITEM_SPEED,
		.x = 440, .y = 17,
		.isHeld = true,
		.width = 5,
		.height = 2,
		.mapStatus = 3
	};

	// ======================================================
}
