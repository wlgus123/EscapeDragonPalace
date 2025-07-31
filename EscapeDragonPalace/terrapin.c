#include "monster.h"


#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

const char* turtle[] = {
    "        ______ ",
    "  ___ _/ \\__/ \\_   /|",
    " (_o / \\ /  \\ / \\_/ |",
    "  \\__ -----------__/",
    "      \\_|_|_|_|_/ ",
    "     /_|_|  /_|_|"
};

#define TURTLE_HEIGHT 6
#define TURTLE_WIDTH 22
#define TURTLE_Y (SCREEN_HEIGHT - TURTLE_HEIGHT)
#define TURTLE_X 55

#define WATER_CHAR '@'

enum Direction { UP, MIDDLE, DOWN };
enum Direction currentDir = MIDDLE;

int waterY = -1;
int waterLength = 0;  // 물대포 길이
int tcolor = 15;
int Hp = 80;

int getDirectionY(enum Direction dir) {
    switch (dir) {
    case UP: return TURTLE_Y + 1;
    case MIDDLE: return TURTLE_Y + 3;
    case DOWN: return TURTLE_Y + 5;
    }
    return TURTLE_Y + 3;
}

void DrawWaterBlast(int y, int length) {
    if (length > TURTLE_X) length = TURTLE_X;  // 자라 왼쪽 범위 넘어가지 않도록 제한

    char line[81] = { 0 };
    for (int i = 0; i < length; i++) {
        line[i] = WATER_CHAR;
    }
    line[length] = '\0';

    _DrawText(0, y, line);
}

void Draw() {
    // 화면 클리어
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        char blank[SCREEN_WIDTH + 1];
        for (int i = 0; i < SCREEN_WIDTH; i++) blank[i] = ' ';
        blank[SCREEN_WIDTH] = '\0';
        _DrawText(0, y, blank);
    }

    // 자라 그리기
    for (int i = 0; i < TURTLE_HEIGHT; i++) {
        _DrawTextColor(TURTLE_X, TURTLE_Y + i, (char*)turtle[i],tcolor);
    }

    // 방향 표시 (자라 앞 머리 왼쪽에 느낌표)
    int arrowY = getDirectionY(currentDir);
    _DrawText(TURTLE_X - 4, arrowY, "!!!");

    // 물대포 그리기
    if (waterLength > 0) {
        DrawWaterBlast(waterY, waterLength);
    }
}

void hit() {
    time_t start = time(NULL); // 타임함수를 이용
    time_t end = start + 1;
    Hp--;
    tcolor = 12;

    if (Hp == 0) {
        tcolor = 0;
    }

    _Invalidate();
    while (time(NULL) < end) {
        //대기
    }

    if (tcolor == 12) {
        tcolor = 15;
    }

    return start;

}

int main() {
    srand((unsigned int)time(NULL));
    _BeginWindow();

    while (1) {
        currentDir = (enum Direction)(rand() % 3);

        // ESC 키 누르면 종료
        char key = _GetKey();
        if (key == 27) break;
        if (key == 'w')
            hit();


        // 방향 예고 3초
        waterLength = 0;
        _Invalidate();
        _Delay(3000);

        // 물대포 일직선 한방에 빡 하고 2초 보여주기
        waterY = getDirectionY(currentDir);
        waterLength = TURTLE_X; // 자라 왼쪽 끝까지 꽉 채움
        _Invalidate();
        _Delay(2000);

        // 물대포 사라짐
        waterLength = 0;
        _Invalidate();

        
    }

    _EndWindow();
    return 0;
}