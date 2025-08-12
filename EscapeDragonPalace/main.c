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

// ===========================================================

void Draw() // 화면 그리기
{
	// 게임 시작 전
	if (!GetGameStart())
	{
		_SetColor(E_White); // 문구 색 변경
		GameStartScreen();  // 게임시작 화면 출력
		// 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)
		if (GetTextE())
			_SetColor(E_White); // 문구 색 변경
		else
			_SetColor(E_Gray); // 문구 색 변경
		_DrawText(26, 22, "아무 키나 눌러 게임 시작하기");
	}
	else if (!GetControlScreen() && GetGameStart())
	{
		_SetColor(E_White);

		for (int y = 0; y < 21; y++)
		{
			_DrawText(12, 2 + y, Controls[y]);
		}

		if (GetTextE())
			_SetColor(E_White); // 문구 색 변경
		else
			_SetColor(E_Gray); // 문구 색 변경
		_DrawText(28, 22, "아무 키나 눌러 넘어가기");
	}
	// 게임 시작 후
	else {
		// 게임오버했을 때
		if (GetIsGameOver())
		{
			_SetColor(E_White); // 문구 색 변경
			GameOverScreen();   // 게임오버 화면 출력
			// 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)
			if (GetTextE())
				_SetColor(E_White); // 문구 색 변경
			else
				_SetColor(E_Gray); // 문구 색 변경
			_DrawText(12, 21, "ESC로 게임 종료 혹은 아무 키나 눌러 시작화면으로 돌아가기");
		}
		// 스테이지 클리어했을 때
		else if (StageClear())
		{
			RabbitSCAnim();     // 스테이지 클리어 화면 출력
			_Delay(45);         // 스테이지 클리어 애니메이션 딜레이
		}
		// 보스 클리어했을 때 (자라 몬스터의 체력이 0 이하일 때)
		else if (GetTurtleHp() <= 0)
		{
			_SetColor(E_White); // 문구 색 변경
			GameClearSceen();   // 게임 클리어 화면 출력
			// 문구 이펙트 효과 (흰색, 회색으로 깜빡거림)
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
			// 무기 선택 완료 후 게임 진행시
			else
			{
				DrawMapBG(); // 맵 배경 그리기

				// 스테이지 시작후 아이템 세팅이 안 되어있을 때
				if (!GetMapSetting()) {
					ItemSetting();  // 아이템 세팅
				}

				DrawItem();         // 아이템 그리기
				_SetColor(E_White); // 아이템 외 색상 초기화

				// 보스맵일 경우 자라 그리기
				if (GetMapStatus() == E_Ground) DrawTurtle();
				// 보스맵이 아닐 경우(일반 맵) 몬스터 그리기
				else DrawMonster();

				_SetColor(E_White); // 몬스터 외 색상 초기화

				// 플레이어 주변에 아이템이 있을 때 알림문구 출력
				if (IsNearItem())
					_DrawText(player.Pos.x, player.Pos.y - 3.f, "e를 눌러 아이템 먹기");

				_DrawText(3, 3, player.HeldWeapon->sprite); // 좌측 상단에 현재 사용중인 무기 스프라이트 그리기
				DrawHealth();       // 체력바 그리기
				DrawBuffNDebuff();  // 속도 버프 및 디버프 남은 시간 표시

				// 현재 맵 정보 그리기
				char mapTxt[STR_MAX] = "";	// 현재 맵 정보 문자열
				switch (GetMapStatus())
				{
				case E_Jail:
					strcpy(mapTxt, "감옥");
					break;
				case E_DragonPalace:
					strcpy(mapTxt, "용궁");
					break;
				case E_Sea1:
					strcpy(mapTxt, "바다1");
					break;
				case E_Sea2:
					strcpy(mapTxt, "바다2");
					break;
				case E_Ground:
					strcpy(mapTxt, "육지(보스)");
					break;
				}
				_DrawText(3, 5, mapTxt);
			}

			// 이 아래로는 무기 선택 안 했을 때, 무기 선택 완료 후 게임 진행시 모두 실행됨

			DrawPlayer();   // 플레이어 그리기
			_SetColor(E_White); // 플레이어 외 색상 초기화

			// 무기 선택 완료 후 보스맵일 경우 자라 체력 그리기
			if (GetWeaponChosen() && GetMapStatus() == E_Ground)
				DrawTurtleHP();

			// 맵 틀 그리기
			DrawMap();
		}
	}
}

// ===========================================================

void Update() // 업데이트 내용
{
	UpdateMapPos();         // 맵 위치 업데이트
	UpdatePlayer();         // 플레이어 업데이트
	SetIsNearItem(false);   // 플레이어가 아이템 근처에 있는지 여부 초기화
	CheckItemPickup();      // 아이템 먹었는지 체크
	UpdateBuffs();          // 속도 버프 지속시간 체크 및 종료 처리

	// 보스맵일 경우 자라 업데이트
	unsigned long now = _GetTickCount();
	if (GetMapStatus() == E_Ground)
	{
		UpdateTurtle(now);
		PlayerHitTurtle();      // 플레이어가 자라를 공격했는지 체크
	}
	// 보스맵이 아닐 경우(일반 맵) 몬스터 업데이트
	else {
		UpdateMonster();        // 몬스터 업데이트
		MonsterHitPlayer();     // 몬스터가 플레이어를 공격했는지 체크
		PlayerHitMonster();     // 플레이어가 몬스터를 공격했는지 체크 
	}
}

// ===========================================================

void Input() // 키 입력
{
	g_Key = _GetKey();
}

// ===========================================================

void main()
{
	// 초기화
	_BeginWindow();
	

	while (true)
	{
		InitWeapon(weaponList); // 무기 초기화
		InitPlayer();   // 플레이어 초기화
		InitMonster();  // 몬스터 초기화
		ResetItem();    // 아이템 alive false 처리
		InitItem();     // 아이템 초기화

		unsigned long startTime = _GetTickCount();
		InitTurtle(startTime);  // 자라(보스) 초기화

		SetConsoleTitle("용궁탈출");

		DrawStartScreen();  // 시작화면 작동 함수
		DrawControlsScreen();
		SelectWeapon();     // 무기 선택
		player.HeldWeapon = &weaponList[GetSelectedIndex()];    // 플레이어 무기 세팅

		// 게임 로직
		while (true)
		{
			if (!GetGameStart())
			{
				_getch(); // 키보드 버퍼 지우기
			}

			GetInput();
			ItemFrameDelay();   // 아이템 모션 효과
			Input();            // 키 입력
			Update();           // 업데이트
			_Invalidate();      // 화면 그리기 (Draw() 함수 자동 적용)
			_Delay(30);

			// 게임오버했을 때
			if (GetIsGameOver())
			{
				system("cls");          // 화면 지우기
				ReturnStartScreen();    // 게임오버 화면 작동 함수
				break;  // 게임 로직 종료
			}
			// 보스 클리어했을 때 (자라 몬스터의 체력이 0 이하일 때)
			if (GetTurtleHp() <= 0)
			{
				system("cls");          // 화면 지우기
				DrawGameClearScreen();  // 게임 클리어 화면 작동 함수
				break;  // 게임 로직 종료
			}
		}

		_EndWindow();
	}
}