#include "init.h"
#include "map.h"
#include "item.h"
#include "screens.h"
#include "Rabbit.h"
#include "weapon.h"

// 함수

// ===========================================================
void Draw() // 화면 그리기
{
    // 게임 시작 전
    if (!GetGameStart()) {
        GameStartScreen();
        if (GetGameStartText()) 
            _DrawText(23, 21, "아무 키나 눌러 게임 시작하기");
        else 
            _DrawText(21, 21, "                               ");
    }
    // 게임 시작 후
    else {
        if (IsGameOver)
        {
            GameOverScreen();
            if (GameOverText) _DrawText(14, 21, "아무 키나 눌러 시작화면으로 돌아가기");
            else _DrawText(14, 21, "                                     ");
        }
        // 스테이지 클리어
        else if (StageClear())
        {
            _Delay(45);
            RabbitCAnim();
            SetMapStatus();
            for (int i = 0; i < numItem; i++)
            {
                if (itemList[i].mapStatus == GetMapStatus()) {
                    itemList[i].isHeld = false;
                }
                else {
                    itemList[i].isHeld = true;
                }
            }
        }
        else {

            if (!GetWeaponChosen())
            {
                for (int i = 0; i < NUMWEAPON; i++)
                {
                    int isSelected = (i == GetSelectedIndex()); // 현재 무기인지 여부
                    DrawWeapon(&weaponList[i], i, isSelected);
                }
            }
            else
            {
                DrawMap(); // 맵 그리기
                _DrawText(3, 3, player.HeldWeapon->sprite);
                DrawHealth();

                for (int i = 0; i < numItem; i++)
                {
                    if (!itemList[i].isHeld) {
                        DrawItem(&itemList[i], frame);
                    }
                }

            }

            DrawPlayer();




            if (IsNearItem())
            {
                // 수정 ================
                _DrawText(player.Pos.x, player.Pos.y - 3.f, "e를 눌러 아이템 먹기");
                // =====================

            }
        }

    }
}
// ===========================================================

// 업데이트 내용
void Update()
{
    UpdateMapPos();
    // 하영 ====================================
    UpdatePlayer();

	SetIsNearItem(false); // 플레이어가 아이템 근처에 있는지 여부 초기화

    CheckItemPickup();
    UpdateSpeedBuffs();
    // 하영 ====================================

}

// 키 입력
void Input()
{
    g_Key = _GetKey();
}

void main()
{
    // init
    _BeginWindow();

    // 맵 크기 변경
    //col = 가로, lines = 세로
    SetConsoleTitle("용궁탈출");

    InitPlayer();
    InitItem();  // 아이템 초기화
    InitWeapon(weaponList); // 무기 초기화
    DrawStartScreen();
    SelectWeapon();
    player.HeldWeapon = &weaponList[GetSelectedIndex()];

    //로직
    while (true)
    {
        GetInput();
        ItemFrameDelay();
        Input(); // 키 입력

        Update(); // 업데이트

        _Invalidate(); // 화면 그리기 (Draw() 함수 자동 적용)
        _Delay(30);

        if (IsGameOver)
        {
            ReturnStartScreen();
            main();

        }
    }

    _EndWindow();
}