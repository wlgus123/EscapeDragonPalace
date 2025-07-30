#include "init.h"
#include "screens.h"
#include "weapon.h"

void GameOverScreen() {
    _DrawText(20, 5, "бсбсбсбс   бсбс   бс   бс  бсбсбсбс");
    _DrawText(20, 6, "бс     бс  бс  бсбс бсбс  бс      ");
    _DrawText(20, 7, "бс бсбс  бсбсбсбс  бс бс бс  бсбсбсбс");
    _DrawText(20, 8, "бс  бс  бс  бс  бс   бс  бс      ");
    _DrawText(20, 9, "бсбсбсбс  бс  бс  бс   бс  бсбсбсбс   ");
              
    _DrawText(20, 11, "бсбсбсбс  бс   бс  бсбсбсбс  бсбсбсбс");
    _DrawText(20, 12, "бс  бс  бс   бс  бс     бс  бс");
    _DrawText(20, 13, "бс  бс  бс   бс  бсбсбсбс  бсбсбсбс");
    _DrawText(20, 14, "бс  бс   бс бс   бс     бс  бс");
    _DrawText(20, 15, "бсбсбсбс    бс    бсбсбсбс  бс   бс");
            
    _DrawText(28, 17, " (\\(\\ ");
    _DrawText(28, 18, " (x-x)");
    _DrawText(28, 19, "o(   )");
}


void GameStartScreen()
{
    _DrawText(20, 1, "бсбсбс    бсбс   бсбсбсбс  бсбсбсбс  бсбсбсбс  бс  бс");
    _DrawText(20, 2, "бс  бс  бс  бс  бс  бс  бс     бс  бс  бсбс бс");
    _DrawText(20, 3, "бс  бс  бсбсбсбс  бсбсбсбс  бс бсбс  бс  бс  бс бсбс");
    _DrawText(20, 4, "бс  бс  бс  бс  бс бс   бс  бс  бс  бс  бс  бс");
    _DrawText(20, 5, "бсбсбс   бс  бс  бс  бс  бсбсбсбс  бсбсбсбс  бс  бс");

    _DrawText(20, 7, "бсбсбсбс   бсбс   бс      бсбс   бсбсбсбс  бсбсбсбс");
    _DrawText(20, 8, "бс  бс  бс  бс  бс     бс  бс  бс     бс");
    _DrawText(20, 9, "бсбсбсбс  бсбсбсбс  бс     бсбсбсбс  бс     бсбсбсбс");
    _DrawText(20, 10, "бс     бс  бс  бс     бс  бс  бс     бс");
    _DrawText(20, 11, "бс     бс  бс  бсбсбсбс  бс  бс  бсбсбсбс  бсбсбсбс");

    _DrawText(20, 13, "бсбсбсбс  бсбсбсбс  бсбсбсбс   бсбс   бсбсбсбс  бсбсбсбс");
    _DrawText(20, 14, "бс     бс     бс     бс  бс  бс  бс  бс");
    _DrawText(20, 15, "бсбсбсбс  бсбсбсбс  бс     бсбсбсбс  бсбсбсбс  бсбсбсбс");
    _DrawText(20, 16, "бс        бс  бс     бс  бс  бс     бс");
    _DrawText(20, 17, "бсбсбсбс  бсбсбсбс  бсбсбсбс  бс  бс  бс     бсбсбсбс");


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
