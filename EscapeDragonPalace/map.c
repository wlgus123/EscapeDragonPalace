#include "map.h"
#include "Rabbit.h"

float g_Plus_X = 0.f; // X좌표 이동 증가값

// TODO: 맵 시작 시 E_Jail에서 시작
// TODO: 맵 클리어 시 자동으로 넘어가기
// 현재 맵 정보
MapStatus g_MapStatus = E_Jail;

// TODO: 코드 수정
// 맵 틀 그리기
void DrawMap()
{
	_DrawText(0, 0, "==============================================================================");
	_DrawText(0, SCREEN_HEIGHT - 1, "==============================================================================");
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		_DrawText(0, y, "∥");
		_DrawText(78, y, "∥");
	}
}

// 맵 배경 그리기
void DrawMapBG()
{
	// 보스 맵일 경우
	if (g_MapStatus == E_Ground)
	{
		// 배경 그리기
		for (int y = 0; y < SCREEN_HEIGHT; y++)
			_DrawTextColor(0, y, g_StageBG[g_MapStatus][y], E_Gray);

		// 발판 그리기
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			char* tmpPlatform = g_StagePlatform[g_MapStatus][y]; // 현재 맵의 발판 정보 가져오기
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				if (tmpPlatform[x] != ' ')
					_DrawTextColor(x, y, (char[]) { tmpPlatform[x], 0 }, E_White);
			}
		}
	}
	// 일반 맵일 경우
	else
	{
		if (g_MapStatus >= 5)
			return;

		// 배경 그리기
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			char* tmpBG = g_StageBG[g_MapStatus][y]; // 현재 맵의 배경 정보 가져오기
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				_DrawTextColor(x, y, (char[]) { tmpBG[(int)(x + g_Plus_X)], 0 }, E_Gray);
			}
		}
		// 발판 그리기
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			char* tmpPlatform = g_StagePlatform[g_MapStatus][y]; // 현재 맵의 발판 정보 가져오기
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				if (tmpPlatform[(int)(x + g_Plus_X)] != ' ')
					_DrawTextColor(x, y, (char[]) { tmpPlatform[(int)(x + g_Plus_X)], 0 }, E_White);
			}
		}
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

// X 증가값 불러오기
int GetPlusX()
{
	return g_Plus_X;
}

// X값 변경하기
void SetPlusX(int src)
{
	g_Plus_X = src;
}

// 맵 위치 업데이트
void UpdateMapPos()
{
	if (GetKeyA() && player.Pos.x <= 24 || ((GetAsyncKeyState('A') & 0x8000) && !(GetAsyncKeyState('D') & 0x8000)))
	{
		SetMapEnd(false);
		// 토끼가 끝 위치에서 왼쪽으로 이동할 때 화면의 가운데로 이동할 때까지 배경 멈추기
		if (player.Pos.x >= 26) return;
		g_Plus_X -= player.Speed;
	}
	else if (GetKeyD() && player.Pos.x >= 26 || ((GetAsyncKeyState('D') & 0x8000) && !(GetAsyncKeyState('A') & 0x8000)))
	{
		if(g_MapStatus < E_Ground)
			SetMapEnd(false);
		if (g_MapStatus == E_Ground)
			SetMapEnd(true);
		// 토끼가 시작 위치에서 오른쪽으로 이동할 때 화면의 가운데로 이동할 때까지 배경 멈추기
		if (player.Pos.x <= 24) return;
		g_Plus_X += player.Speed;
	}

	// X좌표 증가값이 맵을 넘어갈 때
	if (g_Plus_X < 0.f)
	{
		g_Plus_X = 0.f;
		SetMapEnd(true);
	}
	else if (g_MapStatus < E_Ground && g_Plus_X > MAP_WIDTH - SCREEN_WIDTH) // 일반 맵일 때
	{
		g_Plus_X = MAP_WIDTH - SCREEN_WIDTH;
		SetMapEnd(true);
	}
	else if (g_MapStatus == E_Ground) // 보스 맵일 때
	{
		g_Plus_X = 0.f;
		SetMapEnd(true);
	}

}