#include "init.h"
#include "Rabbit.h"

#define GX 70 // 목표 지점 X 좌표
#define GY 22 // 목표 지점 Y 좌표

//--------------------------------------------------

float amount;

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


bool g_KeyW = false;
bool g_KeyA = false;
bool g_KeyD = false;

bool halfHealth = false; // 체력 반칸

bool g_MouseClick = false;


//애니메이션 상태 관리
int animFrame;         // 0 ~ jumpFrames*2-1 (up + down)
int animRepeatCount;   // 애니메이션 반복 횟수
int jumpFrames;  // 올라가는 프레임 개수
int maxRepeats;  // 최대 반복 횟수
int centerX;
int baseY;
int jumpHeight;
int animFramesTotal; // 전체 애니메이션 길이 (up+down)

bool animGoingUp = true;  // 점프 중 올라가는지 여부

// --------------------------------------------------
// 플레이어와 오브젝트 충돌 체크
bool IsOverlap(Rect player, Rect Obj)
{
    return (player.x < Obj.x + Obj.w) && (player.x + player.w > Obj.x) &&
        (player.y < Obj.y + Obj.h) && (player.y + player.h > Obj.y);
}

// 플레이어의 충돌 범위 반환
Rect GetPlayerRect()
{
    return (Rect) { player.Pos.x, player.Pos.y, 8, 3 };
}

// 아이템의 충돌 범위 반환
Rect GetItemRect(Item item)
{
    return (Rect) { item.x - 7, item.y, item.width + 2, item.height };
}

// 아이템 먹었는지 체크
void CheckItemPickup()
{

    Rect playerRect = GetPlayerRect();

    for (int i = 0; i < numItem; i++)
    {
        if (itemList[i].isHeld) continue;

        Rect itemRect = GetItemRect(itemList[i]);

        if (IsOverlap(playerRect, itemRect))
        {
            isNearItem = true;  // 문구출력
            if (GetAsyncKeyState('E') & 0x8000)
            {
                itemList[i].isHeld = true;
                switch (itemList[i].type) {
                case ITEM_LIFE:
                    player.Health += itemList[i].value;
                    break;
                case ITEM_SPEED:
                    // 속도 증가값, 지속시간(ms)
                    amount = itemList[i].value;
                    DWORD duration = 5000; // 예: 5초

                    for (int j = 0; j < MAX_BUFFS; j++)
                    {
                        if (!speedBuffs[j].active)
                        {
                            speedBuffs[j].active = true;
                            speedBuffs[j].amount = amount;
                            speedBuffs[j].endTime = GetTickCount() + duration;
                            player.Speed += amount; // 즉시 적용
                            break;
                        }
                        break;
                    }
                    break;
                }
            }
        }
    }
}

// 속도 버프 지속시간 체크 및 종료 처리
void UpdateSpeedBuffs()
{
    DWORD now = GetTickCount();

    for (int i = 0; i < MAX_BUFFS; i++)
    {
        if (speedBuffs[i].active && now >= speedBuffs[i].endTime)
        {
            player.Speed -= speedBuffs[i].amount; // 원래대로 감소
            speedBuffs[i].active = false;
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
	_DrawText(36, 10, "아무 키나 눌러 다음 스테이지로 넘어가기");

    player.Pos.x = 10.0f;
    player.Pos.y = 21.0f;

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

    // 마우스 왼쪽 버튼 클릭 여부
    g_MouseClick = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
}

void ApplyGravity() // 중력 적용 함수
{

}

bool CheckGround() // 바닥 체크 함수
{
    
}

void PlayerYPosition() // 플레이어 Y 위치 조정 함수
{
	
}

void JumpFN()
{
    // 점프 시작
    if (!player.IsJumping && g_KeyW)
    {
        player.IsJumping = true;
        player.VelY = JUMP_POWER;
    }

    // 중력 적용
    if (player.IsJumping)
    {
        player.VelY += GRAVITY;
        if (player.VelY > MAX_FALL_SPEED)
            player.VelY = MAX_FALL_SPEED;

        player.Pos.y += player.VelY;

        if (player.Pos.y >= 21.0f)
        {
            player.Pos.y = 21.0f;
            player.VelY = 0.0f;
            player.IsJumping = false;
        }
    }
}

void AttackFN()
{
    // 공격 시작: 마우스 클릭 && 공격 중 아님
    if (g_MouseClick && !player.IsAttacking)
    {
        player.IsAttacking = true;
        player.AttackFrame = 0;
        player.attackStartTime = GetTickCount();

        switch (player.HeldWeapon ? player.HeldWeapon->attackSpeed : 1)
        {
        case 3: player.AttackFrameMax = 10; player.attackDuration = 100; break; // 단검
        case 2: player.AttackFrameMax = 15; player.attackDuration = 300; break; // 장검
        case 1: player.AttackFrameMax = 20; player.attackDuration = 500; break; // 창
        default: player.AttackFrameMax = 15; player.attackDuration = 300; break;
        }
    }

    // 공격 애니메이션 처리
    if (player.IsAttacking)
    {
        player.AttackFrame++;

        if (GetTickCount() - player.attackStartTime >= player.attackDuration)
        {
            player.IsAttacking = false;
            player.AttackFrame = 0;
        }
    }
}

void moveFN()
{
    // 이동 처리
    float move = player.IsJumping ? player.Speed * 1.2f : player.Speed;

    if (g_KeyA)
    {
        player.Pos.x -= move;
        player.Direction = 1;
    }
    if (g_KeyD)
    {
        player.Pos.x += move;
        player.Direction = 0;
    }
}

bool ISOnGoal()
{
    /*int idx;
    int dir = player.Direction;

    if (!weaponChosen)
        idx = dir == 0 ? 12 : 13;
    else {
        int weaponType = player.HeldWeapon ? player.HeldWeapon->attackSpeed : 2;
        if (weaponType == 3)      idx = dir == 0 ? 4 : 5;
        else if (weaponType == 2) idx = dir == 0 ? 0 : 1;
        else                      idx = dir == 0 ? 8 : 9;
        if (player.IsAttacking) {
            if (weaponType == 3)      idx = dir == 0 ? 6 : 7;
            else if (weaponType == 2) idx = dir == 0 ? 2 : 3;
            else                      idx = dir == 0 ? 10 : 11;
        }
    }

    for (int y = 0; y < RabbitY; y++) {
        const char* line = Rabbit[idx][y];
        int len = strlen(line);
        for (int x = 0; x < len; x++) {
            if (line[x] != ' ') {
                int px = (int)player.Pos.x + x;
                int py = (int)player.Pos.y + y;
                if (px == GX && py == GY)
                    return true;
            }
        }
    }
    return false;*/
}

// 키보드 버퍼 비우기 함수
void ClearInputBuffer()
{
    while (_kbhit()) _getch();
}

void UpdatePlayer() // 플레이어 이동 키 입력 처리 
{
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
    if (player.Pos.y > SCREEN_HEIGHT - RabbitY) player.Pos.y = SCREEN_HEIGHT - RabbitY;

    JumpFN();

    AttackFN();

    moveFN();

    // Rabbit이 @에 닿았는지 체크
    if (ISOnGoal())
    {
        stageClear = true;
    }

    if (_kbhit())
    {
        g_Key = _getch();

        if (stageClear == false)
        {
            ClearInputBuffer(); // 키보드 버퍼 비우기
        }
        else if (g_Key != -1 && stageClear == true)
        {
            stageClear = false; // 스테이지 클리어 상태 초기화
        }
    }
    
}

void DrawPlayer()
{
    int idx;
    int dir = player.Direction;

    if (!weaponChosen) 
    {
        // 무기 선택 전: 무기 없는 토끼 이미지 사용
        idx = dir == 0 ? 12 : 13; // 오른쪽/왼쪽
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
                _DrawText((int)player.Pos.x + x, (int)player.Pos.y + y, (char[]) { line[x], '\0' });
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

        for (size_t i = 0; i < player.Health / 2; i++)
        {
            _DrawText(x, 1, "★"); // 체력 아이콘 그리기
            x += 3;
        }

        if (halfHealth)
        {
            halfHealth = false;

            _DrawText(x, 1, "☆"); // 체력 반칸 아이콘 그리기
        }
    }
    else
    {
        IsGameOver = true;
    }
}

void InitPlayer() // 초기화
{
    player.Pos.x = 10.0f;
    player.Pos.y = 21.0f;
    player.Speed = 1.0f;
    player.Health = 10;
    player.VelY = 0.0f;
    player.IsJumping = false;
    player.Direction = 0;

    player.IsAttacking = false;
    player.AttackFrame = 0;
    player.attackStartTime = 0;

    // 무기
    player.HeldWeapon = &weaponList[selectedIndex];

    // 무기 속도에 따라 공격 지속 시간 설정
    int speed = player.HeldWeapon->attackSpeed;
    player.attackDuration = (speed == 3) ? 100 : (speed == 2) ? 200 : 400;

    switch (player.HeldWeapon->attackSpeed)
    {
    case 3:  // 단검
        player.attackDuration = 100;
        break;
    case 2:  // 장검
        player.attackDuration = 300;
        break;
    case 1:  // 창
        player.attackDuration = 500;
        break;
    }

    animFrame = 0;
    animRepeatCount = 0;
    jumpFrames = 5;
    maxRepeats = 4;
    centerX = 40;
    baseY = 20;
    jumpHeight = 2;
    animFramesTotal = 10;

    isNearItem = false;
}

