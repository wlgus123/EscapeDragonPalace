#include "map.h"
#include "Rabbit.h"
// ===============================================================

bool MapSetting = false;	// 아이템 세팅여부 변수

// 맵 아이템, 몬스터 세팅 여부 가져오기
bool GetMapSetting()
{
    return MapSetting;
}

// 맵 아이템, 몬스터 세팅 여부 세팅하기
void SetMapSetting(bool src)
{
    MapSetting = src;
}

void FMapSetting() {
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
    for (int i = 0; i < numMonster; i++)
    {
        // 현재 스테이지에 들어가는 몬스터 보이게 하기
        if (monsterList[i].mapStatus == GetMapStatus()) {
            monsterList[i].alive = true;
        }
        else {
            monsterList[i].alive = false;
        }
    }
    // 스테이지 아이템 세팅 완료
    SetMapSetting(true);
}


// ===============================================================

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
            RabbitCAnim();  // 스테이지 클리어 화면 출력
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
                DrawMapBG(); // 맵 틀 그리기
                _DrawText(3, 3, player.HeldWeapon->sprite); // 무기 그림그리기
                DrawHealth();   // 체력바 그리기


                // 스테이지 시작후 아이템 세팅이 안 되어있을 때
                if (!GetMapSetting()) {
                    FMapSetting();
                }
                
                // 아이템 출력
                DrawItem();
                _SetColor(E_White); // 아이템 외 색상 초기화


                // 몬스터 출력

                DrawMonster();
                DrawTurtle();
                _SetColor(E_White); // 몬스터 외 색상 초기화

                // 플레이어 주변에 아이템이 있을 때 알림문구 출력
                if (IsNearItem())
                {
                    _DrawText(player.Pos.x, player.Pos.y - 3.f, "e를 눌러 아이템 먹기");
                }
            }

            // 플레이어 출력
            DrawPlayer();

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
    unsigned int now = _GetTickCount();
    UpdateTurtle(now);


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

    unsigned int startTime = _GetTickCount();
    InitTurtle(startTime);


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