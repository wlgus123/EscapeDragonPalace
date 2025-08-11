#pragma once

#define ESC 27 // ESC 키 값

// 함수
void GameStartScreen();
void GameOverScreen();
void GameClearSceen();
void DrawControls();
void DrawStartScreen();
void ReturnStartScreen();
void DrawGameClearScreen();
bool GetGameStart();
bool GetIsGameOver();
void SetIsGameOver(int src);
bool GetTextE();
void SetTextE(int src);
