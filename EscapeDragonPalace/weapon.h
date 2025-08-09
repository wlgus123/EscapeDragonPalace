#pragma once

#define NUMWEAPON 3 // 무기 종류 갯수
#define WEAPON_SPRITE_LENGTH  21 // 무기 스프라이트 길이

// 무기 구조체
typedef struct Weapon {
	char name[30];                      // 무기 이름
	char sprite[WEAPON_SPRITE_LENGTH];  // 무기 스프라이트
    int attack;                         // 공격력
    int attackSpeed;                    // 공격속도
} Weapon;

// 무기 인스턴스 구조체
typedef struct WeaponInstance {
    Weapon* weapon; // 무기 종류
    int x, y;       // 맵 위치
    int isHeld;     // 0: 바닥에 있음, 1: 플레이어가 소지
} WeaponInstance;

Weapon weaponList[NUMWEAPON];   // 무기 리스트
char buffer[50];

// 함수
extern void InitWeapon(Weapon* weapons);
extern void DrawWeapon(const Weapon* w, int i, int isSelected);
extern void SelectWeapon();
bool GetWeaponChosen(); 
void SetWeaponChosen(bool src);
int GetSelectedIndex();