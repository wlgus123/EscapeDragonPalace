#include "init.h"
#include "weapon.h"


int selectedIndex = 0;   // 현재 선택 중인 무기 인덱스
bool weaponChosen = false;  // Enter로 선택했는지 여부

// 무기배열 초기화
void InitWeapon(Weapon* weapons) {

    // 인덱스 0번
    strcpy(weapons[0].name, "장검");
    strcpy(weapons[0].sprite, "--|====>");
    weapons[0].attack = 2;
    weapons[0].attackSpeed = 2;

    // 인덱스 1번
    strcpy(weapons[1].name, "단검");
    strcpy(weapons[1].sprite, "-|=>");
    weapons[1].attack = 1;
    weapons[1].attackSpeed = 3;

    // 인덱스 2번
    strcpy(weapons[2].name, "창");
    strcpy(weapons[2].sprite, "------>");
    weapons[2].attack = 3;
    weapons[2].attackSpeed = 1;

}

// 무기출력
void DrawWeapon(const Weapon* w, int i, int isSelected) {
    int baseX = 5 + (i * 28);

    if (isSelected)
        _SetColor(E_Yellow); // 노란색 (현재 선택중인 무기)
    else
        _SetColor(E_White); // 흰색 (기본)

    _DrawText(baseX, 10, "종류: ");
    _DrawText(baseX + 6, 10, w->name);


    sprintf(buffer, "공격력: %d", w->attack);
    _DrawText(baseX, 11, buffer);

    sprintf(buffer, "공격속도: %d", w->attackSpeed);
    _DrawText(baseX, 12, buffer);

    _DrawText(baseX, 13, "형태: ");
    _DrawText(baseX + 6, 13, w->sprite);

    _SetColor(E_White); // 색상 초기화
}

// 무기 선택 함수
void SelectWeapon() {

    // 무기 선택 안했을 때
    while (!weaponChosen)
    {
        // 키 입력 받기
        if (_kbhit()) {
            char key = _getch();

			// A or a 키를 누르면 왼쪽 무기 설명으로 이동
            if (key == 'a' || key == 'A') {
                selectedIndex = (selectedIndex - 1 + NUMWEAPON) % NUMWEAPON;
            }
			// D or d 키를 누르면 오른쪽 무기 설명으로 이동
            else if (key == 'd' || key == 'D') {
                selectedIndex = (selectedIndex + 1) % NUMWEAPON;
            }
			// Enter 키를 누르면 무기 선택
            else if (key == '\r') {
                weaponChosen = true; // 무기 선택 여부 true로 변경
            }
        }

        _Invalidate();
    }
}

// 무기 선택 여부 가져오기
bool GetWeaponChosen()
{
    return weaponChosen;
}

// 무기 선택 여부 세팅
void SetWeaponChosen(bool src)
{
    weaponChosen = src;
}

// 현재 선택중인 무기 인덱스 가져오기
int GetSelectedIndex()
{
    return selectedIndex;
}
