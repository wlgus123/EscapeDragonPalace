#pragma once

#define ESC 27 // ESC 키 값

// 함수
void GameStartScreen();
void GameOverScreen();
void GameClearSceen();
void DrawStartScreen();
void ReturnStartScreen();
void DrawGameClearScreen();
bool GetGameStart();
bool GetIsGameOver();
void SetIsGameOver(int src);
bool GetTextE();
void SetTextE(int src);
bool GetControlScreen();
void SetControlScreen(bool src);
void DrawControlsScreen();


static char Controls[SCREEN_HEIGHT][SCREEN_WIDTH] = {
"                     조작법 설명",
"",
"",
"",
"",
" 　  　  점프",
"        _____",
"       |  W  |                     _______________",
"       |_____|                    |    SPACEBAR   |",
"  _____ _____ _____               |_______________|",
" |  A  |  S  |  D  |",		      
" |_____|_____|_____|                    공격",
" ",
" 왼쪽  아래  오른쪽",
};
