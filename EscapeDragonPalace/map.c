#include "init.h"
#include "map.h"

#define STAGE_CNT 5 // ½ºÅ×ÀÌÁö °¹¼ö
#define BG_CNT 5 // ¸Ê ¹è°æ ¿ä¼Ò °¹¼ö
#define BG_LINE 20 // ¸Ê ¹è°æ ÁÙ ¼ö
#define MAP_HEIGHT 25 // ¸Ê ³ôÀÌ
#define GOAL_POS_X 4 // µµÂøÁöÁ¡ ÃÊ±âÀ§Ä¡ X
#define GOAL_POS_Y 7 // µµÂøÁöÁ¡ ÃÊ±âÀ§Ä¡ Y

// CMD »ö±ò
typedef enum CMDColor
{
	E_Black,
	E_Blue,
	E_Green,
	E_Teal, // Ã»·Ï»ö
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
	char bg[20][80];
} StageBG;

int g_Plus = 0;
int g_Key;
char g_Map[49][80] =
{
	"¡«==========================================================================¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«================================                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                              ============================================¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«================================                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«                                                                          ¡«",
	"¡«==========================================================================¡«",
};

// ¿ë±Ã ¹è°æ
StageBG g_StageBG[STAGE_CNT][BG_CNT] =
{
	{0, },
{
	{
		{GOAL_POS_X, GOAL_POS_Y},
		E_BrightBlue,
		{
			"  @@@",
			" @@ @@",
			"@@ @ @@",
			" @@ @@",
			"  @@@",
		} // Goal
	},
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
		E_Green,
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
		{39, 38},
		E_Gray,
		{
			"         ____________________________",
			"        /",
			"    ¦¡¦¡",
			"   /",
			"<____________________________________",
			"   ¦¢                 __________",
			"   ¦¢               /     ¦¢     \\",
			"   ¦¢              ¦¢    .¦¢.    ¦¢",
			"   ¦¢              ¦¢     ¦¢     ¦¢",
			"   ¦¢              ¦¢     ¦¢     ¦¢",
		}
	}
},

};

MapStatus g_MapStatus = E_DragonPalace;

// ¸Ê ±×¸®±â
void DrawMap()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		_DrawText(0, y, g_Map[y + g_Plus]);
	}

	for (int i = 0; i < STAGE_CNT; i++)
	{
		StageBG* tmpBG = g_StageBG[g_MapStatus];
		int printCnt = MAP_HEIGHT;
		for (int j = MAP_HEIGHT; j >= 0; j--)
		{
			int tmpY = tmpBG[i].pos.y + j - g_Plus;
			if (tmpY < 0 || tmpY > MAP_HEIGHT) // ¹è°æÀÌ ¸ÊÀ» ¹þ¾î³µÀ» ¶§ Ãâ·Â X
				continue;
			_DrawTextColor(tmpBG[i].pos.x, tmpY, tmpBG[i].bg[j], tmpBG[i].color);
		}
	}
}

// ¸Ê À§Ä¡ ¾÷µ¥ÀÌÆ®
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