#pragma once

#define ESC 27 // ESC 키 값

void GameStartScreen();
void GameOverScreen();
void DrawStartScreen();
void ReturnStartScreen();
bool GetGameStart(); // 게임 시작 여부 가져오는 함수
bool GetIsGameOver();	// 게임 오버 여부 반환
void SetIsGameOver(int src);
bool GetTextE();		// 문구 이펙트 효과
void SetTextE(int src);
void GameClearSceen(); // 게임 클리어 화면 출력 함수
void DrawGameClearScreen(); // 게임 클리어 화면 출력 함수