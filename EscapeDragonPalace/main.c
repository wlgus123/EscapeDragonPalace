#include "init.h"
#include "item.h"
#include "Rabbit.h"
#include "map.h"
#include "weapon.h"
#include "monster.h"
#include "screens.h"
#include "turtle.h"
#include "fish_big.h"
#include "fish_small.h"

// ===============================================================

void Draw() // 화면 그리기
{
    // 게임 시작 전
    if (!GetGameStart()) {
        _SetColor(E_White); // 문구 색 변경
        GameStartScreen();  // 게임시작 화면 출력
        // 문구 이펙트 효과
        if (GetTextE())
            _SetColor(E_White); // 문구 색 변경
        else
            _SetColor(E_Gray); // 문구 색 변경
        _DrawText(26, 22, "아무 키나 눌러 게임 시작하기");


    }
    // 게임 시작 후
    else {
        // 게임오버했을 때
        if (GetIsGameOver())
        {
            _SetColor(E_White); // 문구 색 변경
            GameOverScreen();   // 게임오버 화면 출력
            // 문구 이펙트 효과
            if (GetTextE())
                _SetColor(E_White); // 문구 색 변경
            else
                _SetColor(E_Gray); // 문구 색 변경
            _DrawText(23, 21, "아무 키나 눌러 시작화면으로 돌아가기");


        }
        // 스테이지 클리어
        else if (StageClear())
        {
            RabbitSCAnim();  // 스테이지 클리어 화면 출력

            _Delay(45);
        }
		else if (GetTurtleHp() <= 0) // 자라 몬스터의 체력이 0 이하일 때 (보스 클리어)
        {
            _SetColor(E_White); // 문구 색 변경
            GameClearSceen(); // 게임 클리어 화면 출력
            // 문구 이펙트 효과
            if (GetTextE())
                _SetColor(E_White); // 문구 색 변경
            else
                _SetColor(E_Gray); // 문구 색 변경
            _DrawText(12, 21, "ESC로 게임 종료 혹은 아무 키나 눌러 시작화면으로 돌아가기");


        }
        // 플레이 중일 때
        else {

            // 무기 선택 안 했을 때
            if (!GetWeaponChosen())
            {
                for (int i = 0; i < NUMWEAPON; i++)
                {
                    int isSelected = (i == GetSelectedIndex()); // 현재 무기인지 여부
                    DrawWeapon(&weaponList[i], i, isSelected);  // 무기 정보 출력, 선택중인 무기는 노란색
                }
            }
            else
            {

                strcpy(weaponList[0].name, "장검");
                strcpy(weaponList[0].sprite, "--|====>");
                weaponList[0].attack = 2;
                weaponList[0].attackSpeed = 2;

                DrawMapBG(); // 맵 배경 그리기

                // 스테이지 시작후 아이템 세팅이 안 되어있을 때
                if (!GetMapSetting()) {
                    ItemSetting();
                }

                // 아이템 출력
                DrawItem();
                _SetColor(E_White); // 아이템 외 색상 초기화


                // 몬스터 출력  
                DrawMonster();
                if (GetMapStatus() == E_Ground) DrawTurtle();
                _SetColor(E_White); // 몬스터 외 색상 초기화

                // 플레이어 주변에 아이템이 있을 때 알림문구 출력
                if (IsNearItem())
                {
                    _DrawText(player.Pos.x, player.Pos.y - 3.f, "e를 눌러 아이템 먹기");
                }

                _DrawText(3, 3, player.HeldWeapon->sprite); // 무기 그림그리기
                DrawHealth();   // 체력바 그리기
                DrawBuffNDebuff();
            }

            // 플레이어 출력
            DrawPlayer();
            _SetColor(E_White);


            // 맵 틀 그리기
            DrawMap();
        }
    }
}
// ===========================================================

// 업데이트 내용
void Update()
{
    if (GetAsyncKeyState('H') & 0x8000)
    {
        g_Turtle.mon.hp = 0;
    }
    UpdateMapPos();

    UpdatePlayer();

    SetIsNearItem(false); // 플레이어가 아이템 근처에 있는지 여부 초기화

    CheckItemPickup();  // 아이템 먹었는지 체크
    UpdateBuffs(); // 속도 버프 지속시간 체크 및 종료 처리


    UpdateMonster();
	MonsterHitPlayer(); // 몬스터 피격 처리 함수
    PlayerHitMonster();
	PlayerHitTurtle(); // 플레이어가 자라를 공격했는지 체크

    // 보스맵일 경우 자라 업데이트
    unsigned long now = _GetTickCount();
    if (GetMapStatus() == E_Ground)
    {
        UpdateTurtle(now);
    }
}

// 키 입력
void Input()
{
    g_Key = _GetKey();
}

void main()
{
    // 초기화
    _BeginWindow();
    InitWeapon(weaponList); // 무기 초기화
    InitItem();  // 아이템 초기화
    while (true)
    {
        InitPlayer();
        ResetBigFish();
        ResetSmallFish();
        InitMonster();  // 몬스터 초기화
        unsigned long startTime = _GetTickCount();
        InitTurtle(startTime);  // 자라(보스) 초기화

        SetConsoleTitle("용궁탈출");

        DrawStartScreen();  // 시작화면 작동 함수 출력
        SelectWeapon(); // 무기 선택
        player.HeldWeapon = &weaponList[GetSelectedIndex()];    // 플레이어 무기 세팅

        //로직
        while (true)
        {
            GetInput();
            ItemFrameDelay();   // 아이템 모션 효과
            Input(); // 키 입력

            Update(); // 업데이트

            _Invalidate(); // 화면 그리기 (Draw() 함수 자동 적용)
            _Delay(30);

            if (GetIsGameOver())
            {
                system("cls"); // 화면 지우기

                ReturnStartScreen();    // 게임오버 화면 출력
                break;
            }
			if (GetTurtleHp() <= 0) // 자라 몬스터의 체력이 0 이하일 때 (보스 클리어)
			{
				system("cls"); // 화면 지우기
				DrawGameClearScreen(); // 게임 클리어 화면 출력
				break;
			}
        }


        _EndWindow();
    }
}