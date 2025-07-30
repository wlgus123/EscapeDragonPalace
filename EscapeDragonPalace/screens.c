#include "init.h"
#include "screens.h"
#include "weapon.h"

bool GameStart = false; // 게임 시작 여부
bool GameStartText = true; // 게임 시작 여부 텍스트

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


void GameStartScreen()
{
    _DrawText(20, 1, "■■■    ■■   ■■■■  ■■■■  ■■■■  ■  ■");
    _DrawText(20, 2, "■  ■  ■  ■  ■  ■  ■     ■  ■  ■■ ■");
    _DrawText(20, 3, "■  ■  ■■■■  ■■■■  ■ ■■  ■  ■  ■ ■■");
    _DrawText(20, 4, "■  ■  ■  ■  ■ ■   ■  ■  ■  ■  ■  ■");
    _DrawText(20, 5, "■■■   ■  ■  ■  ■  ■■■■  ■■■■  ■  ■");

    _DrawText(20, 7, "■■■■   ■■   ■      ■■   ■■■■  ■■■■");
    _DrawText(20, 8, "■  ■  ■  ■  ■     ■  ■  ■     ■");
    _DrawText(20, 9, "■■■■  ■■■■  ■     ■■■■  ■     ■■■■");
    _DrawText(20, 10, "■     ■  ■  ■     ■  ■  ■     ■");
    _DrawText(20, 11, "■     ■  ■  ■■■■  ■  ■  ■■■■  ■■■■");

    _DrawText(20, 13, "■■■■  ■■■■  ■■■■   ■■   ■■■■  ■■■■");
    _DrawText(20, 14, "■     ■     ■     ■  ■  ■  ■  ■");
    _DrawText(20, 15, "■■■■  ■■■■  ■     ■■■■  ■■■■  ■■■■");
    _DrawText(20, 16, "■        ■  ■     ■  ■  ■     ■");
    _DrawText(20, 17, "■■■■  ■■■■  ■■■■  ■  ■  ■     ■■■■");


}

void DrawStartScreen() {
    while(!GameStart) {
        if (_kbhit()){
            GameStart = true;
            _getch();
        }
        GameStartText = !GameStartText;
        _Invalidate();
        Sleep(500);
    }
}

void ReturnStartScreen() {

    while (IsGameOver) {
        if (_kbhit()) {
            IsGameOver = false;
			GameStart = false;
            weaponChosen = false;
            _getch();
        }
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
