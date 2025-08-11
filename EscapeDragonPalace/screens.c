#include "init.h"
#include "map.h"
#include "screens.h"
#include "weapon.h"
#include "turtle.h"
#include "Rabbit.h"

bool GameStart = false;		// 게임 시작 여부
bool IsGameOver = false;	// 게임 오버 여부
bool textE = true;			// 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)

bool ControlScreen = false; // 조작 화면 여부

// 게임오버 화면
void GameOverScreen() {
	_DrawText(28, 4, "■■■■   ■■   ■   ■  ■■■■");
	_DrawText(28, 5, "■     ■  ■  ■■ ■■  ■      ");
	_DrawText(28, 6, "■ ■■  ■■■■  ■ ■ ■  ■■■■");
	_DrawText(28, 7, "■  ■  ■  ■  ■   ■  ■      ");
	_DrawText(28, 8, "■■■■  ■  ■  ■   ■  ■■■■   ");
			   
	_DrawText(28, 10, "■■■■  ■   ■  ■■■■  ■■■■");
	_DrawText(28, 11, "■  ■  ■   ■  ■     ■  ■");
	_DrawText(28, 12, "■  ■  ■   ■  ■■■■  ■■■■");
	_DrawText(28, 13, "■  ■   ■ ■   ■     ■  ■");
	_DrawText(28, 14, "■■■■    ■    ■■■■  ■   ■");

	_DrawText(36, 16, " (\\(\\ ");
	_DrawText(36, 17, " (x-x)");
	_DrawText(36, 18, "o(   )");
}

// 게임시작 화면
void GameStartScreen()
{

	_DrawText(23, 2, "■■■■  ■■■■  ■■■■   ■■   ■■■■  ■■■■");
	_DrawText(23, 3, "■     ■     ■     ■  ■  ■  ■  ■");
	_DrawText(23, 4, "■■■■  ■■■■  ■     ■■■■  ■■■■  ■■■■");
	_DrawText(23, 5, "■        ■  ■     ■  ■  ■     ■");
	_DrawText(23, 6, "■■■■  ■■■■  ■■■■  ■  ■  ■     ■■■■");
			   
	_DrawText(23, 8, "■■■   ■■■■   ■■   ■■■■  ■■■■  ■  ■");
	_DrawText(23, 9, "■  ■  ■  ■  ■  ■  ■     ■  ■  ■■ ■");
	_DrawText(23, 10, "■  ■  ■■■■  ■■■■  ■ ■■  ■  ■  ■ ■■");
	_DrawText(23, 11, "■  ■  ■ ■   ■  ■  ■  ■  ■  ■  ■  ■");
	_DrawText(23, 12, "■■■   ■  ■  ■  ■  ■■■■  ■■■■  ■  ■");
			   
	_DrawText(23, 14, "■■■■   ■■   ■      ■■   ■■■■  ■■■■");
	_DrawText(23, 15, "■  ■  ■  ■  ■     ■  ■  ■     ■");
	_DrawText(23, 16, "■■■■  ■■■■  ■     ■■■■  ■     ■■■■");
	_DrawText(23, 17, "■     ■  ■  ■     ■  ■  ■     ■");
	_DrawText(23, 18, "■     ■  ■  ■■■■  ■  ■  ■■■■  ■■■■");
}

// 게임 클리어 화면
void GameClearSceen()
{
	_DrawText(26, 3, "■■■■    ■■    ■   ■   ■■■■");
	_DrawText(26, 4, "■      ■  ■   ■■ ■■   ■      ");
	_DrawText(26, 5, "■ ■■   ■■■■   ■ ■ ■   ■■■■");
	_DrawText(26, 6, "■  ■   ■  ■   ■   ■   ■      ");
	_DrawText(26, 7, "■■■■   ■  ■   ■   ■   ■■■■   ");

	_DrawText(25, 9, "■■■■  ■     ■■■■   ■■   ■■■■");
	_DrawText(25, 10, "■     ■     ■     ■  ■  ■  ■");
	_DrawText(25, 11, "■     ■     ■■■■  ■■■■  ■■■■");
	_DrawText(25, 12, "■     ■     ■     ■  ■  ■ ■");
	_DrawText(25, 13, "■■■■  ■■■■  ■■■■  ■  ■  ■  ■");

	_DrawText(29, 15, "       ______");
	_DrawText(29, 16, " ___ _/ \\__/ \\_   /|");
	_DrawText(29, 17, "(_x / \\ /  \\ / \\_/ |");
	_DrawText(29, 18, " \\__ -----------__/");
	_DrawText(29, 19, " (___\\_|_|_|_|_/___)");
}

// 게임 시작 화면 출력 함수
void DrawStartScreen() {
	// 게임 시작 전일 때
	while (!GameStart) {
		// 키 입력이 들어오면
		if (_kbhit()) {
			GameStart = true;   // 게임 시작
			_getch();			// 입력 버퍼 비우기
		}
		textE = !textE; // 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)
		_Invalidate();
		Sleep(500);
	}
}

// 게임 오버 화면 출력 함수
void ReturnStartScreen() {
	// 게임 오버일 때
	while (IsGameOver) {
		// 키 입력이 들어오면
		if (_kbhit()) {
			if (_getch() == ESC) {	// ESC 키 입력시
				// ESC 키를 누르면 게임 종료
				system("cls"); // 화면 지우기
				exit(0); // 프로그램 종료
			}
			else {
				IsGameOver = false;		// 게임오버 변수 false 변경
				GameStart = false;		// 게임시작 변수 false 변경
				SetWeaponChosen(false);	// 무기 선택여부 false로 변경
				SetMapSetting(false);	// 아이템 세팅 초기화
				SetIsNearLadder(false); // 사다리 근처 여부 false로 변경
				SetInvincibleTime(false); // 플레이어 무적 시간 false로 변경
				SetMapStatus(E_Jail);	// 원래 맵으로 이동
				SetPlusX(0);			// X 좌표 증가값 0으로 변경
			}
		}
		textE = !textE; // 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)
		_Invalidate();
		Sleep(500);
	}

}

// 게임 클리어 화면 출력 함수
void DrawGameClearScreen() {
	// 게임 클리어 후 ( 보스 자라 몬스터의 체력이 0 이하일 때 )
	while (GetTurtleHp() <= 0) {
		// 키 입력이 들어오면
		if (_kbhit()) {
			if (_getch() == ESC) {	// ESC 키 입력시
				// ESC 키를 누르면 게임 종료
				system("cls"); // 화면 지우기
				exit(0); // 프로그램 종료
			}
			else {
				GameStart = false;		// 게임시작 변수 false 변경
				SetWeaponChosen(false); // 무기 선택여부 false로 변경
				SetMapSetting(false);	// 아이템 세팅 초기화
				SetMapStatus(E_Jail);	// 원래 맵으로 이동
				SetIsNearLadder(false); // 사다리 근처 여부 false로 변경
				SetInvincibleTime(false); // 플레이어 무적 시간 false로 변경
				SetPlusX(0);			// X 좌표 증가값 0으로 변경
				ResetTurtleHp();		// 자라 몬스터 체력 초기화	
			}
		}
		textE = !textE; // 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)
		_Invalidate();
		Sleep(500);
	}

}

// 게임 시작 여부 가져오기
bool GetGameStart()
{
	return GameStart;
}

// 게임 오버 여부 가져오기
bool GetIsGameOver()
{
	return IsGameOver;
}

// 게임 오버 여부 설정
void SetIsGameOver(int src)
{
	IsGameOver = src;
}

// 문구 이펙트 효과용 변수 가져오기
bool GetTextE()
{
	return textE;
}

// 문구 이펙트 효과용	변수 설정
void SetTextE(int src)
{
	textE = src;
}

bool GetControlScreen()
{
	return ControlScreen;
}

void SetControlScreen(bool src)
{
	ControlScreen = src;
}

void DrawControlsScreen() {
	while (!ControlScreen) {
		if (_kbhit()) {
			ControlScreen = true;
			_getch();
		}
		textE = !textE;
		_Invalidate();
		Sleep(500);
	}
}