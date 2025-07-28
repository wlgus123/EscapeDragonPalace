#include "init.h"
#include "map.h"

// 함수

// 화면 그리기
void Draw()
{
    // 맵
    if (GetMapStatus() != E_Boss)
    {
        DrawNormalBG();
        DrawNormalMap(); // 맵 그리기
    }
    else
        DrawBossMap();
}

// 업데이트 내용
void Update()
{
    UpdateMapPos();
}

// 키 입력
void Input()
{
    g_Key = _GetKey();
}

void main()
{
    // 맵 크기 변경
    //col = 가로, lines = 세로
    system("mode con:cols=78 lines=25");
    SetConsoleTitle("용궁탈출");

    // init
    _BeginWindow();

    //로직
    while (true)
    {
        Input(); // 키 입력

        Update(); // 업데이트

        _Invalidate(); // 화면 그리기 (Draw() 함수 자동 적용)
    }

    _EndWindow();
}