#include "map.h"
#include "Rabbit.h"
//================================================================

void Draw() // 화면 그리기
{
    // 게임 시작 전
    if (!GetGameStart()) {
        GameStartScreen();  // 게임시작 화면 출력
        // 문구 이펙트 효과
        if (GetGameStartText()) 
            _DrawText(23, 21, "아무 키나 눌러 게임 시작하기");
        else 
            _DrawText(21, 21, "                               ");
    }
    // 게임 시작 후
    else {
        // 게임오버했을 때
        if (IsGameOver)
        {
            GameOverScreen();   // 게임오버 화면 출력
            // 문구 이펙트 효과
            if (GameOverText)
                _DrawText(14, 21, "아무 키나 눌러 시작화면으로 돌아가기");
            else
                _DrawText(14, 21, "                                     ");
        }
        // 스테이지 클리어
        else if (StageClear())
        {
            _Delay(45);
            RabbitCAnim();  // 스테이지 클리어 화면 출력
            SetMapStatus(GetMapStatus() + 1);   // 맵 스테이터스 1 증가
            SetSettingItem(false);  // 스테이지 아이템 세팅 리셋
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
                DrawMapBG(); // 맵 틀 그리기
                _DrawText(3, 3, player.HeldWeapon->sprite); // 무기 그림그리기
                DrawHealth();   // 체력바 그리기


                // 스테이지 시작후 아이템 세팅이 안 되어있을 때
                if (!GetSettingItem()) {
                    for (int i = 0; i < numItem; i++)
                    {
                        // 현재 스테이지에 들어가는 아이템 보이게 하기
                        if (itemList[i].mapStatus == GetMapStatus()) {
                            itemList[i].isHeld = false;
                        }
                        else {
                            itemList[i].isHeld = true;
                        }
                    }
                    // 스테이지 아이템 세팅 완료
                    SetSettingItem(true);
                }
                
                // 아이템 출력
                for (int i = 0; i < numItem; i++)
                {
                    if (!itemList[i].isHeld) {

                        DrawItem(&itemList[i], GetFrame());
                    }
                }
                

            }

            DrawPlayer();


            // TODO: 몬스터가 살아있다면 몬스터 그리기 추가
            //DrawFish();
            //DrawCrab();
            //DrawClam();

            // 몬스터 출력
            for (int i = 0; i < numMonster; i++)
            {
                if (monsterList[i].alive) {

                    DrawMonster(monsterList[i]);
                }
            }


            // 플레이어 주변에 아이템이 있을 때 알림문구 출력
            if (IsNearItem())
            {
                _DrawText(player.Pos.x, player.Pos.y - 3.f, "e를 눌러 아이템 먹기");
            }

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
    UpdateSpeedBuffs(); // 속도 버프 지속시간 체크 및 종료 처리
    

    UpdateMonster();



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

    SetConsoleTitle("용궁탈출");

    InitMonster();  // 몬스터 초기화
    InitItem();  // 아이템 초기화
    InitWeapon(weaponList); // 무기 초기화
    InitPlayer();
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


        if (IsGameOver)
        {
            ReturnStartScreen();    // 게임오버 화면 출력
            main(); // 메인 재호출

        }
    }


    _EndWindow();
}