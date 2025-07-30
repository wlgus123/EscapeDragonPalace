#pragma once

bool GameStart = false;
bool GameStartText = true;

bool IsGameOver = false; // 게임 오버 여부
bool GameOverText = true;

void GameStartScreen();
void GameOverScreen();
void DrawStartScreen();
void ReturnStartScreen();
