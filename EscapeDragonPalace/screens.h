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


static char Controls[25][35] = {
"             조작법 설명",
"",
"",
"          　  　 점프",
"                 _____",
"                |  W  |",
"                |_____|",
"           _____ _____ _____",
"          |  A  |  S  |  D  |",
"          |_____|_____|_____|",
" ",
"           왼쪽  아래  오른쪽",
"    ",
"     ____________________________",
"    |          SPACEBAR          |",
"    |____________________________|",
"    ",
"                 공격"
};

