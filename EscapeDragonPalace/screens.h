#pragma once

void GameStartScreen();
void GameOverScreen();
void DrawStartScreen();
void ReturnStartScreen();
bool GetGameStart(); // 게임 시작 여부 가져오는 함수
bool GetGameStartText();
bool GetIsGameOver();	// 게임 오버 여부 반환
void SetIsGameOver(int src);
bool GetGameOverText();	// 게임 오버 텍스트 그리기 여부 반환
void SetIsGameOverText(int src);