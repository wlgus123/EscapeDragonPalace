#include "init.h"
#include "item.h"
#include "Rabbit.h"
#include "map.h"
#include "weapon.h"
#include "monster.h"
#include "screens.h"

// ===============================================================

void Draw() // 화면 그리기
{
    // 게임 시작 전
    if (!GetGameStart()) {
        _SetColor(E_White); // 문구 색 변경
        GameStartScreen();  // 게임시작 화면 출력
        // 문구 이펙트 효과
        if (GetGameStartText())
            _SetColor(E_White); // 문구 색 변경
        else
            _SetColor(E_Gray); // 문구 색 변경
        _DrawText(23, 21, "아무 키나 눌러 게임 시작하기");

    }
    // 게임 시작 후
    else {
        // 게임오버했을 때
        if (GetIsGameOver())
        {
            _SetColor(E_White); // 문구 색 변경
            GameOverScreen();   // 게임오버 화면 출력
            // 문구 이펙트 효과
            if (GetGameOverText())
                _SetColor(E_White); // 문구 색 변경
            else
                _SetColor(E_Gray); // 문구 색 변경
            _DrawText(14, 21, "아무 키나 눌러 시작화면으로 돌아가기");

        }
        // 스테이지 클리어
        else if (StageClear())
        {
            RabbitSCAnim();  // 스테이지 클리어 화면 출력
            _Delay(45);
            // 여기 있던 거 Rabbit.c에 ISOnGoal()로 옮겼어요 _ 서영
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
                _SetColor(E_White); // 몬스터 외 색상 초기화

                // 플레이어 주변에 아이템이 있을 때 알림문구 출력
                if (IsNearItem())
                {
                    _DrawText(player.Pos.x, player.Pos.y - 3.f, "e를 눌러 아이템 먹기");
                }
            }

            // 플레이어 출력
            DrawPlayer();
            _SetColor(E_White);

            _DrawText(3, 3, player.HeldWeapon->sprite); // 무기 그림그리기
            DrawHealth();   // 체력바 그리기
            DrawBuffNDebuff();

            // 맵 틀 그리기
            DrawMap();
        }
    }
}
// ===========================================================

// 업데이트 내용
void Update()
{
    UpdateMapPos();

    UpdatePlayer();

    SetIsNearItem(false); // 플레이어가 아이템 근처에 있는지 여부 초기화

    CheckItemPickup();  // 아이템 먹었는지 체크
    UpdateBuffs(); // 속도 버프 지속시간 체크 및 종료 처리


    UpdateMonster();
	MonsterHitPlayer(); // 몬스터 피격 처리 함수
    PlayerHitMonster();

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
    InitMonster();  // 몬스터 초기화
    InitWeapon(weaponList); // 무기 초기화
    InitItem();  // 아이템 초기화
    while (true)
    {
        InitPlayer();

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
        }


        _EndWindow();
    }
}