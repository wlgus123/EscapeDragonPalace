#include "init.h"
#include "map.h"

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
	E_Ground,
} MapStatus;

typedef struct StageBG
{
	MyPoint pos;
	CMDColor color;
	char bg[20][256];
} StageBG;

int g_Plus = 0;
int g_Key;
char g_Map[49][80] =
{
	"∥==========================================================================∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥=                                                                         ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                         =∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥=                                                                         ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥                                                                          ∥",
	"∥==========================================================================∥",
};

// 용궁 배경
StageBG g_StageBG[STAGE_CNT][BG_CNT] =
{
{ // 맵 - 감옥
	{
		{2, 1},
		E_Gray,
		{
			"┐ ┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐│",
			"│ │││││││││││││││││││",
			"│ │││││││││││││││││││",
			"│ └┘└┘└┘│││││││││││││",
			"│ ┌────┐│││││││││││││",
			"│ │       ││││││││││││││",
			"│ │       ││││││││││││││",
			"┘ │       │└┘└┘└┘└┘└┘└┘│",
			"┐ │       │┌┐┌┐┌┐┌┐┌┐┌┐│",
			"│ │       ││││││││││││││",
			"│ │       ││││││││││││││",
		},
	},
	{
		{6, 7},
		E_BrightBlue,
		{
			"  @@@",
			" @@ @@",
			"@@ @ @@",
			" @@ @@",
			"  @@@",
		} // Goal
	},
},
{ // 맵 - 용궁
	{
		{36, 4},
		E_Gray,
		{
			"                 /\\        /\\",
			"                ( (________) )",
			"               /             \\",
			"       _______/               \\_______",
			"       \\                             /",
			"        \\___________________________/",
			"             |       ___       |",
			"   __________|______|_|_|______|________",
			"   \\                                   /",
			"    \\_________________________________/",
			"         |        _________        |",
			"         |       |    |    |       |",
			"   ______|_______|____|____|_______|____",
			"  /",
			" /",
			"/_______________________________________",
			"     |           ___________          |",
			"     |          |     |     |         |",
			"     |          |    .|.    |         |",
			"     |          |     |     |         |",
		},
	},
	{
		{3, 29},
		E_Gray,
		{
			"         __ ",
			"       / / /",
			"       \\ \\ \\    __ ",
			"       / / /  / / /",
			"  __   \\ \\ \\  \\ \\ \\",
			"/ / /  / / /  / / /",
			"\\ \\ \\  \\ \\ \\  \\ \\ \\",
		}
	},
	{
		{45, 42},
		E_Gray,
		{
			"        __",
			"       \\ \\ \\",
			" __    / / /",
			"\\ \\ \\  \\ \\ \\",
			"/ / /  / / /",
			"\\ \\ \\  \\ \\ \\",
		}
	},
	{
		{3, 38},
		E_Gray,
		{
			"_____________________ ",
			"                     \\ ",
			"                      \\ ",
			"                       \\ ",
			"________________________\\ ",
			"                  |",
			"_________         |",
			"   |     \\        |",
			"  .|.    |        |",
			"   |     |        |",
		}
	},
	{
		{4, 7},
		E_BrightBlue,
		{
			"  @@@",
			" @@ @@",
			"@@ @ @@",
			" @@ @@",
			"  @@@",
		} // Goal
	},
},
};

StageBG g_StagePlatform[STAGE_CNT][PLATFORM_CNT] =
{
	{0, },
{
	{
		{2, 12},
		E_BrightWhite,
		{
		"=====================",
		}
	},
	{
		{26, 18},
		E_BrightWhite,
		{
			"=======",
		}
	},
	{
		{35, 24},
		E_BrightWhite,
		{
		"=========================================",
		}
	},
	{
		{17, 30},
		E_BrightWhite,
		{
			"========",
		}
	},
	{
		{2, 36},
		E_BrightWhite,
		{
		"=====================================================",
		}
	},
	{
		{60, 42},
		E_BrightWhite,
		{
			"================",
		}
	},

},
};

MapStatus g_MapStatus = E_DragonPalace;

// 맵 그리기
void DrawMap()
{
	// 맵 틀 그리기
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < sizeof(g_Map[y]); x++)
			if(g_Map[y][x])
			_DrawText(0, y, g_Map[y + g_Plus]);
	}

	// 배경 그리기
	for (int i = 0; i < BG_CNT; i++)
	{
		StageBG* tmpBG = g_StageBG[g_MapStatus]; // 현재 맵의 배경 정보 가져오기
		// 배경 그리기
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			int tmpY = tmpBG[i].pos.y + j - g_Plus;
			if (tmpY < 0 || tmpY > SCREEN_HEIGHT) // 배경이 맵을 벗어났을 때 출력 X
				continue;
			_DrawTextColor(tmpBG[i].pos.x, tmpY, tmpBG[i].bg[j], tmpBG[i].color);
		}
	}
	// 발판 그리기
	for (int i = 0; i < PLATFORM_CNT; i++)
	{
		StageBG* tmpPlatform = g_StagePlatform[g_MapStatus]; // 현재 맵의 발판 정보 가져오기
		// 발판 그리기
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			int tmpY = tmpPlatform[i].pos.y + j - g_Plus;
			if (tmpY < 0 || tmpY > SCREEN_HEIGHT)
				continue;
			_DrawTextColor(tmpPlatform[i].pos.x, tmpY, tmpPlatform[i].bg[j], tmpPlatform[i].color);
		}
	}
}

// 맵 위치 업데이트
void UpdateMapPos()
{
	if (g_Key == 'w' || g_Key == 'W')
	{
		--g_Plus;
	}
	else if (g_Key == 's' || g_Key == 'S')
	{
		++g_Plus;
	}
	if (g_Plus < 0)
		g_Plus = 0;
	else if (g_Plus > 24)
		g_Plus = 24;
}