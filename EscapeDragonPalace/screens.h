#pragma once

void GameStartScreen();
void GameOverScreen();
void DrawStartScreen();
void ReturnStartScreen();

static bool GameStart = false;
static bool GameStartText = true;

static bool IsGameOver = false; // 게임 오버 여부
static bool GameOverText = true;
