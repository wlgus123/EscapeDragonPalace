#include "init.h"
#include "map.h"
#include "screens.h"
#include "weapon.h"

bool GameStart = false; // 게임 시작 여부
bool GameStartText = true; // 게임 시작 여부 텍스트
bool IsGameOver = false; // 게임 오버 여부
bool GameOverText = true;


// 게임오버 화면 출력 함수
void GameOverScreen() {
	_DrawText(20, 5, "■■■■   ■■   ■   ■  ■■■■");
	_DrawText(20, 6, "■     ■  ■  ■■ ■■  ■      ");
	_DrawText(20, 7, "■ ■■  ■■■■  ■ ■ ■  ■■■■");
	_DrawText(20, 8, "■  ■  ■  ■  ■   ■  ■      ");
	_DrawText(20, 9, "■■■■  ■  ■  ■   ■  ■■■■   ");

	_DrawText(20, 11, "■■■■  ■   ■  ■■■■  ■■■■");
	_DrawText(20, 12, "■  ■  ■   ■  ■     ■  ■");
	_DrawText(20, 13, "■  ■  ■   ■  ■■■■  ■■■■");
	_DrawText(20, 14, "■  ■   ■ ■   ■     ■  ■");
	_DrawText(20, 15, "■■■■    ■    ■■■■  ■   ■");

	_DrawText(28, 17, " (\\(\\ ");
	_DrawText(28, 18, " (x-x)");
	_DrawText(28, 19, "o(   )");
}

// 게임시작 화면 출력 함수
void GameStartScreen()
{

	_DrawText(20, 1, "■■■■  ■■■■  ■■■■   ■■   ■■■■  ■■■■");
	_DrawText(20, 2, "■     ■     ■     ■  ■  ■  ■  ■");
	_DrawText(20, 3, "■■■■  ■■■■  ■     ■■■■  ■■■■  ■■■■");
	_DrawText(20, 4, "■        ■  ■     ■  ■  ■     ■");
	_DrawText(20, 5, "■■■■  ■■■■  ■■■■  ■  ■  ■     ■■■■");

	_DrawText(20, 7, "■■■		■■■■   ■■   ■■■■  ■■■■  ■  ■");
	_DrawText(20, 8, "■  ■		■  ■  ■  ■  ■     ■  ■  ■■ ■");
	_DrawText(20, 9, "■  ■		■■■■  ■■■■  ■ ■■  ■  ■  ■ ■■");
	_DrawText(20, 10, "■  ■		 ■ ■   ■  ■  ■  ■  ■  ■  ■  ■");
	_DrawText(20, 11, "■■■		 ■  ■  ■  ■  ■■■■  ■■■■  ■  ■");

	_DrawText(20, 13, "■■■■   ■■   ■      ■■   ■■■■  ■■■■");
	_DrawText(20, 14, "■  ■  ■  ■  ■     ■  ■  ■     ■");
	_DrawText(20, 15, "■■■■  ■■■■  ■     ■■■■  ■     ■■■■");
	_DrawText(20, 16, "■     ■  ■  ■     ■  ■  ■     ■");
	_DrawText(20, 17, "■     ■  ■  ■■■■  ■  ■  ■■■■  ■■■■");
}

// 게임 클리어 화면 출력 함수
void GameClearSceen() 
{
	_DrawText(27, 5, "■■■■   ■■   ■   ■  ■■■■");
	_DrawText(27, 6, "■     ■  ■  ■■ ■■  ■      ");
	_DrawText(27, 7, "■ ■■  ■■■■  ■ ■ ■  ■■■■");
	_DrawText(27, 8, "■  ■  ■  ■  ■   ■  ■      ");
	_DrawText(27, 9, "■■■■  ■  ■  ■   ■  ■■■■   ");
			   
	_DrawText(25, 11, "■■■■  ■     ■■■■   ■■   ■■■■");
	_DrawText(25, 12, "■     ■     ■     ■  ■  ■  ■");
	_DrawText(25, 13, "■     ■     ■■■■  ■■■■  ■■■■");
	_DrawText(25, 14, "■     ■     ■     ■  ■  ■ ■");
	_DrawText(25, 15, "■■■■  ■■■■  ■■■■  ■  ■  ■  ■");

	_DrawText(27, 17, "       ________");
	_DrawText(27, 18, " ___ _/ \\__/ \\_   /|");
	_DrawText(27, 19, "(_x / \\ /  \\ / \\_/ |");
	_DrawText(27, 20, " \\__ -----------__/");
	_DrawText(27, 21, " (___\\_|_|_|_|_/___)");
}

void DrawStartScreen() {
	// 게임 시작 전일 때
	while (!GameStart) {
		// 키 입력이 들어오면
		if (_kbhit()) {
			GameStart = true;   // 게임 시작
			_getch();   // 입력 버퍼 비우기
		}
		// 문구 이펙트 효과
		GameStartText = !GameStartText;
		_Invalidate();
		Sleep(500);
	}
}

void ReturnStartScreen() {

	// 게임 오버일 때
	while (IsGameOver) {
		// 키 입력이 들어오면
		if (_kbhit()) {
			IsGameOver = false; // 게임오버 변수 false 변경
			GameStart = false;  // 게임시작 변수 false 변경
			SetWeaponChosen(false); // 무기 선택여부 false로 변경
			SetMapSetting(false);	// 아이템 세팅 초기화
			SetMapStatus(E_Jail);	// 원래 맵으로 이동
			SetPlusX(0);	// X 좌표 증가값 0으로 변경
			_getch();	// 입력 버퍼 비우기
		}
		// 문구 이펙트 효과
		GameOverText = !GameOverText;
		_Invalidate();
		Sleep(500);
	}

}

// 게임 시작 여부 가져오기
bool GetGameStart()
{
	return GameStart;
}

// 게임 시작 여부 텍스트 가져오기
bool GetGameStartText()
{
	return GameStartText;
}

bool GetIsGameOver()
{
	return IsGameOver;
}

void SetIsGameOver(int src)
{
	IsGameOver = src;
}

bool GetGameOverText()
{
	return GameOverText;
}

void SetIsGameOverText(int src)
{
	GameOverText = src;
}