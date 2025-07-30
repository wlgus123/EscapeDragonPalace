#include "init.h"
#include "weapon.h"


int selectedIndex = 0;   // 현재 선택 중인 무기 인덱스
bool weaponChosen = false;  // Enter로 선택했는지 여부



void InitWeapon(Weapon* weapons) {
    strcpy(weapons[0].name, "장검");
    strcpy(weapons[0].sprite, "--|====>");
    weapons[0].attack = 2;
    weapons[0].attackSpeed = 2;


    strcpy(weapons[1].name, "단검");
    strcpy(weapons[1].sprite, "-|=>");
    weapons[1].attack = 1;
    weapons[1].attackSpeed = 3;



    strcpy(weapons[2].name, "창");
    strcpy(weapons[2].sprite, "------>");
    weapons[2].attack = 3;
    weapons[2].attackSpeed = 1;

}

void DrawWeapon(const Weapon* w, int i, int isSelected) {
    int baseX = 5 + (i * 28);

    if (isSelected)
        _SetColor(14); // 노란색 (선택된 무기)
    else
        _SetColor(7); // 흰색 (기본)

    _DrawText(baseX, 10, "종류: ");
    _DrawText(baseX + 6, 10, w->name);


    sprintf(buffer, "공격력: %d", w->attack);
    _DrawText(baseX, 11, buffer);

    sprintf(buffer, "공격속도: %d", w->attackSpeed);
    _DrawText(baseX, 12, buffer);

    _DrawText(baseX, 13, "형태: ");
    _DrawText(baseX + 6, 13, w->sprite);

    _SetColor(7); // 색상 초기화
}

void SelectWeapon() {

    while (!weaponChosen)
    {
        if (_kbhit()) {
            char key = _getch();

            if (key == 'a' || key == 'A') {
                selectedIndex = (selectedIndex - 1 + NUMWEAPON) % NUMWEAPON;
            }
            else if (key == 'd' || key == 'D') {
                selectedIndex = (selectedIndex + 1) % NUMWEAPON;
            }
            else if (key == '\r') { // Enter 키
                weaponChosen = true;
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

void SetWeaponChosen(bool src)
{
    weaponChosen = !src;
}

// 현재 선택중인 무기 인덱스 가져오기
int GetSelectedIndex()
{
    return selectedIndex;
}
