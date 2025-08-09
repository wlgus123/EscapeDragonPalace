#include "Rabbit.h"
#include "map.h"
#include "screens.h"


//--------------------------------------------------

char Rabbit[14][RabbitY][RabbitX] =
{
	{
		"        (\\(\\",
		"        ('-')",
		"       o(　--|====>"
	}, // 장검 오른쪽 기본
	{
		"         /)/)",
		"        ('-')",
		"  <====|--　)o"
	}, // 장검 왼쪽 기본
	{
		"        (\\(\\ ",
		"        ('-')",
		"       o(　　--|====>"
	}, // 장검 오른쪽 공격
	{
		"         /)/) ",
		"        ('-') ",
		"<====|--　　)o"
	}, // 장검 왼쪽 공격
	{
		"        (\\(\\ ",
		"        ('-')",
		"       o(　-|=>"
	}, // 단검 오른쪽 기본
	{
		"         /)/) ",
		"        ('-') ",
		"      <=|-　)o"
	}, // 단검 왼쪽 기본
	{
		"        (\\(\\ ",
		"        ('-')",
		"       o(　　-|=>"
	}, // 단검 오른쪽 공격
	{
		"         /)/) ",
		"        ('-') ",
		"    <=|-　　)o"
	}, // 단검 왼쪽 공격
	{
		"        (\\(\\ ",
		"        ('-')",
		"       o(　------>"
	}, // 창 오른쪽 기본
	{
		"         /)/) ",
		"        ('-') ",
		"   <------　)o"
	}, // 창 왼쪽 기본
	{
		"        (\\(\\ ",
		"        ('-')",
		"       o(　　------>"
	}, // 창 오른쪽 공격
	{
		"         /)/) ",
		"        ('-') ",
		" <------　　)o"
	}, // 창 왼쪽 공격
	{
		"        (\\(\\",
		"        ('-')",
		"       o(   )"
	}, // 플레이어 오른쪽
	{
		"         /)/) ",
		"        ('-') ",
		"        (   )o"
	}, // 플레이어 왼쪽
};



//애니메이션 상태 관리
int animFrame;         // 0 ~ jumpFrames*2-1 (up + down)
int animRepeatCount;   // 애니메이션 반복 횟수
int jumpFrames;  // 올라가는 프레임 개수
int maxRepeats;  // 최대 반복 횟수
int centerX;
int baseY;
int jumpHeight;
int animFramesTotal; // 전체 애니메이션 길이 (up+down)
int PrevPlayerHealth;


float amount;

SpeedBuff speedBuffs;
SpeedBuff slowDebuffs;

DWORD g_SKeyLastTime = 0;

bool animGoingUp = true;  // 점프 중 올라가는지 여부

bool isNearItem = false;    // 플레이어 주변에 아이템이 있으면 true

bool stageClear = false; // 스테이지 클리어 여부

bool g_KeyW = false;
bool g_KeyA = false;
bool g_KeyD = false;
bool g_KeyS = false;
bool g_KeySpace = false;

bool IsMapEnd = false;

bool halfHealth = false; // 체력 반칸


bool IsDamaged = false; // 플레이어가 피격당했는지 여부

bool IsNearLadder = false; // 플레이어가 사다리 근처에 있는지 여부

bool IsInvincibleTime = false; // 플레이어 무적 시간 여부

// --------------------------------------------------

bool SetInvincibleTime(bool src)
{
	IsInvincibleTime = src;
}

bool SetMapEnd(bool src)
{
	IsMapEnd = src;
}
bool GetKeyW()
{
	return g_KeyW;
}

bool GetKeyA()
{
	return g_KeyA;
}

bool GetKeyD()
{
	return g_KeyD;
}

bool GetKeyS()
{
	return g_KeyS;
}

bool IsNearItem()
{
	return isNearItem;
}

void SetIsNearItem(bool src)
{
	isNearItem = src;
}

bool StageClear()
{
	return stageClear;
}

// 플레이어와 오브젝트 충돌 체크
bool IsOverlap(Rect player, Rect Obj)
{
	return (player.x < Obj.x + Obj.w) && (player.x + player.w > Obj.x) &&
		(player.y < Obj.y + Obj.h) && (player.y + player.h > Obj.y);
}

// 플레이어의 충돌 범위 반환
Rect GetPlayerRect()
{
	return (Rect) { player.Pos.x + 8, player.Pos.y, 4, 3 };
}

// 아이템의 충돌 범위 반환
Rect GetItemRect(Item item)
{
	if (item.type == E_ITEM_DEBUFF) { return (Rect) { item.x + 1 - GetPlusX(), item.y, item.width - 3, item.height }; }
	return (Rect) { item.x - 4 - GetPlusX(), item.y, item.width + 6, item.height };
}

// 무기 충돌 범위 반환
Rect GetWeaponRect()
{
	int tempX = player.Pos.x;
	// 플레이어가 오른쪽 보고 있을 때
	if (player.Direction == E_Right) {
		switch (GetSelectedIndex())
		{
		case 0: // 장검
			return (Rect) { tempX + 14, player.Pos.y + 2, 8, 0 };
		case 1: // 단검
			return (Rect) { tempX + 14, player.Pos.y + 2, 4, 0 };
		case 2: // 창
			return (Rect) { tempX + 14, player.Pos.y + 2, 7, 0 };
		}
	}
	// 플레이어가 왼쪽 보고 있을 때
	else if (player.Direction == E_Left) {
		switch (GetSelectedIndex())
		{
		case 0: // 장검
			return (Rect) { tempX, player.Pos.y + 2, 8, 0 };
		case 1: // 단검
			return (Rect) { tempX + 4, player.Pos.y + 2, 4, 0 };
		case 2: // 창
			return (Rect) { tempX + 1, player.Pos.y + 2, 7, 0 };
		}
	}
}



// 아이템 먹었는지 체크
void CheckItemPickup()
{
	// 플레이어 충돌 범위 받아오기
	Rect playerRect = GetPlayerRect();

	for (int i = 0; i < g_ItemIdx; i++)
	{
		// 화면에 보이지 않는 아이템(이미 먹었거나 다른 스테이지)인 경우 넘어가기
		if (g_ItemList[i].isHeld) continue;

		// 아이템 충돌 범위 받아오기
		Rect itemRect = GetItemRect(g_ItemList[i]);

		// 플레이어와 아이템 충돌 체크, 충돌시
		if (IsOverlap(playerRect, itemRect))
		{
			// 먹은 아이템 타입에 따라
			switch (g_ItemList[i].type) {
			case E_ITEM_LIFE: // 목숨 추가 (해초)

				SetIsNearItem(true);  // 문구출력

				// e를 눌렀을 때
				if (GetAsyncKeyState('E') & 0x8000)
				{
					g_ItemList[i].isHeld = true;  // 화면에 안 보이게 처리
					player.Health += LIFEUP;	// 체력 증가

					PrevPlayerHealth = player.Health; //체력 저장

				}
				break;
			case E_ITEM_SPEED: // 이동속도 증가 (공기방울)

				SetIsNearItem(true);  // 문구출력

				// e를 눌렀을 때
				if (GetAsyncKeyState('E') & 0x8000)
				{
					g_ItemList[i].isHeld = true;  // 화면에 안 보이게 처리

					if (!speedBuffs.active)
					{
						speedBuffs.active = true;
						speedBuffs.endTime = GetTickCount();
						player.Speed += SPEEDUP; // 즉시 적용
					}

					speedBuffs.endTime += DURATION;  // 효과 지속 중 다시 먹었을 경우 시간 초기화

				}
				break;
			case E_ITEM_DEBUFF:	// 디버프 (조개)
				g_ItemList[i].isHeld = true;  // 화면에 안 보이게 처리

				if (!slowDebuffs.active)
				{
					slowDebuffs.active = true;
					slowDebuffs.endTime = GetTickCount();
					player.Speed -= SPEEDDOWN;	// 이동 속도는 누적 X
				}

				player.Health -= LIFEDOWN;	// 체력은 누적으로 닳게
				slowDebuffs.endTime += DURATION;  // 효과 지속 중 다시 먹었을 경우 시간 초기화

			}
		}
	}
}

// 속도 버프 지속시간 체크 및 종료 처리
void UpdateBuffs()
{
	if (speedBuffs.active && GetTickCount() >= speedBuffs.endTime)
	{
		player.Speed -= SPEEDUP; // 원래대로 감소
		speedBuffs.active = false;
	}

	if (slowDebuffs.active && GetTickCount() >= slowDebuffs.endTime)
	{
		player.Speed += SPEEDDOWN;	// 원래대로 증가
		slowDebuffs.active = false;
	}
}

void DrawBuffNDebuff() {
	char buf[32];
	if (speedBuffs.active) {
		if (slowDebuffs.active) {
			_DrawText(59, 2, "속도 버프: ");
			snprintf(buf, sizeof(buf), "%.1fs", (float)(speedBuffs.endTime - GetTickCount()) / 1000); // ms → 초 변환
			_DrawText(71, 2, buf);
			_DrawText(59, 3, "속도 디버프: ");
			snprintf(buf, sizeof(buf), "%.1fs", (float)(slowDebuffs.endTime - GetTickCount()) / 1000);
			_DrawText(73, 3, buf);
		}
		else {
			_DrawText(59, 2, "속도 버프: ");
			snprintf(buf, sizeof(buf), "%.1fs", (float)(speedBuffs.endTime - GetTickCount()) / 1000); // ms → 초 변환
			_DrawText(71, 2, buf);
		}
	}
	else {
		if (slowDebuffs.active) {
			_DrawText(59, 2, "속도 디버프: ");
			snprintf(buf, sizeof(buf), "%.1fs", (float)(slowDebuffs.endTime - GetTickCount()) / 1000);
			_DrawText(73, 2, buf);
		}
	}


}

void ClearRabbitAt(int x, int y)
{
	for (int row = 0; row < RabbitY; row++)
	{
		// RabbitX 크기만큼 공백 그리기
		char blankLine[RabbitX + 1];
		memset(blankLine, ' ', RabbitX);
		blankLine[RabbitX] = '\0';

		_DrawText(x, y + row, blankLine);
	}
}

void DrawRabbitAt(int x, int y, int idx)
{
	for (int row = 0; row < RabbitY; row++)
	{
		_DrawText(x, y + row, Rabbit[idx][row]);
	}
}

void RabbitCAnim() // Rabbit clear 애니메이션
{
	_DrawText(20, 10, "아무 키나 눌러 다음 스테이지로 넘어가기");

	player.Pos.x = RabbitXPos;
	player.Pos.y = RabbitYPos;

	static int prevX = -1, prevY = -1;

	// 이전 위치 지우기
	if (prevX >= 0 && prevY >= 0)
	{
		ClearRabbitAt(prevX, prevY);
	}

	// 올라가는 중인지 내려가는 중인지 판단
	int yOffset;
	if (animFrame < jumpFrames)
	{
		// 올라가는 중: yOffset 점점 커짐 (0,1,2,...)
		yOffset = (jumpHeight * animFrame) / jumpFrames;
	}
	else
	{
		// 내려가는 중: yOffset 점점 작아짐 (jumpHeight,...1,0)
		int downFrame = animFrame - jumpFrames;
		yOffset = (jumpHeight * (jumpFrames - downFrame)) / jumpFrames;
	}

	int currentY = baseY - yOffset;

	// 토끼 그리기
	int idx = 12;  // 오른쪽 토끼 스프라이트 인덱스
	DrawRabbitAt(centerX, currentY, idx);

	prevX = centerX;
	prevY = currentY;

	// 애니메이션 프레임 증가
	animFrame++;

	// 한 사이클 끝났으면
	if (animFrame >= jumpFrames * 2)
	{
		animFrame = 0;
		animRepeatCount++;

		// 애니메이션 반복 제한
		if (animRepeatCount >= maxRepeats)
		{
			animRepeatCount = 0;

			// 애니메이션 종료 후 고정 위치에 토끼 그리기
			ClearRabbitAt(prevX, prevY);
			DrawRabbitAt(centerX, baseY, idx);
		}
	}
}

void GetInput() // GetAsyncKeyState로 다중 키 입력 감지
{
	g_KeyW = (GetAsyncKeyState('W') & 0x8000);
	g_KeyA = (GetAsyncKeyState('A') & 0x8000);
	g_KeyD = (GetAsyncKeyState('D') & 0x8000);
	g_KeyS = (GetAsyncKeyState('S') & 0x8000);

	g_KeySpace = (GetAsyncKeyState(' ') & 0x8000);

}

// Rabbit가 현재 발판 위에 있는지 확인 (g_StagePlatform 또는 g_Map에 '='가 있으면 true)
bool CheckGround()
{
	int FpxL = (int)(player.Pos.x + 8) + GetPlusX();
	int FpxR = (int)(player.Pos.x + 12) + GetPlusX();
	int MpxL = (int)(player.Pos.x + 8);
	int MpxR = (int)(player.Pos.x + 12);
	int py = (int)(player.Pos.y + RabbitY);

	// x좌표 경계 보정
	if (FpxL < 0) FpxL = 0;
	if (FpxR >= MAP_WIDTH) FpxR = MAP_WIDTH - 1;
	if (MpxL < 0) MpxL = 0;
	if (MpxR >= MAP_WIDTH) MpxR = MAP_WIDTH - 1;

	for (int x = FpxL; x <= FpxR; x++)
	{
		int mapStatus = GetMapStatus();

		if (mapStatus < 0 || mapStatus >= 5 || py < 0 || py >= 25)
			return false;

		if (g_StagePlatform[mapStatus][py][x] == '=')
			return true;
	}
	for (int x = MpxL; x <= MpxR; x++)
	{
		if (py < 0 || py >= 25)
			return false;

		if (g_Map[py][x] == '=')
			return true;
	}
	return false;
}

// 중력 적용 함수: 발 아래가 발판이 아니면 아래로 떨어짐
void ApplyGravity()
{
	if (!CheckGround())
	{
		if (player.Pos.y > SCREEN_HEIGHT - RabbitY)
			player.Pos.y = SCREEN_HEIGHT - RabbitY;
		player.Pos.y += 1.0f; // 한 칸씩 아래로
	}
}

// 발 아래 발판의 y좌표를 반환 (없으면 -1)
int GetGroundY()
{
	int FpxL = (player.Pos.x + 8) + GetPlusX();
	int FpxR = (player.Pos.x + 12) + GetPlusX();
	int MpxL = (player.Pos.x + 8);
	int MpxR = (player.Pos.x + 12);
	int py = (int)(player.Pos.y + RabbitY);

	if (py >= SCREEN_HEIGHT) return -1;

	// x좌표 경계 보정
	if (FpxL < 0) FpxL = 0;
	if (FpxR >= MAP_WIDTH) FpxR = MAP_WIDTH - 1;
	if (MpxL < 0) MpxL = 0;
	if (MpxR >= MAP_WIDTH) MpxR = MAP_WIDTH - 1;

	for (int x = FpxL; x <= FpxR; x++)
	{
		int mapStatus = GetMapStatus();

		if (mapStatus < 0 || mapStatus >= 5 || py < 0 || py >= 25)
			return false;

		if (g_StagePlatform[mapStatus][py][x] == '=')
			return (py - 1);
	}
	for (int x = MpxL; x <= MpxR; x++)
	{
		if (py < 0 || py >= 25)
			return false;

		if (g_Map[py][x] == '=')
			return (py - 1);
	}

	return -1;
}

void JumpFN()
{
	// 점프 시작
	if (!player.IsJumping && g_KeyW && CheckGround())
	{
		player.IsJumping = true;
		player.VelY = JUMP_POWER;
	}

	if (player.IsJumping)
	{
		player.VelY += GRAVITY;
		if (player.VelY > MAX_FALL_SPEED)
			player.VelY = MAX_FALL_SPEED;

		// 점프 중 아래에 발판이 생기면 멈춤
		if (player.VelY >= 0 && CheckGround())
		{
			int groundY = GetGroundY();
			if (groundY != -1)
				player.Pos.y = groundY - RabbitY + 1; // 발판 바로 위에 정렬
			else
				player.Pos.y = (int)player.Pos.y;

			player.VelY = 0.0f;
			player.IsJumping = false;
		}

		float nextY = player.Pos.y + player.VelY;
		float step = (player.VelY > 0) ? 1.0f : -1.0f;
		bool landed = false;

		// y축 이동을 1씩 쪼개서 검사
		while ((step > 0 && player.Pos.y < nextY) || (step < 0 && player.Pos.y > nextY))
		{
			player.Pos.y += step;

			if (player.VelY >= 0 && CheckGround())
			{
				int groundY = GetGroundY();
				if (groundY != -1)
					player.Pos.y = groundY - RabbitY + 1;
				else
					player.Pos.y = (int)player.Pos.y;

				player.VelY = 0.0f;
				player.IsJumping = false;
				landed = true;
				break;
			}
		}

		// 남은 이동량 처리(착지하지 않은 경우)
		if (!landed)
			player.Pos.y = nextY;

		// 맵 아래로 떨어지는 것 방지
		if (player.Pos.y > SCREEN_HEIGHT - RabbitY)
		{
			player.Pos.y = SCREEN_HEIGHT - RabbitY - 1;
			player.VelY = 0.0f;
			player.IsJumping = false;
		}
	}
}

void AttackFN()
{
	// 공격 시작: 마우스 클릭했을 때 공격 중이 아니면
	if (g_KeySpace && !player.IsAttacking)
	{
		// 공격중으로 변경
		player.IsAttacking = true;
		player.AttackFrame = 0;
		player.attackStartTime = GetTickCount();

	}

	// 공격 애니메이션 처리
	if (player.IsAttacking)
	{
		player.AttackFrame++;


		if (GetTickCount() - player.attackStartTime >= player.attackDuration)
		{
			player.IsAttacking = false;
			player.AttackFrame = 0;

			// 무기 충돌 범위 받아오기
			Rect weaponRect = GetWeaponRect();
			//for (int i = 0; i < numMonster; i++)
			//{
			//    // 화면에 보이지 않는 몬스터(이미 죽었거나 다른 스테이지)인 경우 넘어가기
			//    if (!monsterList[i].alive) continue;

			//    // 몬스터 충돌 범위 받아오기
			//    Rect monsterRect = GetMonsterRect(monsterList[i]);

			//    // 무기와 몬스터 충돌 체크, 충돌시
			//    if (IsOverlap(weaponRect, monsterRect))
			//    {
			//        // 몬스터 공격
			//        HitMonster(&monsterList[i], player.HeldWeapon, player.attackStartTime);
			//    }
			//}
		}
	}
}

void moveFN()
{
	// 이동 처리
	float move = player.IsJumping ? player.Speed * 1.2f : player.Speed;


	// A와 D가 동시에 눌리면 둘 다 false 처리
	if (g_KeyA && g_KeyD)
	{
		g_KeyA = false;
		g_KeyD = false;
	}

	if (g_KeyA)
	{
		if (IsMapEnd)
		{
			move = player.IsJumping ? player.Speed * 1.2f : player.Speed;
		}
		else if (player.Pos.x < 25)
		{
			move = 0;
		}
		player.Pos.x -= move;
		player.Direction = 1;
	}
	if (g_KeyD)
	{
		if (IsMapEnd)
		{
			move = player.IsJumping ? player.Speed * 1.2f : player.Speed;
		}
		else if (player.Pos.x > 25)
		{
			move = 0;
		}
		player.Pos.x += move;
		player.Direction = 0;
	}

	if (g_KeyS)
	{
		if ((player.Pos.y + RabbitY) > 21) return; // 화면 밖으로 내려가지 않도록
		DWORD now = GetTickCount64();
		int KeyignoreTime = 250; // 키 입력 무시 시간 (ms)

		if (now - g_SKeyLastTime < KeyignoreTime) {
			return; // 키 입력 무시
		}

		g_SKeyLastTime = now;

		player.Pos.y++;

		CheckGround();

		if (!player.IsJumping && !CheckGround())
		{
			ApplyGravity();
		}
	}
}

void ClimbLadder()
{
	int PxL = (int)(player.Pos.x + 6);
	int PxR = (int)(player.Pos.x + 14);
	int Py = (int)player.Pos.y;

	// x좌표 경계 보정
	if (PxL < 0) PxL = 0;
	if (PxR >= MAP_WIDTH) PxR = MAP_WIDTH - 1;

	for (int y = Py; y < SCREEN_HEIGHT; y++)
	{
		for (int x = PxL; x <= PxR; x++)
		{
			int mapStatus = GetMapStatus();

			if (mapStatus < 0 || mapStatus >= 5 || Py < 0 || Py >= 25)
				return;

			if (g_StagePlatform[mapStatus][Py][x] == '_')
			{
				IsNearLadder = true; // 사다리 근처에 있으면 true

				if (!player.IsJumping && CheckGround())
				{
					if (GetAsyncKeyState('Q') & 0x8000)
					{// 사다리 근처에서 Q키를 누르면 사다리 올라가기
						IsNearLadder = false;
						player.Pos.y -= 8.0f; // 사다리 위로 올라가기
					}
				}

				return; // 사다리 근처에 있으면 함수 종료
			}
		}
	}


	IsNearLadder = false;
}

void ISOnGoal()
{
	int pxL = (int)(player.Pos.x + 8) + GetPlusX();
	int pxR = (int)(player.Pos.x + 12) + GetPlusX();
	int py = (int)player.Pos.y;

	for (int x = pxL; x <= pxR; x++)
	{
		int mapStatus = GetMapStatus();
		if (mapStatus < 0 || mapStatus >= 5 || py < 0 || py >= 25 || x < 0 || x >= 700)
			return false;

		if (g_StagePlatform[mapStatus][py][x] == '@')// Rabbit이 @에 닿았는지 체크
		{
			stageClear = true;
		}
	}
}

// 키보드 버퍼 비우기 함수
void ClearInputBuffer()
{
	g_Key = NULL;
}

void UpdatePlayer() // 플레이어 이동 키 입력 처리 
{
	if (IsInvincibleTime && (GetTickCount() - player.lastHitTime >= INVINCIBLE_TIME)) {
		SetInvincibleTime(false);
	}

	ISOnGoal(); // 플레이어가 목표에 도달했는지 체크

	if (!stageClear)
	{
		if (_kbhit())
		{
			ClearInputBuffer();
		}
	}
	else
	{
		if (g_Key != -1)
		{
			stageClear = false;

			system("cls"); // 화면 지우기

			SetPlusX(0); // 플레이어가 목표에 도달했을 때, 맵의 x좌표를 초기화
			SetMapStatus(GetMapStatus() + 1);   // 맵 스테이터스 1 증가

			SetMapSetting(false);  // 스테이지 아이템 세팅 리셋


			player.Speed = 1; // 원래대로 감소
			speedBuffs.active = false;
			slowDebuffs.active = false;

			player.Pos.x = RabbitXPos; // 플레이어 x위치 초기화
			player.Pos.y = RabbitYPos; // 플레이어 y위치 초기화
		}

	}

	CheckGround(); // 플레이어가 땅에 있는지 체크

	if (!player.IsJumping && !CheckGround())
	{
		ApplyGravity();
	}

	int iR = 0;
	int iL = 0;

	switch (player.HeldWeapon->attackSpeed)
	{
	case 3:  // 단검
		iR = 17;
		iL = 2;
		break;
	case 2:  // 장검
		iR = 21;
		iL = -2;
		break;
	case 1:  // 창
		iR = 20;
		iL = -1;
		break;
	}

	// 경계 체크
	if (player.Pos.x < 0 - iL) player.Pos.x = 0 - iL;
	if (player.Pos.x > SCREEN_WIDTH - iR) player.Pos.x = SCREEN_WIDTH - iR;
	if (player.Pos.y < 0) player.Pos.y = 0;
	if (player.Pos.y > SCREEN_HEIGHT - RabbitY) player.Pos.y = SCREEN_HEIGHT - RabbitY - 1;

	JumpFN();

	AttackFN();

	moveFN();

	ClimbLadder(); // 플레이어가 사다리 근처에 있는지 체크
}

char Color = E_White; // 플레이어 색상

void DrawPlayer()
{
	if (IsNearLadder)
	{
		_DrawText(player.Pos.x - 3, player.Pos.y - 3, "'Q' 키를 눌러 위로 올라가기");
	}

	if (player.Health < PrevPlayerHealth)
	{
		Color = E_Gray;
		PrevPlayerHealth = player.Health; // 이전 체력 갱신
	}
	else if (!IsInvincibleTime)
	{
		Color = E_White;
	}

	int idx;
	int dir = player.Direction;

	if (!GetWeaponChosen())
	{
		//무기 선택 화면에 따라 플레이어 스프라이트 인덱스 변경
		int selectedIndex = GetSelectedIndex();

		if (selectedIndex == 0)
		{
			idx = dir == 0 ? 0 : 1; // 장검
		}
		else if (selectedIndex == 1)
		{
			idx = dir == 0 ? 4 : 5; // 단검
		}
		else
		{
			idx = dir == 0 ? 8 : 9; // 창
		}
	}
	else
	{
		int weaponType = player.HeldWeapon ? player.HeldWeapon->attackSpeed : 2;
		if (weaponType == 3)      idx = dir == 0 ? 4 : 5;
		else if (weaponType == 2) idx = dir == 0 ? 0 : 1;
		else                      idx = dir == 0 ? 8 : 9;

		if (player.IsAttacking)
		{
			if (weaponType == 3)      idx = dir == 0 ? 6 : 7;
			else if (weaponType == 2) idx = dir == 0 ? 2 : 3;
			else                      idx = dir == 0 ? 10 : 11;
		}
	}

	for (int y = 0; y < RabbitY; y++)
	{
		const char* line = Rabbit[idx][y];
		int len = strlen(line);
		for (int x = 0; x < len; x++)
		{
			if (line[x] != ' ')
			{
				_DrawTextColor((int)player.Pos.x + x, (int)player.Pos.y + y, (char[]) { line[x], '\0' }, Color);
			}
		}
	}
}

void DrawHealth() // 플레이어 체력 그리기
{
	if (player.Health > 0)
	{

		if (player.Health % 2 == 1)
		{
			halfHealth = true; // 체력 반칸 있다
		}

		int x = 3;

		for (int i = 0; i < player.Health / 2; i++)
		{
			_DrawText(x, 1, "O"); // 체력 아이콘 그리기
			x += 3;
		}

		if (halfHealth)
		{
			halfHealth = false;

			_DrawText(x, 1, "o"); // 체력 반칸 아이콘 그리기
		}
	}
	else
	{
		SetIsGameOver(true);
	}
}

void InitPlayer() // 초기화
{
	g_Key = _GetKey;

	player.Pos.x = RabbitXPos;
	player.Pos.y = RabbitYPos;
	player.Speed = 1.2f;
	player.Health = 10;
	player.VelY = 0.0f;
	player.IsJumping = false;
	player.Direction = 0;

	PrevPlayerHealth = player.Health; // 이전 체력 초기화

	player.IsAttacking = false;
	player.AttackFrame = 0;
	player.attackStartTime = 0;

	// 무기
	player.HeldWeapon = &weaponList[GetSelectedIndex()];

	// 무기 속도에 따라 공격 지속 시간 설정
	int speed = player.HeldWeapon->attackSpeed;

	switch (speed)
	{
	case 3:
		player.AttackFrameMax = 10;
		player.attackDuration = 100;
		break; // 단검
	case 2:
		player.AttackFrameMax = 15;
		player.attackDuration = 300;
		break; // 장검
	case 1:
		player.AttackFrameMax = 20;
		player.attackDuration = 500;
		break; // 창
	}

	animFrame = 0;
	animRepeatCount = 0;
	jumpFrames = 5;
	maxRepeats = 4;
	centerX = 30;
	baseY = 20;
	jumpHeight = 2;
	animFramesTotal = 10;

	isNearItem = false;

	player.lastHitTime = 0;

	speedBuffs.endTime = 0;
	slowDebuffs.endTime = 0;

	speedBuffs.active = false;
	slowDebuffs.active = false;
}

