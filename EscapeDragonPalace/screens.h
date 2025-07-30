#pragma once

void GameStartScreen();
void GameOverScreen();
void DrawStartScreen();
void ReturnStartScreen();
bool GetGameStart(); // 게임 시작 여부 가져오는 함수
bool GetGameStartText();

static bool IsGameOver = false; // 게임 오버 여부
static bool GameOverText = true;
