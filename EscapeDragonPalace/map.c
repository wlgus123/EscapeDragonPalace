#include "init.h"
#include "map.h"

int g_Plus_Y = 24; // 감옥, 용궁 맵 시작 위치
int g_Plus_X = 0; // 바다 1, 2 맵 시작 위치

// TODO: 맵 시작 시 E_Jail에서 시작
// TODO: 맵 클리어 시 자동으로 넘어가기
// 현재 맵 정보
MapStatus g_MapStatus = E_Sea1;

// 맵 그리기
// TODO: 코드 수정
void DrawMap()
{
	switch (g_MapStatus)
	{
	case E_Jail: case E_DragonPalace:	// 맵이 감옥, 용궁일 경우
		// 배경 그리기
		for (int i = 0; i < BG_CNT; i++)
		{
			StageBG tmpBG = g_StageBG[g_MapStatus][i]; // 현재 맵의 배경 정보 가져오기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				int tmpY = tmpBG.pos.y + y - g_Plus_Y;
				// 배경 범위 내에 있을 때 출력
				if (tmpY >= 0 && tmpY <= SCREEN_HEIGHT)
					_DrawTextColor(tmpBG.pos.x, tmpY, tmpBG.bg[y], tmpBG.color);
			}
		}
		// 발판 그리기
		for (int i = 0; i < PLATFORM_LINE_CNT; i++)
		{
			StageBG tmpPlatform = g_StagePlatform[g_MapStatus][i]; // 현재 맵의 발판 정보 가져오기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				int tmpY = tmpPlatform.pos.y + y - g_Plus_Y;

				// 배경 범위 내에 있을 때 출력
				if (tmpY >= 0 && tmpY <= SCREEN_HEIGHT)
					_DrawTextColor(tmpPlatform.pos.x, tmpY, tmpPlatform.bg[y], tmpPlatform.color);
			}
		}
		break;

	case E_Sea1: case E_Sea2:	// 맵이 바다1, 바다2일 경우
		// 배경 그리기
		for (int i = 0; i < BG_CNT; i++)
		{
			StageBG tmpBG = g_StageBG[g_MapStatus][i]; // 현재 맵의 배경 정보 가져오기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				for (int x = 0; x < SCREEN_WIDTH; x++)
				{
					int tmpX = tmpBG.pos.x + x - g_Plus_X;
					if (tmpX >= 0 && tmpX <= SCREEN_WIDTH) // 배경이 범위 내에 있을 때만 출력
						_DrawTextColor(tmpX, tmpBG.pos.y + y, (char[]) { tmpBG.bg[y][x], 0 }, tmpBG.color);

				}
			}
		}
		// 발판 그리기
		for (int i = 0; i < PLATFORM_LINE_CNT; i++)
		{
			StageBG tmpPlatform = g_StagePlatform[g_MapStatus][i]; // 현재 맵의 발판 정보 가져오기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				int tmpX = tmpPlatform.pos.x + y - g_Plus_X;
				if (tmpX >= 0 && tmpX <= SCREEN_WIDTH) // 배경이 범위 내에 있을 때만 출력
					_DrawTextColor(tmpX, tmpPlatform.pos.y, (char[]) { tmpPlatform.bg[y], 0 }, tmpPlatform.color);
			}
		}
		break;

	case E_Ground:	// 맵이 육지(보스맵)일 경우
		// 배경 그리기
		for (int i = 0; i < BG_CNT; i++)
		{
			StageBG tmpBG = g_StageBG[g_MapStatus][i]; // 현재 맵의 배경 정보 가져오기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(tmpBG.pos.x, tmpBG.pos.y, tmpBG.bg[y], tmpBG.color);
			}
		}
		// 발판 그리기
		for (int i = 0; i < PLATFORM_LINE_CNT; i++)
		{
			StageBG tmpPlatform = g_StagePlatform[g_MapStatus][i]; // 현재 맵의 발판 정보 가져오기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(tmpPlatform.pos.x, tmpPlatform.pos.y, tmpPlatform.bg[y], tmpPlatform.color);
			}
		}
		break;
	}

	// 맵 틀 그리기
	_DrawText(0, 0, "==============================================================================");
	_DrawText(0, SCREEN_HEIGHT - 1, "==============================================================================");
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		_DrawText(0, y, "∥");
		_DrawText(76, y, "∥");
	}
}

// 현재 맵 불러오기
int GetMapStatus()
{
	return g_MapStatus;
}

// 현재 맵 상태 변경하기
void SetMapStatus(int src)
{
	g_MapStatus = src;
}

// 맵 위치 업데이트
void UpdateMapPos()
{
	switch (g_MapStatus)
	{
	case E_Jail: case E_DragonPalace:
		if (g_Key == 'w' || g_Key == 'W')
		{
			--g_Plus_Y;
		}
		else if (g_Key == 's' || g_Key == 'S')
		{
			++g_Plus_Y;
		}
		if (g_Plus_Y < 0)
			g_Plus_Y = 0;
		else if (g_Plus_Y > 24)
			g_Plus_Y = 24;
		break;
	case E_Sea1: case E_Sea2:
		if (g_Key == 'a' || g_Key == 'A')
		{
			g_Plus_X -= 2;
		}
		else if (g_Key == 'd' || g_Key == 'D')
		{
			g_Plus_X += 2;
		}
		if (g_Plus_X < 0)
			g_Plus_X = 0;
		else if (g_Plus_X > 222)
			g_Plus_X = 222;
		break;
	}

}